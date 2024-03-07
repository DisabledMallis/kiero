#pragma once

#include <dxgi.h>
#include <d3d12.h>

#include <winrt/base.h>

namespace kiero::d3d12
{
    using CreateDXGIFactory_t = decltype(&::CreateDXGIFactory);
    using D3D12CreateDevice_t = decltype(&::D3D12CreateDevice);

    static Status init(HWND window)
    {
        HMODULE libDXGI;
        HMODULE libD3D12;
        if ((libDXGI = ::GetModuleHandle(KIERO_TEXT("dxgi.dll"))) == nullptr || (libD3D12 = ::GetModuleHandle(KIERO_TEXT("d3d12.dll"))) == nullptr)
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

        D3D12CreateDevice_t D3D12CreateDevice;
        if ((D3D12CreateDevice = reinterpret_cast<D3D12CreateDevice_t>(::GetProcAddress(libD3D12, "D3D12CreateDevice"))) == nullptr)
        {
            return Status::UnknownError;
        }

        winrt::com_ptr<ID3D12Device> device;
        if (D3D12CreateDevice(adapter, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(device.put())) < 0)
        {
            return Status::UnknownError;
        }

        D3D12_COMMAND_QUEUE_DESC queueDesc;
        queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
        queueDesc.Priority = 0;
        queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
        queueDesc.NodeMask = 0;

        winrt::com_ptr<ID3D12CommandQueue> commandQueue;
        if (device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(commandQueue.put())) < 0)
        {
            return Status::UnknownError;
        }

        winrt::com_ptr<ID3D12CommandAllocator> commandAllocator;
        if (device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(commandAllocator.put())) < 0)
        {
            return Status::UnknownError;
        }

        winrt::com_ptr<ID3D12GraphicsCommandList> commandList;
        if (device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, commandAllocator.get(), nullptr, IID_PPV_ARGS(commandList.put())) < 0)
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

        DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
        swapChainDesc.BufferDesc = bufferDesc;
        swapChainDesc.SampleDesc = sampleDesc;
        swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        swapChainDesc.BufferCount = 2;
        swapChainDesc.OutputWindow = window;
        swapChainDesc.Windowed = 1;
        swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
        swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

        winrt::com_ptr<IDXGISwapChain> swapChain;
        if (factory->CreateSwapChain(commandQueue.get(), &swapChainDesc, swapChain.put()) < 0)
        {
            return Status::UnknownError;
        }

        g_methodsTable = static_cast<uintptr_t*>(::calloc(150, sizeof(uintptr_t)));
        ::memcpy(g_methodsTable, *reinterpret_cast<uintptr_t**>(device.get()), 44 * sizeof(uintptr_t));
        ::memcpy(g_methodsTable + 44, *reinterpret_cast<uintptr_t**>(commandQueue.get()), 19 * sizeof(uintptr_t));
        ::memcpy(g_methodsTable + 44 + 19, *reinterpret_cast<uintptr_t**>(commandAllocator.get()), 9 * sizeof(uintptr_t));
        ::memcpy(g_methodsTable + 44 + 19 + 9, *reinterpret_cast<uintptr_t**>(commandList.get()), 60 * sizeof(uintptr_t));
        ::memcpy(g_methodsTable + 44 + 19 + 9 + 60, *reinterpret_cast<uintptr_t**>(swapChain.get()), 18 * sizeof(uintptr_t));

        return Status::Success;
    }
}
