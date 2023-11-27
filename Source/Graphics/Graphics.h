#pragma once
#include <Windows.h>
#include <d3dx12.h>
#include <dxgi1_6.h>
#include <DirectXMath.h>

class Graphics
{
private:
    Graphics();
    ~Graphics();


private:
    IDXGIFactory6* dxgiFactory_             = nullptr;
    ID3D12Device* device_                   = nullptr;
    IDXGISwapChain4* swapChain_             = nullptr;

    ID3D12CommandAllocator* cmdAllocator_   = nullptr;
    ID3D12GraphicsCommandList* cmdList_     = nullptr;

    ID3D12CommandQueue* cmdQueue_           = nullptr;
};