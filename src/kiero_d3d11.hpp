#pragma once

#include <dxgi.h>
#include <d3d11.h>

namespace kiero::d3d11
{
    static Status init(HWND window)
    {
        HMODULE libD3D11;
        if ((libD3D11 = ::GetModuleHandle(KIERO_TEXT("d3d11.dll"))) == NULL)
        {
            return Status::ModuleNotFoundError;
        }

        void* D3D11CreateDeviceAndSwapChain;
        if ((D3D11CreateDeviceAndSwapChain = ::GetProcAddress(libD3D11, "D3D11CreateDeviceAndSwapChain")) == NULL)
        {
            return Status::UnknownError;
        }

        D3D_FEATURE_LEVEL featureLevel;
        const D3D_FEATURE_LEVEL featureLevels[] = { D3D_FEATURE_LEVEL_10_1, D3D_FEATURE_LEVEL_11_0 };

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
        ID3D11Device* device;
        ID3D11DeviceContext* context;

        if (((long(__stdcall*)(
            IDXGIAdapter*,
            D3D_DRIVER_TYPE,
            HMODULE,
            UINT,
            const D3D_FEATURE_LEVEL*,
            UINT,
            UINT,
            const DXGI_SWAP_CHAIN_DESC*,
            IDXGISwapChain**,
            ID3D11Device**,
            D3D_FEATURE_LEVEL*,
            ID3D11DeviceContext**))(D3D11CreateDeviceAndSwapChain))(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, featureLevels, 2, D3D11_SDK_VERSION, &swapChainDesc, &swapChain, &device, &featureLevel, &context) < 0)
        {
            return Status::UnknownError;
        }

        g_methodsTable = (uintptr_t*)::calloc(205, sizeof(uintptr_t));
        ::memcpy(g_methodsTable, *(uintptr_t**)swapChain, 18 * sizeof(uintptr_t));
        ::memcpy(g_methodsTable + 18, *(uintptr_t**)device, 43 * sizeof(uintptr_t));
        ::memcpy(g_methodsTable + 18 + 43, *(uintptr_t**)context, 144 * sizeof(uintptr_t));

#if KIERO_USE_MINHOOK
        MH_Initialize();
#endif

        swapChain->Release();
        swapChain = NULL;

        device->Release();
        device = NULL;

        context->Release();
        context = NULL;

        g_renderType = RenderType::D3D11;

        return Status::Success;
    }
}
