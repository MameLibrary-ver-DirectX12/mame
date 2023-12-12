#include "Descriptor.h"
#include "../Other/Misc.h"

//============================================================================
// ディスクリプタ
//----------------------------------------------------------------------------

// 初期化
void Descriptor::Initialize(D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle, D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle, UINT index)
{
    cpuHandle_ = cpuHandle;
    gpuHandle_ = gpuHandle;
    index_ = index;
}

//============================================================================
// ディスクリプタヒープ操作オブジェクト
//----------------------------------------------------------------------------

// コンストラクタ
DescriptorHeap::DescriptorHeap(ID3D12Device* device, D3D12_DESCRIPTOR_HEAP_TYPE heapType, UINT maxDescriptorCount)
{
    HRESULT result = S_OK;

    // --- ディスクリプタヒープ毎の設定 ---
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

	// --- ディスクリプタヒープ生成 ---
	D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};
	heapDesc.Type = heapType;							// ディスクリプタヒープのタイプ
	heapDesc.NumDescriptors = maxDescriptorCount;		// 格納できるディスクリプタの数
	heapDesc.Flags = descriptorHeapFlags;				// ディスクリプタがさしているデータをシェーダーから参照したいかどうか
	heapDesc.NodeMask = 0;								// アダプタを１つしか使わない場合は０を設定

	result = device->CreateDescriptorHeap(
		&heapDesc,										// ディスクリプタヒープの設定
		IID_PPV_ARGS(descriptorHeap_.GetAddressOf())	// 格納するポインタのアドレス
	);
	_ASSERT_EXPR(SUCCEEDED(result), HRTrace(result));

	descriptorHeap_->SetName(name);

	// --- ディスクリプタをあらかじめ最大数確保しておく ---
	descriptors_.resize(maxDescriptorCount);

	// --- ディスクリプタ初期化 ---
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

// デストラクタ
DescriptorHeap::~DescriptorHeap()
{
	//std::lock_guard<std::mutex> lock(mutex_);
}

// ディスクリプタを受け取る
Descriptor* DescriptorHeap::PopDescriptor()
{
	if (descriptorStackIndex_ < 0) return nullptr;

//	std::lock_guard<std::mutex> lock(mutex_);

	Descriptor* descriptor = descriptorStack_[descriptorStackIndex_];
	descriptorStack_[descriptorStackIndex_] = nullptr;
	--descriptorStackIndex_;

	return descriptor;
}

// ディスクリプタを返却する
void DescriptorHeap::PushDescriptor(Descriptor* descriptor)
{
	return;
	if (descriptorStackIndex_ >= descriptorStack_.size() - 1) return;

	//std::lock_guard<std::mutex> lock(mutex_);

	++descriptorStackIndex_;
	descriptorStack_[descriptorStackIndex_] = descriptor;
}
