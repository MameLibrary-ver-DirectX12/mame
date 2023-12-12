#pragma once

#include <vector>
#include <mutex>
#include <d3d12.h>
#include <wrl.h>

//============================================================================
// ディスクリプタ
//----------------------------------------------------------------------------
class Descriptor
{
public:
    // 初期化
    void Initialize(D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle, D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle, UINT index);

    // GPUハンドル取得
    D3D12_CPU_DESCRIPTOR_HANDLE GetCpuHandle() const { return cpuHandle_; }

    // GPUハンドル取得
    D3D12_GPU_DESCRIPTOR_HANDLE GetGpuHandle() const { return gpuHandle_; }

    // ディスクリプタヒープ内のインデックス取得
    UINT GetIndex() const { return index_; }

private:
    D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle_ = { 0 };
    D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle_ = { 0 };
    UINT                        index_ = 0;
};

//============================================================================
// ディスクリプタヒープ
//----------------------------------------------------------------------------
class DescriptorHeap
{
public:
    DescriptorHeap(ID3D12Device* device, D3D12_DESCRIPTOR_HEAP_TYPE heapType, UINT maxDescriptorCount);
    ~DescriptorHeap();

public:
    // D3Dディスクリプタヒープ取得
    ID3D12DescriptorHeap* GetD3DDescriptorHeap() const { return descriptorHeap_.Get(); }

    // ディスクリプタを受け取る
    Descriptor* PopDescriptor();

    // ディスクリプタを返却する
    void PushDescriptor(Descriptor* descriptor);

private:
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>    descriptorHeap_;
  //  std::mutex                                      mutex_;
    std::vector<Descriptor>                         descriptors_;
    std::vector<Descriptor*>                        descriptorStack_;
    int                                             descriptorStackIndex_ = 0;
};