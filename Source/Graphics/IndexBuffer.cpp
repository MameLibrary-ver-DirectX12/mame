#include "IndexBuffer.h"

#include "Graphics.h"

#include "../Other/Misc.h"

// ----- コンストラクタ -----
IndexBuffer::IndexBuffer(void* pData, UINT dataNum, UINT sizeOfStructure)
{
    HRESULT result = S_OK;

    D3D12_HEAP_PROPERTIES heapProp = {};
    heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;
    heapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
    heapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
    heapProp.CreationNodeMask = 1;
    heapProp.VisibleNodeMask = 1;

    D3D12_RESOURCE_DESC resourceDesc = {};
    resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
    resourceDesc.Alignment = 0;
    resourceDesc.Width = sizeOfStructure * dataNum;
    resourceDesc.Height = 1;
    resourceDesc.DepthOrArraySize = 1;
    resourceDesc.MipLevels = 1;
    resourceDesc.Format = DXGI_FORMAT_UNKNOWN;
    resourceDesc.SampleDesc.Count = 1;
    resourceDesc.SampleDesc.Quality = 0;
    resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
    resourceDesc.Flags = D3D12_RESOURCE_FLAG_NONE;

    result = Graphics::Instance().GetDevice()->CreateCommittedResource(
        &heapProp,
        D3D12_HEAP_FLAG_NONE,
        &resourceDesc,
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(buffer_.ReleaseAndGetAddressOf())
    );
    _ASSERT_EXPR(SUCCEEDED(result), HRTrace(result));

    bufferView_.BufferLocation = buffer_->GetGPUVirtualAddress();
    bufferView_.SizeInBytes = static_cast<UINT>(buffer_->GetDesc().Width);
    bufferView_.Format = DXGI_FORMAT_R32_UINT;

    void* map = nullptr;
    buffer_->Map(0, nullptr, &map);
    memcpy(map, pData, dataNum * sizeOfStructure);
    buffer_->Unmap(0, nullptr);
}
