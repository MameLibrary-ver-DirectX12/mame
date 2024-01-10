#include "FrameBuffer.h"
#include "../Other/Misc.h"
#include "Graphics.h"

// --- コンストラクタ ---
FrameBuffer::FrameBuffer()
{
    HRESULT result = S_OK;

    // --- ヒーププロパティの設定 ---
    CD3DX12_HEAP_PROPERTIES heapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);

    // --- リソースの設定 ---
    D3D12_RESOURCE_DESC resourceDesc = {};
    resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
    resourceDesc.Alignment = 0;
    resourceDesc.Width = static_cast<UINT>(Graphics::Instance().GetScreenWidth());
    resourceDesc.Height = static_cast<UINT>(Graphics::Instance().GetScrennHeight());
    resourceDesc.DepthOrArraySize = 1;
    resourceDesc.MipLevels = 1;
    resourceDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    resourceDesc.SampleDesc.Count = 1;
    resourceDesc.SampleDesc.Quality = 0;
    resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
    resourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;
    
    result = Graphics::Instance().GetDevice()->CreateCommittedResource(
        &heapProp,
        D3D12_HEAP_FLAG_NONE,
        &resourceDesc,
        D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
        nullptr,
        IID_PPV_ARGS(resource_.ReleaseAndGetAddressOf())
    );
    _ASSERT_EXPR(SUCCEEDED(result), HRTrace(result));

    // DepthStencil
    D3D12_CLEAR_VALUE clearValue = CD3DX12_CLEAR_VALUE(DXGI_FORMAT_D24_UNORM_S8_UINT, 1.0f, 0);
    resourceDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    resourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;
    result = Graphics::Instance().GetDevice()->CreateCommittedResource(
        &heapProp,
        D3D12_HEAP_FLAG_NONE,
        &resourceDesc,
        D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
        &clearValue,
        IID_PPV_ARGS(depthResource_.ReleaseAndGetAddressOf())
    );
    _ASSERT_EXPR(SUCCEEDED(result), HRTrace(result));


    // RTV用ヒープを作る
    auto heapDesc = Graphics::Instance().GetRtvDescriptorHeap()->GetD3DDescriptorHeap()->GetDesc();
    heapDesc.NumDescriptors = 1;
    result = Graphics::Instance().GetDevice()->CreateDescriptorHeap(
        &heapDesc,
        IID_PPV_ARGS(RTVHeap_.ReleaseAndGetAddressOf())
    );

    D3D12_RENDER_TARGET_VIEW_DESC rtvDesc = {};
    rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
    rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    

    // レンダーターゲットビューを(RTV)作る
    Graphics::Instance().GetDevice()->CreateRenderTargetView(
        resource_.Get(),
        &rtvDesc,
        RTVHeap_->GetCPUDescriptorHandleForHeapStart()
    );

    // SRV用ヒープを作る
    heapDesc.NumDescriptors = 1;
    heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
    heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;

    result = Graphics::Instance().GetDevice()->CreateDescriptorHeap(
        &heapDesc,
        IID_PPV_ARGS(SRVHeap_.ReleaseAndGetAddressOf())
    );
    _ASSERT_EXPR(SUCCEEDED(result), HRTrace(result));

    D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
    srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Format = rtvDesc.Format;
    srvDesc.Texture2D.MipLevels = 1;
    srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;

    // シェーダーリソースビュー(SRV)を作る
    Graphics::Instance().GetDevice()->CreateShaderResourceView(
        resource_.Get(),
        &srvDesc,
        SRVHeap_->GetCPUDescriptorHandleForHeapStart()
    );

    // DSV
    DSVHeap_ = std::make_unique<DescriptorHeap>(Graphics::Instance().GetDevice(), D3D12_DESCRIPTOR_HEAP_TYPE_DSV, 10);

}

void FrameBuffer::Activate(ID3D12GraphicsCommandList* commandList)
{
    auto rtvHeapPointer = RTVHeap_->GetCPUDescriptorHandleForHeapStart();
    //auto dsvHeapPointer = Graphics::Instance().GetDsvDescriptorHeap()->GetD3DDescriptorHeap()->GetCPUDescriptorHandleForHeapStart();
    auto dsvHeapPointer = DSVHeap_->GetD3DDescriptorHeap()->GetCPUDescriptorHandleForHeapStart();

    commandList->OMSetRenderTargets(1, &rtvHeapPointer, false, &dsvHeapPointer);

    float clearColor[4] = { 0.5f, 0.5f, 0.5f, 1.0f };
    commandList->ClearRenderTargetView(rtvHeapPointer, clearColor, 0, nullptr);
    commandList->ClearDepthStencilView(dsvHeapPointer, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

#if 0
    auto barrier = CD3DX12_RESOURCE_BARRIER::Transition(
        resource_.Get(), D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
        D3D12_RESOURCE_STATE_RENDER_TARGET);

    commandList->ResourceBarrier(1, &barrier);
#endif
}

void FrameBuffer::Deactivate(ID3D12GraphicsCommandList* commandList)
{
#if 0
    auto barrier = CD3DX12_RESOURCE_BARRIER::Transition(
        resource_.Get(), D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
        D3D12_RESOURCE_STATE_RENDER_TARGET);
#else
    auto barrier = CD3DX12_RESOURCE_BARRIER::Transition(
        resource_.Get(), D3D12_RESOURCE_STATE_RENDER_TARGET,
        D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
#endif

    commandList->ResourceBarrier(1, &barrier);
}
