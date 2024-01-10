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

private:
    Microsoft::WRL::ComPtr<ID3D12Resource> resource_;
    Microsoft::WRL::ComPtr<ID3D12Resource> depthResource_;
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> RTVHeap_; // �����_�[�^�[�Q�b�g�p
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> SRVHeap_; // �e�N�X�`���p

    std::unique_ptr<DescriptorHeap> DSVHeap_;
};

