#pragma once

#include <dxgi.h>
#include <d3d10_1.h>
#include <d3d10.h>

#include <winrt/base.h>

namespace kiero::d3d10
{
    using CreateDXGIFactory_t = decltype(&::CreateDXGIFactory);
    using D3D10CreateDeviceAndSwapChain_t = decltype(&::D3D10CreateDeviceAndSwapChain);

    static Status init(HWND window)
    {
        HMODULE libDXGI;
        HMODULE libD3D10;
        if ((libDXGI = ::GetModuleHandle(KIERO_TEXT("dxgi.dll"))) == nullptr || (libD3D10 = ::GetModuleHandle(KIERO_TEXT("d3d10.dll"))) == nullptr)
        {
            return Status::ModuleNotFoundError;
        }

        CreateDXGIFactory_t CreateDXGIFactory;
        if ((CreateDXGIFactory = reinterpret_cast<CreateDXGIFactory_t>(::GetProcAddress(libDXGI, "CreateDXGIFactory"))) == nullptr)
        {
            return Status::UnknownError;
        }

        winrt::com_ptr<IDXGIFactory> factory;
        if (CreateDXGIFactory(IID_PPV_ARGS(factory.put())) < 0)
        {
            return Status::UnknownError;
        }

        IDXGIAdapter* adapter;
        if (factory->EnumAdapters(0, &adapter) == DXGI_ERROR_NOT_FOUND)
        {
            return Status::UnknownError;
        }

        D3D10CreateDeviceAndSwapChain_t D3D10CreateDeviceAndSwapChain;
        if ((D3D10CreateDeviceAndSwapChain = reinterpret_cast<D3D10CreateDeviceAndSwapChain_t>(::GetProcAddress(libD3D10, "D3D10CreateDeviceAndSwapChain"))) == nullptr)
        {
            return Status::UnknownError;
        }

        DXGI_RATIONAL refreshRate;
        refreshRate.Numerator = 60;
        refreshRate.Denominator = 1;

        DXGI_MODE_DESC bufferDesc;
        bufferDesc.Width = 100;
        bufferDesc.Height = 100;
        bufferDesc.RefreshRate = refreshRate;
        bufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        bufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
        bufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

        DXGI_SAMPLE_DESC sampleDesc;
        sampleDesc.Count = 1;
        sampleDesc.Quality = 0;

        DXGI_SWAP_CHAIN_DESC swapChainDesc;
        swapChainDesc.BufferDesc = bufferDesc;
        swapChainDesc.SampleDesc = sampleDesc;
        swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        swapChainDesc.BufferCount = 1;
        swapChainDesc.OutputWindow = window;
        swapChainDesc.Windowed = 1;
        swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
        swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

        winrt::com_ptr<IDXGISwapChain> swapChain;
        winrt::com_ptr<ID3D10Device> device;

        if (D3D10CreateDeviceAndSwapChain(adapter, D3D10_DRIVER_TYPE_HARDWARE, nullptr, 0, D3D10_SDK_VERSION, &swapChainDesc, swapChain.put(), device.put()) < 0)
        {
            return Status::UnknownError;
        }

        g_methodsTable = static_cast<uintptr_t*>(::calloc(116, sizeof(uintptr_t)));
        ::memcpy(g_methodsTable, *reinterpret_cast<uintptr_t**>(swapChain.get()), 18 * sizeof(uintptr_t));
        ::memcpy(g_methodsTable + 18, *reinterpret_cast<uintptr_t**>(device.get()), 98 * sizeof(uintptr_t));

#if KIERO_USE_MINHOOK
        MH_Initialize();
#endif

        g_renderType = RenderType::D3D10;

        return Status::Success;
    }
}
