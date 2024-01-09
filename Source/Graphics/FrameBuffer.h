#pragma once
#include <d3d12.h>
#include <wrl.h>
#include <cstdint>

class FrameBuffer
{
public:
    FrameBuffer();
    virtual ~FrameBuffer() = default;

    void Activate(ID3D12GraphicsCommandList* commandList);
    void Deactivate(ID3D12GraphicsCommandList* commandList);

private:
    Microsoft::WRL::ComPtr<ID3D12Resource> resource_;
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> RTVHeap_; // レンダーターゲット用
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> SRVHeap_; // テクスチャ用
};

