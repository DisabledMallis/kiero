#pragma once

#include <d3d9.h>

namespace kiero::d3d9
{
    static Status init(HWND window)
    {
        HMODULE libD3D9;
        if ((libD3D9 = ::GetModuleHandle(KIERO_TEXT("d3d9.dll"))) == NULL)
        {
            return Status::ModuleNotFoundError;
        }

        void* Direct3DCreate9;
        if ((Direct3DCreate9 = ::GetProcAddress(libD3D9, "Direct3DCreate9")) == NULL)
        {
            return Status::UnknownError;
        }

        LPDIRECT3D9 direct3D9;
        if ((direct3D9 = ((LPDIRECT3D9(__stdcall*)(uint32_t))(Direct3DCreate9))(D3D_SDK_VERSION)) == NULL)
        {
            return Status::UnknownError;
        }

        D3DPRESENT_PARAMETERS params;
        params.BackBufferWidth = 0;
        params.BackBufferHeight = 0;
        params.BackBufferFormat = D3DFMT_UNKNOWN;
        params.BackBufferCount = 0;
        params.MultiSampleType = D3DMULTISAMPLE_NONE;
        params.MultiSampleQuality = NULL;
        params.SwapEffect = D3DSWAPEFFECT_DISCARD;
        params.hDeviceWindow = window;
        params.Windowed = 1;
        params.EnableAutoDepthStencil = 0;
        params.AutoDepthStencilFormat = D3DFMT_UNKNOWN;
        params.Flags = NULL;
        params.FullScreen_RefreshRateInHz = 0;
        params.PresentationInterval = 0;

        LPDIRECT3DDEVICE9 device;
        if (direct3D9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_NULLREF, window, D3DCREATE_SOFTWARE_VERTEXPROCESSING | D3DCREATE_DISABLE_DRIVER_MANAGEMENT, &params, &device) < 0)
        {
            direct3D9->Release();
            return Status::UnknownError;
        }

        g_methodsTable = (uintptr_t*)::calloc(119, sizeof(uintptr_t));
        ::memcpy(g_methodsTable, *(uintptr_t**)device, 119 * sizeof(uintptr_t));

#if KIERO_USE_MINHOOK
        MH_Initialize();
#endif

        device->Release();
        device = NULL;

        direct3D9->Release();
        direct3D9 = NULL;

        g_renderType = RenderType::D3D9;

        return Status::Success;
    }
}
