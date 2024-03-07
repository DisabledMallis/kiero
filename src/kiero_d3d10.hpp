#pragma once

#include <dxgi.h>
#include <d3d10_1.h>
#include <d3d10.h>

namespace kiero::d3d10
{
    static Status init(HWND window)
    {
        HMODULE libDXGI;
        HMODULE libD3D10;
        if ((libDXGI = ::GetModuleHandle(KIERO_TEXT("dxgi.dll"))) == NULL || (libD3D10 = ::GetModuleHandle(KIERO_TEXT("d3d10.dll"))) == NULL)
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

        void* D3D10CreateDeviceAndSwapChain;
        if ((D3D10CreateDeviceAndSwapChain = ::GetProcAddress(libD3D10, "D3D10CreateDeviceAndSwapChain")) == NULL)
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

        IDXGISwapChain* swapChain;
        ID3D10Device* device;

        if (((long(__stdcall*)(
            IDXGIAdapter*,
            D3D10_DRIVER_TYPE,
            HMODULE,
            UINT,
            UINT,
            DXGI_SWAP_CHAIN_DESC*,
            IDXGISwapChain**,
            ID3D10Device**))(D3D10CreateDeviceAndSwapChain))(adapter, D3D10_DRIVER_TYPE_HARDWARE, NULL, 0, D3D10_SDK_VERSION, &swapChainDesc, &swapChain, &device) < 0)
        {
            return Status::UnknownError;
        }

        g_methodsTable = (uintptr_t*)::calloc(116, sizeof(uintptr_t));
        ::memcpy(g_methodsTable, *(uintptr_t**)swapChain, 18 * sizeof(uintptr_t));
        ::memcpy(g_methodsTable + 18, *(uintptr_t**)device, 98 * sizeof(uintptr_t));

#if KIERO_USE_MINHOOK
        MH_Initialize();
#endif

        swapChain->Release();
        swapChain = NULL;

        device->Release();
        device = NULL;

        g_renderType = RenderType::D3D10;

        return Status::Success;
    }
}
