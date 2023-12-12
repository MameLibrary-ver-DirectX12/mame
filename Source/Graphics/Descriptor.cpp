#include "Descriptor.h"
#include "../Other/Misc.h"

//============================================================================
// �f�B�X�N���v�^
//----------------------------------------------------------------------------

// ������
void Descriptor::Initialize(D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle, D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle, UINT index)
{
    cpuHandle_ = cpuHandle;
    gpuHandle_ = gpuHandle;
    index_ = index;
}

//============================================================================
// �f�B�X�N���v�^�q�[�v����I�u�W�F�N�g
//----------------------------------------------------------------------------

// �R���X�g���N�^
DescriptorHeap::DescriptorHeap(ID3D12Device* device, D3D12_DESCRIPTOR_HEAP_TYPE heapType, UINT maxDescriptorCount)
{
    HRESULT result = S_OK;

    // --- �f�B�X�N���v�^�q�[�v���̐ݒ� ---
    LPCWSTR name = L"";
    D3D12_DESCRIPTOR_HEAP_FLAGS descriptorHeapFlags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

    switch (heapType)
    {
	case D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV:
		name = L"CBV_SRV_UAV_DescriptorHeap";
		descriptorHeapFlags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
		break;
	case D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER:
		name = L"Sampler_DescriptorHeap";
		descriptorHeapFlags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
		break;
	case D3D12_DESCRIPTOR_HEAP_TYPE_RTV:
		name = L"RTV_DescriptorHeap";
		break;
	case D3D12_DESCRIPTOR_HEAP_TYPE_DSV:
		name = L"DSV_DescriptorHeap";
		break;
	}

	// --- �f�B�X�N���v�^�q�[�v���� ---
	D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};
	heapDesc.Type = heapType;							// �f�B�X�N���v�^�q�[�v�̃^�C�v
	heapDesc.NumDescriptors = maxDescriptorCount;		// �i�[�ł���f�B�X�N���v�^�̐�
	heapDesc.Flags = descriptorHeapFlags;				// �f�B�X�N���v�^�������Ă���f�[�^���V�F�[�_�[����Q�Ƃ��������ǂ���
	heapDesc.NodeMask = 0;								// �A�_�v�^���P�����g��Ȃ��ꍇ�͂O��ݒ�

	result = device->CreateDescriptorHeap(
		&heapDesc,										// �f�B�X�N���v�^�q�[�v�̐ݒ�
		IID_PPV_ARGS(descriptorHeap_.GetAddressOf())	// �i�[����|�C���^�̃A�h���X
	);
	_ASSERT_EXPR(SUCCEEDED(result), HRTrace(result));

	descriptorHeap_->SetName(name);

	// --- �f�B�X�N���v�^�����炩���ߍő吔�m�ۂ��Ă��� ---
	descriptors_.resize(maxDescriptorCount);

	// --- �f�B�X�N���v�^������ ---
	UINT descriptorSize = device->GetDescriptorHandleIncrementSize(heapType);
	D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle = descriptorHeap_->GetCPUDescriptorHandleForHeapStart();
	D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle = descriptorHeap_->GetGPUDescriptorHandleForHeapStart();

	for (UINT index = 0; index < maxDescriptorCount; ++index)
	{
		Descriptor* descriptor = &descriptors_.at(index);

		descriptor->Initialize(cpuHandle, gpuHandle, index);

		descriptorStack_.emplace_back(descriptor);

		cpuHandle.ptr += descriptorSize;
		gpuHandle.ptr += descriptorSize;
	}

	descriptorStackIndex_ = static_cast<int>(maxDescriptorCount - 1);
}

// �f�X�g���N�^
DescriptorHeap::~DescriptorHeap()
{
	//std::lock_guard<std::mutex> lock(mutex_);
}

// �f�B�X�N���v�^���󂯎��
Descriptor* DescriptorHeap::PopDescriptor()
{
	if (descriptorStackIndex_ < 0) return nullptr;

//	std::lock_guard<std::mutex> lock(mutex_);

	Descriptor* descriptor = descriptorStack_[descriptorStackIndex_];
	descriptorStack_[descriptorStackIndex_] = nullptr;
	--descriptorStackIndex_;

	return descriptor;
}

// �f�B�X�N���v�^��ԋp����
void DescriptorHeap::PushDescriptor(Descriptor* descriptor)
{
	return;
	if (descriptorStackIndex_ >= descriptorStack_.size() - 1) return;

	//std::lock_guard<std::mutex> lock(mutex_);

	++descriptorStackIndex_;
	descriptorStack_[descriptorStackIndex_] = descriptor;
}
