#include "kiero.hpp"

#include <Windows.h>
#include <cassert>
#include <concepts>

#if KIERO_USE_MINHOOK
    #include <MinHook.h>
#endif

#ifdef _UNICODE
    #define KIERO_TEXT(text) L##text
#else
    #define KIERO_TEXT(text) text
#endif

static kiero::RenderType g_renderType = kiero::RenderType::None;
static uintptr_t* g_methodsTable = NULL;

#if KIERO_INCLUDE_D3D9
    #include "kiero_d3d9.hpp"
#endif

#if KIERO_INCLUDE_D3D10
    #include "kiero_d3d10.hpp"
#endif

#if KIERO_INCLUDE_D3D11
    #include "kiero_d3d11.hpp"
#endif

#if KIERO_INCLUDE_D3D12
    #include "kiero_d3d12.hpp"
#endif

#if KIERO_INCLUDE_OPENGL
    #include "kiero_opengl.hpp"
#endif

#if KIERO_INCLUDE_VULKAN
    #include "kiero_vulkan.hpp"
#endif

namespace
{
    template<std::invocable Fn>
    struct Defer
    {
        Fn impl{};

        ~Defer()
        {
            impl();
        }
    };
}

kiero::Status kiero::init(RenderType _renderType)
{
    if (g_renderType != RenderType::None)
    {
        return Status::AlreadyInitializedError;
    }

    if (_renderType != RenderType::None)
    {
        if (_renderType >= RenderType::D3D9 && _renderType <= RenderType::D3D12)
        {
            WNDCLASSEX windowClass;
            windowClass.cbSize = sizeof(WNDCLASSEX);
            windowClass.style = CS_HREDRAW | CS_VREDRAW;
            windowClass.lpfnWndProc = DefWindowProc;
            windowClass.cbClsExtra = 0;
            windowClass.cbWndExtra = 0;
            windowClass.hInstance = GetModuleHandle(NULL);
            windowClass.hIcon = NULL;
            windowClass.hCursor = NULL;
            windowClass.hbrBackground = NULL;
            windowClass.lpszMenuName = NULL;
            windowClass.lpszClassName = KIERO_TEXT("Kiero");
            windowClass.hIconSm = NULL;

            ::RegisterClassEx(&windowClass);
            Defer d1{[&] {
                ::UnregisterClass(windowClass.lpszClassName, windowClass.hInstance);
            }};

            HWND window = ::CreateWindow(windowClass.lpszClassName, KIERO_TEXT("Kiero DirectX Window"), WS_OVERLAPPEDWINDOW, 0, 0, 100, 100, NULL, NULL, windowClass.hInstance, NULL);
            Defer d2{[&] {
                ::DestroyWindow(window);
            }};

            if (_renderType == RenderType::D3D9)
            {
#if KIERO_INCLUDE_D3D9
                return d3d9::init(window);
#endif
            }
            else if (_renderType == RenderType::D3D10)
            {
#if KIERO_INCLUDE_D3D10
                return d3d10::init(window);
#endif
            }
            else if (_renderType == RenderType::D3D11)
            {
#if KIERO_INCLUDE_D3D11
                return d3d11::init(window);
#endif
            }
            else if (_renderType == RenderType::D3D12)
            {
#if KIERO_INCLUDE_D3D12
                return d3d12::init(window);
#endif
            }

            return Status::NotSupportedError;
        }
        else if (_renderType != RenderType::Auto)
        {
            if (_renderType == RenderType::OpenGL)
            {
#if KIERO_INCLUDE_OPENGL
                return opengl::init();
#endif
            }
            else if (_renderType == RenderType::Vulkan)
            {
#if KIERO_INCLUDE_VULKAN
                return vulkan::init();
#endif
            }

            return Status::NotSupportedError;
        }
        else
        {
            RenderType type = RenderType::None;

            if (::GetModuleHandle(KIERO_TEXT("d3d9.dll")) != NULL)
            {
                type = RenderType::D3D9;
            }
            else if (::GetModuleHandle(KIERO_TEXT("d3d10.dll")) != NULL)
            {
                type = RenderType::D3D10;
            }
            else if (::GetModuleHandle(KIERO_TEXT("d3d11.dll")) != NULL)
            {
                type = RenderType::D3D11;
            }
            else if (::GetModuleHandle(KIERO_TEXT("d3d12.dll")) != NULL)
            {
                type = RenderType::D3D12;
            }
            else if (::GetModuleHandle(KIERO_TEXT("opengl32.dll")) != NULL)
            {
                type = RenderType::OpenGL;
            }
            else if (::GetModuleHandle(KIERO_TEXT("vulkan-1.dll")) != NULL)
            {
                type = RenderType::Vulkan;
            }
            else
            {
                return Status::NotSupportedError;
            }

            return init(type);
        }
    }

    return Status::Success;
}

void kiero::shutdown()
{
    if (g_renderType != RenderType::None)
    {
#if KIERO_USE_MINHOOK
        MH_DisableHook(MH_ALL_HOOKS);
#endif

        ::free(g_methodsTable);
        g_methodsTable = NULL;
        g_renderType = RenderType::None;
    }
}

kiero::Status kiero::bind(uint16_t _index, void** _original, void* _function)
{
    // TODO: Need own detour function

    assert(_original != NULL && _function != NULL);

    if (g_renderType != RenderType::None)
    {
#if KIERO_USE_MINHOOK
        const auto target = reinterpret_cast<void*>(g_methodsTable[_index]);
        if (MH_CreateHook(target, _function, _original) != MH_OK || MH_EnableHook(target) != MH_OK)
        {
            return Status::UnknownError;
        }
#endif

        return Status::Success;
    }

    return Status::NotInitializedError;
}

void kiero::unbind(uint16_t _index)
{
    if (g_renderType != RenderType::None)
    {
#if KIERO_USE_MINHOOK
        MH_DisableHook(reinterpret_cast<void*>(g_methodsTable[_index]));
#endif
    }
}

kiero::RenderType kiero::getRenderType()
{
    return g_renderType;
}

uintptr_t* kiero::getMethodsTable()
{
    return g_methodsTable;
} 