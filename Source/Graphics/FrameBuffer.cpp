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
    

    float color[4] = { 0.5f, 0.5f, 0.5f, 1.0f };
    D3D12_CLEAR_VALUE clearValue = CD3DX12_CLEAR_VALUE(DXGI_FORMAT_R8G8B8A8_UNORM, color);

    result = Graphics::Instance().GetDevice()->CreateCommittedResource(
        &heapProp,
        D3D12_HEAP_FLAG_NONE,
        &resourceDesc,
        D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
        &clearValue,
        IID_PPV_ARGS(resource_.ReleaseAndGetAddressOf())
    );
    _ASSERT_EXPR(SUCCEEDED(result), HRTrace(result));

    // DepthStencil
   clearValue = CD3DX12_CLEAR_VALUE(DXGI_FORMAT_D32_FLOAT, 1.0f, 0);
    resourceDesc.Format = DXGI_FORMAT_D32_FLOAT;
    resourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;
    result = Graphics::Instance().GetDevice()->CreateCommittedResource(
        &heapProp,
        D3D12_HEAP_FLAG_NONE,
        &resourceDesc,
        D3D12_RESOURCE_STATE_DEPTH_WRITE,
        &clearValue,
        IID_PPV_ARGS(depthResource_.ReleaseAndGetAddressOf())
    );
    _ASSERT_EXPR(SUCCEEDED(result), HRTrace(result));




    D3D12_RENDER_TARGET_VIEW_DESC rtvDesc = {};
    rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
    rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    

    // レンダーターゲットビューを(RTV)作る
    rtvDescriptor_ = Graphics::Instance().GetRtvDescriptorHeap()->PopDescriptor();
    Graphics::Instance().GetDevice()->CreateRenderTargetView(
        resource_.Get(),
        &rtvDesc,
        rtvDescriptor_->GetCpuHandle()
    );

    // SRV用ヒープを作る
    srvDescriptor_ = Graphics::Instance().GetSrvCbvUavDescriptorHeap()->PopDescriptor();

    D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
    srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Format = rtvDesc.Format;
    srvDesc.Texture2D.MipLevels = 1;
    srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;

    // シェーダーリソースビュー(SRV)を作る
    Graphics::Instance().GetDevice()->CreateShaderResourceView(
        resource_.Get(),
        &srvDesc,
        srvDescriptor_->GetCpuHandle()
    );

    // --- ディスクリプタ取得 ---
    dsvDescriptor_ = Graphics::Instance().GetDsvDescriptorHeap()->PopDescriptor();

    // --- デプスステンシルビューの設定 ---
    D3D12_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc = {};
    depthStencilViewDesc.Format = DXGI_FORMAT_D32_FLOAT;
    //depthStencilViewDesc.Format = DXGI_FORMAT_D32_FLOAT;
    depthStencilViewDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
    depthStencilViewDesc.Flags = D3D12_DSV_FLAG_NONE;

    // --- デプスステンシルビュー生成 ---
    Graphics::Instance().GetDevice()->CreateDepthStencilView(
        depthResource_.Get(),
        &depthStencilViewDesc,
        dsvDescriptor_->GetCpuHandle()
    );
}

void FrameBuffer::Activate(ID3D12GraphicsCommandList* commandList)
{
#if 1
    auto barrier = CD3DX12_RESOURCE_BARRIER::Transition(
        resource_.Get(), D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
        D3D12_RESOURCE_STATE_RENDER_TARGET);

    commandList->ResourceBarrier(1, &barrier);
#endif
    D3D12_CPU_DESCRIPTOR_HANDLE rtvHeapPointer = rtvDescriptor_->GetCpuHandle();
    D3D12_CPU_DESCRIPTOR_HANDLE dsvHeapPointer = dsvDescriptor_->GetCpuHandle();

    commandList->OMSetRenderTargets(1, &rtvHeapPointer, false, &dsvHeapPointer);

    float clearColor[4] = { 0.5f, 0.5f, 0.5f, 1.0f };
    commandList->ClearRenderTargetView(rtvHeapPointer, clearColor, 0, nullptr);
    commandList->ClearDepthStencilView(dsvHeapPointer, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

#if 0
    // --- ビューポートの設定 ---
    D3D12_VIEWPORT viewport;
    viewport.TopLeftX = 0;
    viewport.TopLeftY = 0;
    viewport.Width = screenWidth_;
    viewport.Height = screenHeight_;
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;

    frameResource.commandList_->RSSetViewports(1, &viewport);

    // --- シザーの設定 ---
    D3D12_RECT scissorRect;
    scissorRect.left = 0;
    scissorRect.top = 0;
    scissorRect.right = static_cast<LONG>(screenWidth_);
    scissorRect.bottom = static_cast<LONG>(screenHeight_);

    frameResource.commandList_->RSSetScissorRects(1, &scissorRect);
#endif

}

void FrameBuffer::Deactivate(ID3D12GraphicsCommandList* commandList)
{
    auto barrier = CD3DX12_RESOURCE_BARRIER::Transition(
        resource_.Get(), D3D12_RESOURCE_STATE_RENDER_TARGET,
        D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);

    commandList->ResourceBarrier(1, &barrier);
}
