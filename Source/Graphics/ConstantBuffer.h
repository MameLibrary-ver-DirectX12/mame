#pragma once

#include <d3d12.h>
#include <wrl.h>

#include "Descriptor.h"

class ConstantBuffer
{
public:
    ConstantBuffer(UINT sizeOfStructure);
    ~ConstantBuffer();

    ID3D12Resource* GetBufferObject() { return buffer_.Get(); }
    Descriptor* GetDescriptor() { return descriptor_; }
    void UpdateSubresource(void* pData);

private:
    Microsoft::WRL::ComPtr<ID3D12Resource> buffer_;
    void* pData_;
    Descriptor* descriptor_;
    size_t sizeOfStructure_;
};