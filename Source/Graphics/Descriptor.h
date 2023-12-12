#pragma once

#include <vector>
#include <mutex>
#include <d3d12.h>
#include <wrl.h>

//============================================================================
// �f�B�X�N���v�^
//----------------------------------------------------------------------------
class Descriptor
{
public:
    // ������
    void Initialize(D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle, D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle, UINT index);

    // GPU�n���h���擾
    D3D12_CPU_DESCRIPTOR_HANDLE GetCpuHandle() const { return cpuHandle_; }

    // GPU�n���h���擾
    D3D12_GPU_DESCRIPTOR_HANDLE GetGpuHandle() const { return gpuHandle_; }

    // �f�B�X�N���v�^�q�[�v���̃C���f�b�N�X�擾
    UINT GetIndex() const { return index_; }

private:
    D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle_ = { 0 };
    D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle_ = { 0 };
    UINT                        index_ = 0;
};

//============================================================================
// �f�B�X�N���v�^�q�[�v
//----------------------------------------------------------------------------
class DescriptorHeap
{
public:
    DescriptorHeap(ID3D12Device* device, D3D12_DESCRIPTOR_HEAP_TYPE heapType, UINT maxDescriptorCount);
    ~DescriptorHeap();

public:
    // D3D�f�B�X�N���v�^�q�[�v�擾
    ID3D12DescriptorHeap* GetD3DDescriptorHeap() const { return descriptorHeap_.Get(); }

    // �f�B�X�N���v�^���󂯎��
    Descriptor* PopDescriptor();

    // �f�B�X�N���v�^��ԋp����
    void PushDescriptor(Descriptor* descriptor);

private:
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>    descriptorHeap_;
  //  std::mutex                                      mutex_;
    std::vector<Descriptor>                         descriptors_;
    std::vector<Descriptor*>                        descriptorStack_;
    int                                             descriptorStackIndex_ = 0;
};