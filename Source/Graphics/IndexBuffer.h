#pragma once

#include <d3d12.h>
#include <wrl.h>

class IndexBuffer
{
public:
    IndexBuffer(void* pData, UINT dataNum, UINT sizeOfStructure);
    ~IndexBuffer();

    D3D12_INDEX_BUFFER_VIEW* GetIndexBufferView() { return &bufferView_; }

private:
    Microsoft::WRL::ComPtr<ID3D12Resource> buffer_;
    D3D12_INDEX_BUFFER_VIEW bufferView_;
};

