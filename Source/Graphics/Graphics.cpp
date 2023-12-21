#include "Graphics.h"

#include <WICTextureLoader.h>

#include <filesystem>

#include "../Core/Application.h"
#include "../Other/Misc.h"

Graphics* Graphics::instance_ = nullptr;

// �R���X�g���N�^
Graphics::Graphics(const HWND& hwnd, const UINT& bufferCount)
{
    // --- �C���X�^���X�ݒ� ---
    _ASSERT_EXPR(instance_ == nullptr, "already instantiated");
    instance_ = this;

    // --- ��ʃT�C�Y���擾 ---
    RECT rc;
    GetClientRect(hwnd, &rc);
    UINT screenWidth = rc.right - rc.left;
    UINT screenHeight = rc.bottom - rc.top;

    screenWidth_ = static_cast<float>(screenWidth);
    screenHeight_ = static_cast<float>(screenHeight);

    bufferCount_ = bufferCount;

    // --- DXGI �܂�� ---
    CreateDXGIDevice();
    
    // --- �O���t�B�b�N�X�R�}���h�L���[�̐��� ---
    CreateGraphicsCommandQueue();

    // --- �X���b�v�`�F�[���̍쐬 ---
    CreateSwapChain(hwnd, bufferCount);

    // --- �f�B�X�N���v�^�q�[�v�̐��� ---
    renderTargetViewDescriptorHeap_ = std::make_unique<DescriptorHeap>(device_.Get(), D3D12_DESCRIPTOR_HEAP_TYPE_RTV, 10);
    depthStencilViewDescriptorHeap_ = std::make_unique<DescriptorHeap>(device_.Get(), D3D12_DESCRIPTOR_HEAP_TYPE_DSV, 10);
    srvCbvUavDescriptorHeap_ = std::make_unique<DescriptorHeap>(device_.Get(), D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, 10000);
    samplerDescriptorHeap_ = std::make_unique<DescriptorHeap>(device_.Get(), D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER, 100);

    // --- �t���[�����\�[�X ---
    CreateFrameResource(bufferCount);

    // --- ���\�[�X�p�R�}���h ---
    CreateResourceCommand();

    // ---- ImGui�����_�� ---
    imguiRenderer_ = std::make_unique<ImGuiRenderer>(hwnd);

    // --- Shader ---
    shader_ = std::make_unique<Shader>();
}

// �f�X�g���N�^
Graphics::~Graphics()
{
    CommandQueue* commandQueues[] =
    {
        &graphicsQueue_,
        &resourceQueue_,
    };

    for (CommandQueue* commandQueue : commandQueues)
    {
        WaitIdle(*commandQueue);

        if (commandQueue->fenceEvent_ != NULL)
        {
            CloseHandle(commandQueue->fenceEvent_);
        }
    }

    for (FrameResource& frameResource : frameResource_)
    {
        if (frameResource.renderTargetViewDescriptor_ != nullptr)
        {
            renderTargetViewDescriptorHeap_->PushDescriptor(frameResource.renderTargetViewDescriptor_);
        }
        if (frameResource.depthStencilViewDescriptor_ != nullptr)
        {
            depthStencilViewDescriptorHeap_->PushDescriptor(frameResource.depthStencilViewDescriptor_);
        }
        if (frameResource.constantBufferViewDescriptor_ != nullptr)
        {
            srvCbvUavDescriptorHeap_->PushDescriptor(frameResource.constantBufferViewDescriptor_);
        }
        if (frameResource.sceneConstants_ != nullptr)
        {
            frameResource.constantBufferView_->Unmap(0, nullptr);
        }
    }

}

// DXGI �܂��
void Graphics::CreateDXGIDevice()
{
    HRESULT result = S_OK;
    UINT dxgiFactoryFlags = 0;

    // --- �f�o�b�O���C���[�̗L���� ---
#if 1 //defined( DEBUG ) | defined( _DEBUG )
    Microsoft::WRL::ComPtr<ID3D12Debug> debug;
    if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(debug.GetAddressOf()))))
    {
        debug->EnableDebugLayer();
    }

    // �t�@�N�g�����f�o�b�O���[�h�ō쐬����
    dxgiFactoryFlags |= DXGI_CREATE_FACTORY_DEBUG;

#endif

    // --- DXGI�t�@�N�g���̐��� ---
    result = CreateDXGIFactory2(
        dxgiFactoryFlags,
        IID_PPV_ARGS(dxgiFactory_.GetAddressOf())
    );
    _ASSERT_EXPR(SUCCEEDED(result), HRTrace(result));

    // --- �n�C�p�t�H�[�}���X�A�_�v�^�̎擾 ---
#pragma region HighPerfomanceAdapter
    Microsoft::WRL::ComPtr<IDXGIAdapter1> adapter;
    Microsoft::WRL::ComPtr<IDXGIFactory6> dxgiFactory6;

    if (SUCCEEDED(dxgiFactory_->QueryInterface(IID_PPV_ARGS(&dxgiFactory6))))
    {
        UINT adapterIndex = 0;
        while (1)
        {
            result = dxgiFactory6->EnumAdapterByGpuPreference(
                adapterIndex,
                DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE,
                IID_PPV_ARGS(adapter.GetAddressOf())
            );

            if (FAILED(result)) break;

            DXGI_ADAPTER_DESC1 adapterDesc;
            adapter->GetDesc1(&adapterDesc);

            // --- �\�t�g�E�F�A�A�_�v�^�[ ---
            if (adapterDesc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE) continue;

            // --- �f�o�C�X�𐶐��ł��邩�`�F�b�N ---
            result = D3D12CreateDevice(
                adapter.Get(),
                D3D_FEATURE_LEVEL_11_1,
                _uuidof(ID3D12Device),
                nullptr
            );

            if (SUCCEEDED(result)) break;

            ++adapterIndex;
        }
    }
#pragma endregion

    // --- �A�_�v�^�̎擾 ---
#pragma region Adapter
    if (adapter == nullptr)
    {
        result = dxgiFactory_->EnumAdapters1(0, adapter.GetAddressOf());

        if (FAILED(result))
        {
            // --- �擾�ł��Ȃ��ꍇ��Warp�A�_�v�^���擾 ---
            result = dxgiFactory_->EnumWarpAdapter(IID_PPV_ARGS(adapter.GetAddressOf()));
            _ASSERT_EXPR(SUCCEEDED(result), HRTrace(result));
        }
    }

#pragma endregion

    // --- �f�o�C�X�̐��� ---
#pragma region Device
    DXGI_ADAPTER_DESC1 adapterDesc;
    adapter->GetDesc1(&adapterDesc);

    OutputDebugStringW(adapterDesc.Description);
    OutputDebugStringW(L"\n");

    // --- �h���C�o�̋@�\���x�� (12_0��DirectX12�̊�����S�ɖ�����������) ---
    const D3D_FEATURE_LEVEL featureLevels[] =
    {
        D3D_FEATURE_LEVEL_12_1,
        D3D_FEATURE_LEVEL_12_0,
        D3D_FEATURE_LEVEL_11_1,
        D3D_FEATURE_LEVEL_11_0,
    };

    // --- �t�B�[�`���[���x�����������x�����珙�X�ɉ����Ȃ���f�o�C�X�𐶐����� ---
    for (D3D_FEATURE_LEVEL featureLevel : featureLevels)
    {
        result = D3D12CreateDevice(
            adapter.Get(),                          // �A�_�v�^�̃|�C���^ (nullptr�Ńf�t�H���g�̃A�_�v�^)
            featureLevel,                           // �t�B�[�`���[���x��
            IID_PPV_ARGS(device_.GetAddressOf())    // �i�[����|�C���^�̃A�h���X
        );

        if (SUCCEEDED(result)) break;
    }
    _ASSERT_EXPR(SUCCEEDED(result), HRTrace(result));
    device_->SetName(L"Device");

#pragma endregion
}

// �O���t�B�b�N�X�R�}���h�L���[�̐���
void Graphics::CreateGraphicsCommandQueue()
{
    HRESULT result = S_OK;

    // --- �R�}���h�L���[�̐��� ---
    D3D12_COMMAND_QUEUE_DESC cmdQueueDesc = {};
    cmdQueueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
    cmdQueueDesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
    cmdQueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
    cmdQueueDesc.NodeMask = 0;

    result = device_->CreateCommandQueue(
        &cmdQueueDesc,
        IID_PPV_ARGS(graphicsQueue_.commandQueue_.GetAddressOf())
    );
    _ASSERT_EXPR(SUCCEEDED(result), HRTrace(result));
    graphicsQueue_.commandQueue_->SetName(L"GraphicsCommandQueue");

    // --- �t�F���X���� ---
    result = device_->CreateFence(
        0,                                      // �t�F���X�̏����l
        D3D12_FENCE_FLAG_NONE,                  // �t�F���X�I�v�V����
        IID_PPV_ARGS(&graphicsQueue_.fence_)    // �i�[����|�C���^�̃A�h���X
    );
    _ASSERT_EXPR(SUCCEEDED(result), HRTrace(result));
    graphicsQueue_.fence_->SetName(L"GraphicsFence");

    // --- �t�F���X�p�̃C�x���g�𐶐� ---
    graphicsQueue_.fenceEvent_ = CreateEvent(
        nullptr,    // �Z�L�����e�B���� (NULL�Ńf�t�H���g����)
        FALSE,      // TRUE�Ŏ蓮���Z�b�g�AFALSE�Ŏ������Z�b�g
        FALSE,      // �����V�O�i�����
        nullptr     // ���O (NULL�Ŗ�����)
    );
}

// �X���b�v�`�F�C���̐���
void Graphics::CreateSwapChain(const HWND& hwnd, const UINT& bufferCount)
{
    HRESULT result = S_OK;

    // --- �X���b�v�`�F�[���̐ݒ� ---
    DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
    swapChainDesc.BufferCount = bufferCount;							// �t���[���o�b�t�@�̐�
    swapChainDesc.BufferDesc.Width = static_cast<UINT>(screenWidth_);	// �t���[���o�b�t�@�̕�
    swapChainDesc.BufferDesc.Height = static_cast<UINT>(screenHeight_); // �t���[���o�b�t�@�̍���
    swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;		// �t���[���o�b�t�@�̃s�N�Z���t�H�[�}�b�g
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;		// �t���[���o�b�t�@�̎g�p���@�i�E�C���h�E�ɏo�́j
    swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_STRETCHED;	    // �t���[���o�b�t�@�̐L�k�\
    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;			// �t���b�v��͑��₩�ɔj��
    swapChainDesc.OutputWindow = hwnd;								    // �o�̓E�C���h�E
    swapChainDesc.SampleDesc.Count = 1;								    // �A���`�G�C���A�V���O�̂��߂̃T���v�����O��
    swapChainDesc.SampleDesc.Quality = 0;								// �A���`�G�C���A�V���O�̕i���w��
    swapChainDesc.Windowed = true;									    // �E�C���h�E/�t���X�N���[��
    swapChainDesc.Flags = SyncInterval == 0 ? DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING : 0;

    // --- �X���b�v�`�F�[������ ---
    Microsoft::WRL::ComPtr<IDXGISwapChain> swapChain;
    
    result = dxgiFactory_->CreateSwapChain(
        graphicsQueue_.commandQueue_.Get(),     // �R�}���h�L���[�̃|�C���^
        &swapChainDesc,                         // �X���b�v�`�F�[���̐ݒ�
        swapChain.GetAddressOf()                // �i�[����|�C���^�̃A�h���X
    );
    _ASSERT_EXPR(SUCCEEDED(result), HRTrace(result));

    // --- IDXGISwapChain4�ɂ��� ---
    result = swapChain->QueryInterface(IID_PPV_ARGS(swapChain_.GetAddressOf()));
    _ASSERT_EXPR(SUCCEEDED(result), HRTrace(result));
}

// �t���[�����\�[�X
void Graphics::CreateFrameResource(const UINT& bufferCount)
{
    HRESULT result = S_OK;

    frameResource_.resize(bufferCount);

    for (UINT index = 0; index < bufferCount; ++index)
    {
        FrameResource& frameResource = frameResource_.at(index);

        // ----- �����_�[�^�[�Q�b�g -----
#pragma region �����_�[�^�[�Q�b�g
        // --- �o�b�N�o�b�t�@�[�擾 ---
        result = swapChain_->GetBuffer(index, IID_PPV_ARGS(frameResource.renderTargetView_.GetAddressOf()));
        _ASSERT_EXPR(SUCCEEDED(result), HRTrace(result));

        frameResource.renderTargetView_->SetName(L"BackBuffer");

        // --- �f�B�X�N���v�^�擾 ---
        frameResource.renderTargetViewDescriptor_ = renderTargetViewDescriptorHeap_->PopDescriptor();

        // --- �����_�[�^�[�Q�b�g�r���[�ݒ� ---
        D3D12_RENDER_TARGET_VIEW_DESC renderTargetViewDesc = {};
        renderTargetViewDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        renderTargetViewDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
        renderTargetViewDesc.Texture2D.MipSlice = 0;
        renderTargetViewDesc.Texture2D.PlaneSlice = 0;

        device_->CreateRenderTargetView(
            frameResource.renderTargetView_.Get(),                      // ���\�[�X�̃|�C���^
            &renderTargetViewDesc,                                      // �����_�[�^�[�Q�b�g�r���[�̐ݒ� (nullptr�Ńf�t�H���g������)
            frameResource.renderTargetViewDescriptor_->GetCpuHandle()   // �f�B�X�N���v�^���쐬����A�h���X
        );

#pragma endregion

        // ----- �f�v�X�X�e���V�� -----
#pragma region �f�v�X�X�e���V��
        // --- �q�[�v�v���p�e�B�̐ݒ� ---
        CD3DX12_HEAP_PROPERTIES heapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);

        // --- ���\�[�X�̐ݒ� ---
        D3D12_RESOURCE_DESC resourceDesc = {};
        resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
        resourceDesc.Alignment = 0;
        resourceDesc.Width = static_cast<UINT>(screenWidth_);
        resourceDesc.Height = static_cast<UINT>(screenHeight_);
        resourceDesc.DepthOrArraySize = 1;
        resourceDesc.MipLevels = 1;
        resourceDesc.Format = DXGI_FORMAT_D32_FLOAT;
        resourceDesc.SampleDesc.Count = 1;
        resourceDesc.SampleDesc.Quality = 0;
        resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
        resourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

        // --- �N���A�l�̐ݒ� ---
        D3D12_CLEAR_VALUE clearValue = {};
        clearValue.Format = DXGI_FORMAT_D32_FLOAT;
        clearValue.DepthStencil.Depth = 1.0f;
        clearValue.DepthStencil.Stencil = 0;

        // --- ���\�[�X�̐��� ---
        result = device_->CreateCommittedResource(
            &heapProp,
            D3D12_HEAP_FLAG_NONE,
            &resourceDesc,
            D3D12_RESOURCE_STATE_DEPTH_WRITE,
            &clearValue,
            IID_PPV_ARGS(frameResource.depthStencilView_.GetAddressOf())
        );
        _ASSERT_EXPR(SUCCEEDED(result), HRTrace(result));
        
        frameResource.depthStencilView_->SetName(L"DepthStencilView");

        // --- �f�B�X�N���v�^�擾 ---
        frameResource.depthStencilViewDescriptor_ = depthStencilViewDescriptorHeap_->PopDescriptor();

        // --- �f�v�X�X�e���V���r���[�̐ݒ� ---
        D3D12_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc = {};
        depthStencilViewDesc.Format = DXGI_FORMAT_D32_FLOAT;
        depthStencilViewDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
        depthStencilViewDesc.Flags = D3D12_DSV_FLAG_NONE;

        // --- �f�v�X�X�e���V���r���[���� ---
        device_->CreateDepthStencilView(
            frameResource.depthStencilView_.Get(),
            &depthStencilViewDesc,
            frameResource.depthStencilViewDescriptor_->GetCpuHandle()
        );

#pragma endregion

        // ----- �萔�o�b�t�@�[�̐��� -----
#pragma region �萔�o�b�t�@�[�̐���
        // --- �q�[�v�v���p�e�B�̐ݒ� ---
        heapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);

        // --- ���\�[�X�̐ݒ� ---
        resourceDesc = CD3DX12_RESOURCE_DESC::Buffer((sizeof(SceneConstants) + 0xff) & ~0xff);

        // --- �萔�o�b�t�@�[�̐��� ---
        result = device_->CreateCommittedResource(
            &heapProp,
            D3D12_HEAP_FLAG_NONE,
            &resourceDesc,
            D3D12_RESOURCE_STATE_GENERIC_READ,
            nullptr,
            IID_PPV_ARGS(frameResource.constantBufferView_.GetAddressOf())
        );
        _ASSERT_EXPR(SUCCEEDED(result), HRTrace(result));

        frameResource.constantBufferView_->SetName(L"SceneConstantBuffer");

        // --- �f�B�X�N���v�^�擾 ---
        frameResource.constantBufferViewDescriptor_ = srvCbvUavDescriptorHeap_->PopDescriptor();

        // --- �萔�o�b�t�@�[�r���[�̐��� ---
        D3D12_CONSTANT_BUFFER_VIEW_DESC constantBufferViewDesc = {};
        constantBufferViewDesc.BufferLocation = frameResource.constantBufferView_->GetGPUVirtualAddress();
        constantBufferViewDesc.SizeInBytes = static_cast<UINT>(resourceDesc.Width);

        device_->CreateConstantBufferView(
            &constantBufferViewDesc,
            frameResource.constantBufferViewDescriptor_->GetCpuHandle()
        );

        // --- �}�b�v ---
        result = frameResource.constantBufferView_->Map(0, nullptr, reinterpret_cast<void**>(&frameResource.sceneConstants_));
        _ASSERT_EXPR(SUCCEEDED(result), HRTrace(result));

#pragma endregion

        // ----- �R�}���h�A���P�[�^�[�̐��� -----
#pragma region �R�}���h�A���P�[�^�[�̐���
        result = device_->CreateCommandAllocator(
            D3D12_COMMAND_LIST_TYPE_DIRECT,                                 // �R�}���h�A���P�[�^�̎��
            IID_PPV_ARGS(frameResource.commandAllocator_.GetAddressOf())    // �i�[����|�C���^�̃A�h���X
        );
        _ASSERT_EXPR(SUCCEEDED(result), HRTrace(result));

        frameResource.commandAllocator_->SetName(L"GraphicsCommandAllocator");

#pragma endregion

        // ----- �R�}���h���X�g�̐��� -----
#pragma region
        result = device_->CreateCommandList(
            0,                                                      // �A�_�v�^���P�����g��Ȃ�
            D3D12_COMMAND_LIST_TYPE_DIRECT,                         // �R�}���h���X�g�̎��
            frameResource.commandAllocator_.Get(),                  // �R�}���h���X�g���쐬�Ɏg�p
            nullptr,                                                // �R�}���h���X�g�̏�����Ԃ̃p�C�v���C���X�e�[�g
            IID_PPV_ARGS(frameResource.commandList_.GetAddressOf()) // �i�[����|�C���^�̃A�h���X
        );
        _ASSERT_EXPR(SUCCEEDED(result), HRTrace(result));

        frameResource.commandList_->Close();
        frameResource.commandList_->SetName(L"GraphicsCommandList");

#pragma endregion
    }

}

// ���\�[�X�p�R�}���h
void Graphics::CreateResourceCommand()
{
    HRESULT result = S_OK;

    // ----- ���\�[�X�R�}���h�L���[�̐��� -----
#pragma region ���\�[�X�R�}���h�L���[�̐���
    // --- �R�}���h�L���[�̐��� ---
    D3D12_COMMAND_QUEUE_DESC commandQueueDesc = {};
    commandQueueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;             // �R�}���h���X�g�̎��
    commandQueueDesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;    // �v���C�I���e�B(�f�t�H���g)
    commandQueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;             // GPU�^�C���A�E�g���L��(�f�t�H���g)
    commandQueueDesc.NodeMask = 0;                                      // �A�_�v�^���P�����g��Ȃ��ꍇ�͂O���w��

    result = device_->CreateCommandQueue(
        &commandQueueDesc,                                              // �R�}���h�L���[�ݒ�
        IID_PPV_ARGS(resourceQueue_.commandQueue_.GetAddressOf())       // �i�[����|�C���^�̃A�h���X
    );
    _ASSERT_EXPR(SUCCEEDED(result), HRTrace(result));

    resourceQueue_.commandQueue_->SetName(L"ResourceCommandQueue");

    // --- �t�F���X���� ---
    result = device_->CreateFence(
        0,                                      // �t�F���X�̏����l
        D3D12_FENCE_FLAG_NONE,                  // �t�F���X�̃I�v�V����
        IID_PPV_ARGS(&resourceQueue_.fence_)    // �i�[����|�C���^�̃A�h���X
    );
    _ASSERT_EXPR(SUCCEEDED(result), HRTrace(result));

    resourceQueue_.fence_->SetName(L"ResourceFence");

    // --- �t�F���X�p�C�x���g�𐶐� ---
    resourceQueue_.fenceEvent_ = CreateEvent(
        nullptr,    // �Z�L�����e�B���� (NULL�Ńf�t�H���g����)
        FALSE,      // TRUE�Ŏ蓮���Z�b�g�AFALSE�Ŏ������Z�b�g
        FALSE,      // �����V�O�i�����
        nullptr     // ���O (NULL�Ŗ�����)
    );

#pragma endregion

    // ----- �R�}���h�A���P�[�^�̐��� -----
#pragma region �R�}���h�A���P�[�^�̐���
    result = device_->CreateCommandAllocator(
        D3D12_COMMAND_LIST_TYPE_DIRECT,
        IID_PPV_ARGS(resourceCommandAllocator_.GetAddressOf())
    );
    _ASSERT_EXPR(SUCCEEDED(result), HRTrace(result));

    resourceCommandAllocator_->SetName(L"ResourceCommandAllocator");

#pragma endregion

    // ----- �R�}���h���X�g�̐��� -----
#pragma region �R�}���h���X�g�̐���
    result = device_->CreateCommandList(
        0,
        D3D12_COMMAND_LIST_TYPE_DIRECT,
        resourceCommandAllocator_.Get(),
        nullptr,
        IID_PPV_ARGS(resourceCommandList_.GetAddressOf())
    );
    _ASSERT_EXPR(SUCCEEDED(result), HRTrace(result));

    resourceCommandList_->SetName(L"ResourceCommandList");

#pragma endregion

}

// �`��R�}���h�����܂ő҂�
void Graphics::WaitIdle()
{
    WaitIdle(graphicsQueue_);
    WaitIdle(resourceQueue_);
}

// �`����s
void Graphics::Execute()
{
    // --- �R�}���h�I���܂ő҂� ---
    WaitIdle(graphicsQueue_);

    UINT frameBufferIndex = swapChain_->GetCurrentBackBufferIndex();
    FrameResource& frameResource = frameResource_.at(frameBufferIndex);

    // --- �R�}���h���s ---
    ID3D12CommandList* commandLists[] =
    {
        frameResource.commandList_.Get()
    };

    graphicsQueue_.commandQueue_->ExecuteCommandLists(_countof(commandLists), commandLists);

    // --- ��ʕ\�� ---
    swapChain_->Present(SyncInterval, SyncInterval == 0 ? DXGI_PRESENT_ALLOW_TEARING : 0);
}

// �`��J�n
ID3D12GraphicsCommandList* Graphics::Begin()
{
    UINT frameBufferIndex = swapChain_->GetCurrentBackBufferIndex();
    FrameResource& frameResource = frameResource_.at(frameBufferIndex);

    // --- �R�}���h�̃��Z�b�g ---
    frameResource.commandAllocator_->Reset();
    frameResource.commandList_->Reset(frameResource.commandAllocator_.Get(), nullptr);

    // --- �f�B�X�N���v�^�q�[�v�����炩���ߐݒ� ---
    ID3D12DescriptorHeap* descriptorHeaps[] =
    {
        srvCbvUavDescriptorHeap_->GetD3DDescriptorHeap(),
        samplerDescriptorHeap_->GetD3DDescriptorHeap()
    };

    frameResource.commandList_->SetDescriptorHeaps(_countof(descriptorHeaps), descriptorHeaps);

    // ----- �����_�[�^�[�Q�b�g�̎w�� -----
#pragma region �����_�[�^�[�Q�b�g�̎w��
    // --- �r���[�|�[�g�̐ݒ� ---
    D3D12_VIEWPORT viewport;
    viewport.TopLeftX = 0;
    viewport.TopLeftY = 0;
    viewport.Width = screenWidth_;
    viewport.Height = screenHeight_;
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;

    frameResource.commandList_->RSSetViewports(1, &viewport);

    // --- �V�U�[�̐ݒ� ---
    D3D12_RECT scissorRect;
    scissorRect.left = 0;
    scissorRect.top = 0;
    scissorRect.right = static_cast<LONG>(screenWidth_);
    scissorRect.bottom = static_cast<LONG>(screenHeight_);

    frameResource.commandList_->RSSetScissorRects(1, &scissorRect);

    // --- ��ԑJ�ڃo���A�𒣂� ---
    D3D12_RESOURCE_BARRIER resourceBarrier = {};
    resourceBarrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
    resourceBarrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
    resourceBarrier.Transition.pResource = frameResource.renderTargetView_.Get();
    resourceBarrier.Transition.Subresource = 0;
    resourceBarrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
    resourceBarrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;

    frameResource.commandList_->ResourceBarrier(1, &resourceBarrier);

    // --- �����_�[�^�[�Q�b�g�ݒ� ---
    FLOAT clearColor[4] = { 0, 0, 0, 1 };
    D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = frameResource.renderTargetViewDescriptor_->GetCpuHandle();
    D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle = frameResource.depthStencilViewDescriptor_->GetCpuHandle();
    frameResource.commandList_->OMSetRenderTargets(1, &rtvHandle, false, &dsvHandle);
    frameResource.commandList_->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);
    frameResource.commandList_->ClearDepthStencilView(dsvHandle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

#pragma endregion

    return frameResource.commandList_.Get();
}

// �`��I��
void Graphics::End()
{
    UINT frameBufferIndex = swapChain_->GetCurrentBackBufferIndex();
    FrameResource& frameResource = frameResource_.at(frameBufferIndex);

    // --- ��ԑJ�ڃo���A�𒣂� ---
    D3D12_RESOURCE_BARRIER resourceBarrier = {};
    resourceBarrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
    resourceBarrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
    resourceBarrier.Transition.pResource = frameResource.renderTargetView_.Get();
    resourceBarrier.Transition.Subresource = 0;
    resourceBarrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
    resourceBarrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;

    frameResource.commandList_->ResourceBarrier(1, &resourceBarrier);

    frameResource.commandList_->Close();
}

// �V�[���p�萔�o�b�t�@�[�X�V
Descriptor* Graphics::UpdateSceneConstantBuffer(const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& projection, const DirectX::XMFLOAT3& lightDirection)
{
    UINT frameBufferIndex = swapChain_->GetCurrentBackBufferIndex();
    FrameResource& frameResource = frameResource_.at(frameBufferIndex);

    DirectX::XMMATRIX View = DirectX::XMLoadFloat4x4(&view);
    DirectX::XMMATRIX Projection = DirectX::XMLoadFloat4x4(&projection);
    DirectX::XMMATRIX ViewProjection = DirectX::XMMatrixMultiply(View, Projection);
    DirectX::XMStoreFloat4x4(&frameResource.sceneConstants_->viewProjection_, ViewProjection);
    frameResource.sceneConstants_->lightDirection_.x = lightDirection.x;
    frameResource.sceneConstants_->lightDirection_.y = lightDirection.y;
    frameResource.sceneConstants_->lightDirection_.z = lightDirection.z;
    frameResource.sceneConstants_->lightDirection_.w = 0.0f;

    return frameResource.constantBufferViewDescriptor_;
}

// �R�}���h�I���܂ő҂�
void Graphics::WaitIdle(CommandQueue& commandQueue)
{
    commandQueue.commandQueue_->Signal(commandQueue.fence_.Get(), ++commandQueue.fenceValue_);
    if (commandQueue.fence_->GetCompletedValue() < commandQueue.fenceValue_)
    {
        // --- �R�}���h�������ɃC�x���g����������悤�ɐݒ� ---
        commandQueue.fence_->SetEventOnCompletion(commandQueue.fenceValue_, commandQueue.fenceEvent_);

        // --- �C�x���g����������܂ő҂� ---
        WaitForSingleObject(commandQueue.fenceEvent_, INFINITE);
    }
}

HRESULT Graphics::LoadTextureFromFile(const wchar_t* filename, ID3D12Resource** resource)
{
    // --- �e�N�X�`���t�@�C���ǂݍ��� ---
    std::unique_ptr<uint8_t[]> image;
    D3D12_SUBRESOURCE_DATA subresourceData = {};

    HRESULT result = S_OK;

    result = DirectX::LoadWICTextureFromFile(
        device_.Get(),
        filename,
        resource,
        image,
        subresourceData
    );

    if (FAILED(result)) return result;

    // --- �ǂݍ��񂾉摜�����\�[�X�ɃR�s�[ ---
    D3D12_RESOURCE_DESC resourceDesc = (*resource)->GetDesc();

    result = CopyImage(
        reinterpret_cast<const BYTE*>(subresourceData.pData),
        static_cast<UINT>(resourceDesc.Width),
        resourceDesc.Height,
        resourceDesc.Format,
        *resource
    );

    return result;
}

// �e�N�X�`���ǂݍ���
HRESULT Graphics::LoadTexture(const char* filename, ID3D12Resource** resource)
{
    // --- �}���`�o�C�g�������烏�C�h�����֕ϊ� ---
    wchar_t wFilename[256];
    ::MultiByteToWideChar(CP_ACP, 0, filename, -1, wFilename, 256);

    // --- �e�N�X�`���t�@�C���ǂݍ��� ---
    std::unique_ptr<uint8_t[]> image;
    D3D12_SUBRESOURCE_DATA subresourceData = {};

    HRESULT result = S_OK;

    result = DirectX::LoadWICTextureFromFile(
        device_.Get(),
        wFilename,
        resource,
        image,
        subresourceData
    );

    if (FAILED(result)) return result;

    // --- �ǂݍ��񂾉摜�����\�[�X�ɃR�s�[ ---
    D3D12_RESOURCE_DESC resourceDesc = (*resource)->GetDesc();

    result = CopyImage(
        reinterpret_cast<const BYTE*>(subresourceData.pData),
        static_cast<UINT>(resourceDesc.Width),
        resourceDesc.Height,
        resourceDesc.Format,
        *resource
    );

    return result;
}

// �e�N�X�`���쐬
HRESULT Graphics::CreateTexture(const BYTE* pixels, UINT width, UINT height, DXGI_FORMAT format, ID3D12Resource** resource)
{
    CD3DX12_HEAP_PROPERTIES heapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);

    D3D12_RESOURCE_DESC resourceDesc = {};
    resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
    resourceDesc.Alignment = 0;
    resourceDesc.Width = width;
    resourceDesc.Height = height;
    resourceDesc.DepthOrArraySize = 1;
    resourceDesc.MipLevels = 1;
    resourceDesc.Format = format;
    resourceDesc.SampleDesc.Count = 1;
    resourceDesc.SampleDesc.Quality = 0;
    resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
    resourceDesc.Flags = D3D12_RESOURCE_FLAG_NONE;

    HRESULT result = device_->CreateCommittedResource(
        &heapProp,
        D3D12_HEAP_FLAG_NONE,
        &resourceDesc,
        D3D12_RESOURCE_STATE_COPY_DEST,
        nullptr,
        IID_PPV_ARGS(resource)
    );

    if (FAILED(result)) return result;

    return CopyImage(pixels, width, height, format, *resource);
}

// �_�~�[�e�N�X�`������
HRESULT Graphics::CreateDummyTexture(ID3D12Resource** resource)
{
    const UINT width = 8;
    const UINT height = 8;
    UINT pixels[width * height];
    ::memset(pixels, 0xFF, sizeof(pixels));

    // --- �q�[�v�v���p�e�B�̐ݒ� ---
    D3D12_HEAP_PROPERTIES heapProp = {};
    heapProp.Type = D3D12_HEAP_TYPE_CUSTOM;
    heapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
    heapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;
    heapProp.CreationNodeMask = 1;
    heapProp.VisibleNodeMask = 1;

    // --- ���\�[�X�̐ݒ� ---
    D3D12_RESOURCE_DESC resourceDesc = {};
    resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
    resourceDesc.Alignment = 0;
    resourceDesc.Width = width;
    resourceDesc.Height = height;
    resourceDesc.DepthOrArraySize = 1;
    resourceDesc.MipLevels = 1;
    resourceDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    resourceDesc.SampleDesc.Count = 1;
    resourceDesc.SampleDesc.Quality = 0;
    resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
    resourceDesc.Flags = D3D12_RESOURCE_FLAG_NONE;

    // --- ���\�[�X�̐��� ---
    HRESULT result = device_->CreateCommittedResource(
        &heapProp,
        D3D12_HEAP_FLAG_NONE,
        &resourceDesc,
        D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
        nullptr,
        IID_PPV_ARGS(resource)
    );

    if (FAILED(result)) return result;

    // �C���[�W���e�N�X�`���ɏ�������
    return (*resource)->WriteToSubresource(
        0,
        nullptr,
        pixels,
        sizeof(UINT) * width,
        sizeof(UINT) * width * height
    );
}

// �o�b�t�@�[�R�s�[
HRESULT Graphics::CopyBuffer(ID3D12Resource* srcResource, ID3D12Resource* dstResource)
{
    D3D12_RESOURCE_BARRIER resourceBarrier = {};
    resourceBarrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
    resourceBarrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
    resourceBarrier.Transition.pResource = dstResource;
    resourceBarrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
    resourceBarrier.Transition.StateBefore = D3D12_RESOURCE_STATE_GENERIC_READ;
    resourceBarrier.Transition.StateAfter = D3D12_RESOURCE_STATE_COPY_DEST;
    
    resourceCommandList_->ResourceBarrier(1, &resourceBarrier);

    resourceCommandList_->CopyResource(dstResource, srcResource);

    resourceBarrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
    resourceBarrier.Transition.StateAfter = D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE;

    resourceCommandList_->ResourceBarrier(1, &resourceBarrier);

    resourceCommandList_->Close();

    ID3D12CommandList* commandLists[] =
    {
        resourceCommandList_.Get()
    };

    resourceQueue_.commandQueue_->ExecuteCommandLists(_countof(commandLists), commandLists);

    WaitIdle(resourceQueue_);

    resourceCommandAllocator_->Reset();
    resourceCommandList_->Reset(resourceCommandAllocator_.Get(), nullptr);

    return S_OK;
}

// �C���[�W�R�s�[
HRESULT Graphics::CopyImage(const BYTE* pixels, UINT width, UINT height, DXGI_FORMAT format, ID3D12Resource* resource)
{
    HRESULT result = S_OK;

    // --- �A�b�v���[�h�p�e�N�X�`�����쐬 ---
    UINT bpp = BitsPerPixel(format);
    UINT rowPitch = (width * bpp) >> 3;
    UINT uploadPitch = (rowPitch + D3D12_TEXTURE_DATA_PITCH_ALIGNMENT - 1u) & ~(D3D12_TEXTURE_DATA_PITCH_ALIGNMENT - 1u);
    UINT uploadSize = height * uploadPitch;

    // --- �q�[�v�v���p�e�B�̐ݒ� ---
    D3D12_HEAP_PROPERTIES heapProp = {};
    heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;
    heapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
    heapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
    heapProp.CreationNodeMask = 1;
    heapProp.VisibleNodeMask = 1;

    // --- ���\�[�X�̐ݒ� ---
    D3D12_RESOURCE_DESC resourceDesc = {};
    resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
    resourceDesc.Alignment = 0;
    resourceDesc.Width = uploadSize;
    resourceDesc.Height = 1;
    resourceDesc.DepthOrArraySize = 1;
    resourceDesc.MipLevels = 1;
    resourceDesc.Format = DXGI_FORMAT_UNKNOWN;
    resourceDesc.SampleDesc.Count = 1;
    resourceDesc.SampleDesc.Quality = 0;
    resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
    resourceDesc.Flags = D3D12_RESOURCE_FLAG_NONE;

    // --- ���\�[�X�̐��� ---
    Microsoft::WRL::ComPtr<ID3D12Resource> uploadResource;

    result = device_->CreateCommittedResource(
        &heapProp,
        D3D12_HEAP_FLAG_NONE,
        &resourceDesc,
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(uploadResource.GetAddressOf())
    );

    if (FAILED(result)) return result;

    // --- �A�b�v���[�h�p�o�b�t�@�[�ɃC���[�W���R�s�[ ---
    void* mapped = nullptr;
    D3D12_RANGE range = {};
    range.Begin = 0;
    range.End = uploadSize;

    result = uploadResource->Map(0, &range, &mapped);
    
    if (FAILED(result)) return result;

    for (UINT y = 0; y < height; ++y)
    {
        memcpy((void*)((uintptr_t)mapped + y * uploadPitch), pixels + y * rowPitch, rowPitch);
    }

    uploadResource->Unmap(0, &range);

    // --- �A�b�v���[�h�p�o�b�t�@�[��e�N�X�`�����\�[�X�ɃC���[�W���R�s�[ ---
    D3D12_TEXTURE_COPY_LOCATION src = {};
    src.pResource = uploadResource.Get();
    src.Type = D3D12_TEXTURE_COPY_TYPE_PLACED_FOOTPRINT;
    src.PlacedFootprint.Footprint.Format = format;
    src.PlacedFootprint.Footprint.Width = width;
    src.PlacedFootprint.Footprint.Height = height;
    src.PlacedFootprint.Footprint.Depth = 1;
    src.PlacedFootprint.Footprint.RowPitch = uploadPitch;

    D3D12_TEXTURE_COPY_LOCATION dst = {};
    dst.pResource = resource;
    dst.Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
    dst.SubresourceIndex = 0;

    D3D12_RESOURCE_BARRIER resourceBarrier = {};
    resourceBarrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
    resourceBarrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
    resourceBarrier.Transition.pResource = resource;
    resourceBarrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
    resourceBarrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
    resourceBarrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;

    resourceCommandList_->CopyTextureRegion(&dst, 0, 0, 0, &src, nullptr);
    resourceCommandList_->ResourceBarrier(1, &resourceBarrier);
    resourceCommandList_->Close();

    // --- �R�}���h���s ---
    ID3D12CommandList* commandLists[] = { resourceCommandList_.Get() };

    resourceQueue_.commandQueue_->ExecuteCommandLists(_countof(commandLists), commandLists);

    // --- �R�}���h�I���܂ő҂� ---
    WaitIdle(resourceQueue_);

    // --- �R�}���h���X�g�N���A ---
    resourceCommandAllocator_->Reset();
    resourceCommandList_->Reset(resourceCommandAllocator_.Get(), nullptr);

    return S_OK;
}

// �t�H�[�}�b�g�ɑΉ������P�s�N�Z��������̃r�b�g����Ԃ�
UINT Graphics::BitsPerPixel(DXGI_FORMAT format)
{
    switch (static_cast<int>(format))
    {
    case DXGI_FORMAT_R32G32B32A32_TYPELESS:
    case DXGI_FORMAT_R32G32B32A32_FLOAT:
    case DXGI_FORMAT_R32G32B32A32_UINT:
    case DXGI_FORMAT_R32G32B32A32_SINT:
        return 128;

    case DXGI_FORMAT_R32G32B32_TYPELESS:
    case DXGI_FORMAT_R32G32B32_FLOAT:
    case DXGI_FORMAT_R32G32B32_UINT:
    case DXGI_FORMAT_R32G32B32_SINT:
        return 96;

    case DXGI_FORMAT_R16G16B16A16_TYPELESS:
    case DXGI_FORMAT_R16G16B16A16_FLOAT:
    case DXGI_FORMAT_R16G16B16A16_UNORM:
    case DXGI_FORMAT_R16G16B16A16_UINT:
    case DXGI_FORMAT_R16G16B16A16_SNORM:
    case DXGI_FORMAT_R16G16B16A16_SINT:
    case DXGI_FORMAT_R32G32_TYPELESS:
    case DXGI_FORMAT_R32G32_FLOAT:
    case DXGI_FORMAT_R32G32_UINT:
    case DXGI_FORMAT_R32G32_SINT:
    case DXGI_FORMAT_R32G8X24_TYPELESS:
    case DXGI_FORMAT_D32_FLOAT_S8X24_UINT:
    case DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS:
    case DXGI_FORMAT_X32_TYPELESS_G8X24_UINT:
    case DXGI_FORMAT_Y416:
    case DXGI_FORMAT_Y210:
    case DXGI_FORMAT_Y216:
        return 64;

    case DXGI_FORMAT_R10G10B10A2_TYPELESS:
    case DXGI_FORMAT_R10G10B10A2_UNORM:
    case DXGI_FORMAT_R10G10B10A2_UINT:
    case DXGI_FORMAT_R11G11B10_FLOAT:
    case DXGI_FORMAT_R8G8B8A8_TYPELESS:
    case DXGI_FORMAT_R8G8B8A8_UNORM:
    case DXGI_FORMAT_R8G8B8A8_UNORM_SRGB:
    case DXGI_FORMAT_R8G8B8A8_UINT:
    case DXGI_FORMAT_R8G8B8A8_SNORM:
    case DXGI_FORMAT_R8G8B8A8_SINT:
    case DXGI_FORMAT_R16G16_TYPELESS:
    case DXGI_FORMAT_R16G16_FLOAT:
    case DXGI_FORMAT_R16G16_UNORM:
    case DXGI_FORMAT_R16G16_UINT:
    case DXGI_FORMAT_R16G16_SNORM:
    case DXGI_FORMAT_R16G16_SINT:
    case DXGI_FORMAT_R32_TYPELESS:
    case DXGI_FORMAT_D32_FLOAT:
    case DXGI_FORMAT_R32_FLOAT:
    case DXGI_FORMAT_R32_UINT:
    case DXGI_FORMAT_R32_SINT:
    case DXGI_FORMAT_R24G8_TYPELESS:
    case DXGI_FORMAT_D24_UNORM_S8_UINT:
    case DXGI_FORMAT_R24_UNORM_X8_TYPELESS:
    case DXGI_FORMAT_X24_TYPELESS_G8_UINT:
    case DXGI_FORMAT_R9G9B9E5_SHAREDEXP:
    case DXGI_FORMAT_R8G8_B8G8_UNORM:
    case DXGI_FORMAT_G8R8_G8B8_UNORM:
    case DXGI_FORMAT_B8G8R8A8_UNORM:
    case DXGI_FORMAT_B8G8R8X8_UNORM:
    case DXGI_FORMAT_R10G10B10_XR_BIAS_A2_UNORM:
    case DXGI_FORMAT_B8G8R8A8_TYPELESS:
    case DXGI_FORMAT_B8G8R8A8_UNORM_SRGB:
    case DXGI_FORMAT_B8G8R8X8_TYPELESS:
    case DXGI_FORMAT_B8G8R8X8_UNORM_SRGB:
    case DXGI_FORMAT_AYUV:
    case DXGI_FORMAT_Y410:
    case DXGI_FORMAT_YUY2:
        return 32;

    case DXGI_FORMAT_P010:
    case DXGI_FORMAT_P016:
        return 24;

    case DXGI_FORMAT_R8G8_TYPELESS:
    case DXGI_FORMAT_R8G8_UNORM:
    case DXGI_FORMAT_R8G8_UINT:
    case DXGI_FORMAT_R8G8_SNORM:
    case DXGI_FORMAT_R8G8_SINT:
    case DXGI_FORMAT_R16_TYPELESS:
    case DXGI_FORMAT_R16_FLOAT:
    case DXGI_FORMAT_D16_UNORM:
    case DXGI_FORMAT_R16_UNORM:
    case DXGI_FORMAT_R16_UINT:
    case DXGI_FORMAT_R16_SNORM:
    case DXGI_FORMAT_R16_SINT:
    case DXGI_FORMAT_B5G6R5_UNORM:
    case DXGI_FORMAT_B5G5R5A1_UNORM:
    case DXGI_FORMAT_A8P8:
    case DXGI_FORMAT_B4G4R4A4_UNORM:
        return 16;

    case DXGI_FORMAT_NV12:
    case DXGI_FORMAT_420_OPAQUE:
    case DXGI_FORMAT_NV11:
        return 12;

    case DXGI_FORMAT_R8_TYPELESS:
    case DXGI_FORMAT_R8_UNORM:
    case DXGI_FORMAT_R8_UINT:
    case DXGI_FORMAT_R8_SNORM:
    case DXGI_FORMAT_R8_SINT:
    case DXGI_FORMAT_A8_UNORM:
    case DXGI_FORMAT_AI44:
    case DXGI_FORMAT_IA44:
    case DXGI_FORMAT_P8:
        return 8;

    case DXGI_FORMAT_R1_UNORM:
        return 1;

    case DXGI_FORMAT_BC1_TYPELESS:
    case DXGI_FORMAT_BC1_UNORM:
    case DXGI_FORMAT_BC1_UNORM_SRGB:
    case DXGI_FORMAT_BC4_TYPELESS:
    case DXGI_FORMAT_BC4_UNORM:
    case DXGI_FORMAT_BC4_SNORM:
        return 4;

    case DXGI_FORMAT_BC2_TYPELESS:
    case DXGI_FORMAT_BC2_UNORM:
    case DXGI_FORMAT_BC2_UNORM_SRGB:
    case DXGI_FORMAT_BC3_TYPELESS:
    case DXGI_FORMAT_BC3_UNORM:
    case DXGI_FORMAT_BC3_UNORM_SRGB:
    case DXGI_FORMAT_BC5_TYPELESS:
    case DXGI_FORMAT_BC5_UNORM:
    case DXGI_FORMAT_BC5_SNORM:
    case DXGI_FORMAT_BC6H_TYPELESS:
    case DXGI_FORMAT_BC6H_UF16:
    case DXGI_FORMAT_BC6H_SF16:
    case DXGI_FORMAT_BC7_TYPELESS:
    case DXGI_FORMAT_BC7_UNORM:
    case DXGI_FORMAT_BC7_UNORM_SRGB:
        return 8;

    default:
        return 0;
    }
}