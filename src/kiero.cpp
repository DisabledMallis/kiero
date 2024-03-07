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
static uintptr_t* g_methodsTable = nullptr;

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

namespace kiero
{
    static kiero::Status initRenderType(kiero::RenderType renderType);
}

static kiero::Status kiero::initRenderType(kiero::RenderType renderType)
{
    if (renderType >= RenderType::D3D9 && renderType <= RenderType::D3D12)
    {
        WNDCLASSEX windowClass;
        windowClass.cbSize = sizeof(WNDCLASSEX);
        windowClass.style = CS_HREDRAW | CS_VREDRAW;
        windowClass.lpfnWndProc = DefWindowProc;
        windowClass.cbClsExtra = 0;
        windowClass.cbWndExtra = 0;
        windowClass.hInstance = GetModuleHandle(nullptr);
        windowClass.hIcon = nullptr;
        windowClass.hCursor = nullptr;
        windowClass.hbrBackground = nullptr;
        windowClass.lpszMenuName = nullptr;
        windowClass.lpszClassName = KIERO_TEXT("Kiero");
        windowClass.hIconSm = nullptr;

        ::RegisterClassEx(&windowClass);
        Defer d1{[&] {
            ::UnregisterClass(windowClass.lpszClassName, windowClass.hInstance);
        }};

        HWND window = ::CreateWindow(windowClass.lpszClassName, KIERO_TEXT("Kiero DirectX Window"), WS_OVERLAPPEDWINDOW, 0, 0, 100, 100, nullptr, nullptr, windowClass.hInstance, nullptr);
        Defer d2{[&] {
            ::DestroyWindow(window);
        }};

#if KIERO_INCLUDE_D3D9
        if (renderType == RenderType::D3D9)
        {
            return d3d9::init(window);
        }
#endif

#if KIERO_INCLUDE_D3D10
        if (renderType == RenderType::D3D10)
        {
            return d3d10::init(window);
        }
#endif

#if KIERO_INCLUDE_D3D11
        if (renderType == RenderType::D3D11)
        {
            return d3d11::init(window);
        }
#endif

#if KIERO_INCLUDE_D3D12
        if (renderType == RenderType::D3D12)
        {
            return d3d12::init(window);
        }
#endif

        return Status::NotSupportedError;
    }

#if KIERO_INCLUDE_OPENGL
    if (renderType == RenderType::OpenGL)
    {
        return opengl::init();
    }
#endif

#if KIERO_INCLUDE_VULKAN
    if (renderType == RenderType::Vulkan)
    {
        return vulkan::init();
    }
#endif

    return Status::NotSupportedError;
}

kiero::Status kiero::init(RenderType renderType)
{
    if (g_renderType != RenderType::None)
    {
        return Status::AlreadyInitializedError;
    }

    if (renderType == RenderType::None)
    {
        return Status::Success;
    }

    if (renderType == RenderType::Auto)
    {
        if (::GetModuleHandle(KIERO_TEXT("d3d9.dll")) != nullptr)
        {
            renderType = RenderType::D3D9;
        }
        else if (::GetModuleHandle(KIERO_TEXT("d3d10.dll")) != nullptr)
        {
            renderType = RenderType::D3D10;
        }
        else if (::GetModuleHandle(KIERO_TEXT("d3d11.dll")) != nullptr)
        {
            renderType = RenderType::D3D11;
        }
        else if (::GetModuleHandle(KIERO_TEXT("d3d12.dll")) != nullptr)
        {
            renderType = RenderType::D3D12;
        }
        else if (::GetModuleHandle(KIERO_TEXT("opengl32.dll")) != nullptr)
        {
            renderType = RenderType::OpenGL;
        }
        else if (::GetModuleHandle(KIERO_TEXT("vulkan-1.dll")) != nullptr)
        {
            renderType = RenderType::Vulkan;
        }
        else
        {
            return Status::NotSupportedError;
        }
    }

    const auto status = initRenderType(renderType);
    if (status == Status::Success)
    {
#if KIERO_USE_MINHOOK
        MH_Initialize();
#endif
        g_renderType = renderType;
    }
    return status;
}

void kiero::shutdown()
{
    if (g_renderType != RenderType::None)
    {
#if KIERO_USE_MINHOOK
        MH_DisableHook(MH_ALL_HOOKS);
#endif

        ::free(g_methodsTable);
        g_methodsTable = nullptr;
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