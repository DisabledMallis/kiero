#pragma once

#include <dxgi.h>
#include <d3d12.h>

namespace kiero
{
    static Status init_d3d12(HWND window)
    {
        HMODULE libDXGI;
        HMODULE libD3D12;
        if ((libDXGI = ::GetModuleHandle(KIERO_TEXT("dxgi.dll"))) == NULL || (libD3D12 = ::GetModuleHandle(KIERO_TEXT("d3d12.dll"))) == NULL)
        {
            return Status::ModuleNotFoundError;
        }

        void* CreateDXGIFactory;
        if ((CreateDXGIFactory = ::GetProcAddress(libDXGI, "CreateDXGIFactory")) == NULL)
        {
            return Status::UnknownError;
        }

        IDXGIFactory* factory;
        if (((long(__stdcall*)(const IID&, void**))(CreateDXGIFactory))(__uuidof(IDXGIFactory), (void**)&factory) < 0)
        {
            return Status::UnknownError;
        }

        IDXGIAdapter* adapter;
        if (factory->EnumAdapters(0, &adapter) == DXGI_ERROR_NOT_FOUND)
        {
            return Status::UnknownError;
        }

        void* D3D12CreateDevice;
        if ((D3D12CreateDevice = ::GetProcAddress(libD3D12, "D3D12CreateDevice")) == NULL)
        {
            return Status::UnknownError;
        }

        ID3D12Device* device;
        if (((long(__stdcall*)(IUnknown*, D3D_FEATURE_LEVEL, const IID&, void**))(D3D12CreateDevice))(adapter, D3D_FEATURE_LEVEL_11_0, __uuidof(ID3D12Device), (void**)&device) < 0)
        {
            return Status::UnknownError;
        }

        D3D12_COMMAND_QUEUE_DESC queueDesc;
        queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
        queueDesc.Priority = 0;
        queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
        queueDesc.NodeMask = 0;

        ID3D12CommandQueue* commandQueue;
        if (device->CreateCommandQueue(&queueDesc, __uuidof(ID3D12CommandQueue), (void**)&commandQueue) < 0)
        {
            return Status::UnknownError;
        }

        ID3D12CommandAllocator* commandAllocator;
        if (device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, __uuidof(ID3D12CommandAllocator), (void**)&commandAllocator) < 0)
        {
            return Status::UnknownError;
        }

        ID3D12GraphicsCommandList* commandList;
        if (device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, commandAllocator, NULL, __uuidof(ID3D12GraphicsCommandList), (void**)&commandList) < 0)
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

        IDXGISwapChain* swapChain;
        if (factory->CreateSwapChain(commandQueue, &swapChainDesc, &swapChain) < 0)
        {
            return Status::UnknownError;
        }

        g_methodsTable = (uintptr_t*)::calloc(150, sizeof(uintptr_t));
        ::memcpy(g_methodsTable, *(uintptr_t**)device, 44 * sizeof(uintptr_t));
        ::memcpy(g_methodsTable + 44, *(uintptr_t**)commandQueue, 19 * sizeof(uintptr_t));
        ::memcpy(g_methodsTable + 44 + 19, *(uintptr_t**)commandAllocator, 9 * sizeof(uintptr_t));
        ::memcpy(g_methodsTable + 44 + 19 + 9, *(uintptr_t**)commandList, 60 * sizeof(uintptr_t));
        ::memcpy(g_methodsTable + 44 + 19 + 9 + 60, *(uintptr_t**)swapChain, 18 * sizeof(uintptr_t));

#if KIERO_USE_MINHOOK
    MH_Initialize();
#endif

        device->Release();
        device = NULL;

        commandQueue->Release();
        commandQueue = NULL;

        commandAllocator->Release();
        commandAllocator = NULL;

        commandList->Release();
        commandList = NULL;

        swapChain->Release();
        swapChain = NULL;

        g_renderType = RenderType::D3D12;

        return Status::Success;
    }
}
