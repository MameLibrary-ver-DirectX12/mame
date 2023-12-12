#pragma once

#include <d3d12.h>
#include <wrl.h>

class VertexBuffer
{
public:
    VertexBuffer(void* pData, UINT dataNum, UINT sizeOfStructure);
    ~VertexBuffer() {}

    D3D12_VERTEX_BUFFER_VIEW* GetVertexBufferView() { return &bufferView_; }

private:
    Microsoft::WRL::ComPtr<ID3D12Resource> buffer_;
    D3D12_VERTEX_BUFFER_VIEW bufferView_;
};

