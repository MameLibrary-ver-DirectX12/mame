#pragma once
#include <d3d12.h>
#include <wrl.h>
#include <cstdint>

#include "Descriptor.h"

class FrameBuffer
{
public:
    FrameBuffer();
    virtual ~FrameBuffer() = default;

    void Activate(ID3D12GraphicsCommandList* commandList);
    void Deactivate(ID3D12GraphicsCommandList* commandList);

    D3D12_GPU_DESCRIPTOR_HANDLE GetGpuHandle() { return srvDescriptor_->GetGpuHandle(); }

private:
    Microsoft::WRL::ComPtr<ID3D12Resource> resource_;
    Microsoft::WRL::ComPtr<ID3D12Resource> depthResource_;
    Descriptor* rtvDescriptor_;
    Descriptor* srvDescriptor_;
    Descriptor* dsvDescriptor_;
};

