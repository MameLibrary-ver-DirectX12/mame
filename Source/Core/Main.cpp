#include <Windows.h>
#include <tchar.h>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <DirectXMath.h>

#include <vector>
#include <string>

#include <d3dcompiler.h>

#include <DirectXTex.h>


#ifdef _DEBUG
#include <iostream>
#endif// _DEBUG

#pragma comment(lib, "DirectXTex.lib")
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

// �R���\�[����ʂɃt�H�[�}�b�g�t���̕������\��
void DebugOutputFormatString(const char* format /*�t�H�[�}�b�g(%d�Ƃ�%f �Ƃ���)*/, ... /*�ϒ�����*/)
{
    // ���̊֐��̓f�o�b�O�p�ł��B�f�o�b�N���ɂ������삵�܂���
#ifdef _DEBUG
    va_list valist;
    va_start(valist, format);
    printf(format, valist);
    va_end(valist);
#endif// _DEBUG
}

LRESULT WindowProcedure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    // �E�B���h�E���j�����ꂽ��Ă΂��
    if (msg == WM_DESTROY)
    {
        PostQuitMessage(0); // OS �ɑ΂��āu�������̃A�v���͏I���v�Ɠ`����
        return 0;
    }
    return DefWindowProc(hwnd, msg, wparam, lparam); // ���̏������s�� 
}

const unsigned int WINDOW_WIDTH = 1280;
const unsigned int WINDOW_HEIGHT = 720;

IDXGIFactory6* dxgiFactory_ = nullptr;
ID3D12Device* device_ = nullptr;
ID3D12CommandAllocator* cmdAllocator_ = nullptr;
ID3D12GraphicsCommandList* cmdList_ = nullptr;
ID3D12CommandQueue* cmdQueue_ = nullptr;
IDXGISwapChain4* swapChain_ = nullptr;

// �A���C�����g�ɑ������T�C�Y��Ԃ�
size_t AlignmentedSize(size_t size/*���̃T�C�Y*/, size_t alignment/*�A���C�����g�T�C�Y*/)
{
    return size + alignment - size % alignment;
}

void EnableDebugLayer()
{
    ID3D12Debug* debugLayer = nullptr;
    if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugLayer))))
    {
        debugLayer->EnableDebugLayer();
        debugLayer->Release();
    }
}

#ifdef _DEBUG
int main()
{
#else
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
#endif// _DEBUG
    DebugOutputFormatString("Show window test.");

    // �E�B���h�E�N���X�̐������o�^
    WNDCLASSEX w = {};
    w.cbSize = sizeof(WNDCLASSEX);
    w.lpfnWndProc = (WNDPROC)WindowProcedure;    // �R�[���o�b�N�֐��̎w��
    w.lpszClassName = _T("Mame");                   // �A�v���P�[�V�����N���X��
    w.hInstance = GetModuleHandle(nullptr);      // �n���h���̎擾

    RegisterClassEx(&w); // �A�v���P�[�V�����N���X ( �E�B���h�E�N���X�̎w��� OS �ɓ`���� �j

    RECT wrc = { 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT }; // �E�B���h�E�T�C�Y�����߂�

    // �֐����g���ăE�B���h�E�̃T�C�Y��␳����
    AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, false);

    // �E�B���h�E�I�u�W�F�N�g�̐���
    HWND hwnd = CreateWindow(
        w.lpszClassName,        // �N���X���w��
        _T("Mame"),             // �^�C�g���o�[�̕���
        WS_OVERLAPPEDWINDOW,    // �^�C�g���o�[�Ƌ��E��������E�B���h�E
        CW_USEDEFAULT,          // �\�������W�� OS �ɂ��C��
        CW_USEDEFAULT,          // �\�������W�� OS �ɂ��C��
        wrc.right - wrc.left,   // �E�B���h�E��
        wrc.bottom - wrc.top,   // �E�B���h�E��
        nullptr,                // �e�E�B���h�E�n���h��
        nullptr,                // ���j���[�n���h��
        w.hInstance,            // �Ăяo���A�v���P�[�V�����n���h��
        nullptr);               // �ǉ��p�����[�^�[

#ifdef _DEBUG
    // �f�o�b�O���C���[���I����
    // �f�o�C�X�������O�ɂ���Ă����Ȃ��ƁA�f�o�C�X������ɂ���
    // �f�o�C�X�����X�Ƃ��Ă��܂��̂Œ���
    EnableDebugLayer();
#endif// _DDEBUG

    // DirectX12 �܂��̏�����
    // �t�B�[�`���[���x����
    D3D_FEATURE_LEVEL levels[] =
    {
        D3D_FEATURE_LEVEL_12_1,
        D3D_FEATURE_LEVEL_12_0,
        D3D_FEATURE_LEVEL_11_1,
        D3D_FEATURE_LEVEL_11_0,
    };

    HRESULT result = CreateDXGIFactory1(IID_PPV_ARGS(&dxgiFactory_));


    // �A�_�v�^�[�̗񋓗�
    std::vector<IDXGIAdapter*> adapters;

    // �����ɓ���̖��O�����A�_�v�^�[�I�u�W�F�N�g������
    IDXGIAdapter* tmpAdapter = nullptr;

    for (int i = 0; dxgiFactory_->EnumAdapters(i, &tmpAdapter) != DXGI_ERROR_NOT_FOUND; ++i)
    {
        adapters.push_back(tmpAdapter);
    }
    for (auto adpt : adapters)
    {
        DXGI_ADAPTER_DESC adapterDesc = {};
        adpt->GetDesc(&adapterDesc);

        std::wstring strDesc = adapterDesc.Description;

        // �T�������A�_�v�^�[�̖��O���m�F
        if (strDesc.find(L"NVIDIA") != std::string::npos)
        {
            tmpAdapter = adpt;
            break;
        }
    }

    // Direct3D �f�o�C�X������
    D3D_FEATURE_LEVEL featureLevel;

    for (auto lv : levels)
    {
        if (D3D12CreateDevice(nullptr, lv, IID_PPV_ARGS(&device_)) == S_OK)
        {
            featureLevel = lv;
            break; // �����\�ȃo�[�W���������������烋�[�v��ł��؂�
        }
    }

    result = device_->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&cmdAllocator_));
    result = device_->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, cmdAllocator_, nullptr, IID_PPV_ARGS(&cmdList_));

    // Queue �쐬
    D3D12_COMMAND_QUEUE_DESC cmdQueueDesc = {};
    cmdQueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;          // �^�C���A�E�g�Ȃ�
    cmdQueueDesc.NodeMask = 0;                                   // �A�_�v�^�[���P�����g��Ȃ��Ƃ��͂O�ł悢
    cmdQueueDesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL; // �v���C�I���e�B�[�͓��Ɏw��Ȃ�
    cmdQueueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;          // �R�}���h���X�g�ƍ��킹��
    result = device_->CreateCommandQueue(&cmdQueueDesc, IID_PPV_ARGS(&cmdQueue_)); // �L���[����

    // SwapChain �쐬
    DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
    swapChainDesc.Width = WINDOW_WIDTH;
    swapChainDesc.Height = WINDOW_HEIGHT;
    swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapChainDesc.Stereo = false;
    swapChainDesc.SampleDesc.Count = 1;
    swapChainDesc.SampleDesc.Quality = 0;
    swapChainDesc.BufferUsage = DXGI_USAGE_BACK_BUFFER;
    swapChainDesc.BufferCount = 2;
    swapChainDesc.Scaling = DXGI_SCALING_STRETCH;                   // �o�b�N�o�b�t�@�[�͐L�яk�݉\
    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;       // �t���b�v��͑��₩�ɔj��
    swapChainDesc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;          // ���Ɏw��Ȃ�
    swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;   // �E�B���h�E <=> �t���X�N���[���؂�ւ��\

    result = dxgiFactory_->CreateSwapChainForHwnd(
        cmdQueue_,
        hwnd,
        &swapChainDesc,
        nullptr,
        nullptr,
        (IDXGISwapChain1**)&swapChain_
        // �{���� QueryInterface �Ȃǂ�p����
        // IDXGISwapChain4* �ւ̕ϊ��`�F�b�N�����邪�A
        // �����ł͂킩��₷���d���̂��߃L���X�g�őΉ�
    );

    // Descriptor heap �쐬
    D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};
    heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;     // �����_�[�^�[�Q�b�g�r���[�Ȃ̂� RTV
    heapDesc.NodeMask = 0;
    heapDesc.NumDescriptors = 2;                        // �\���̂Q��
    heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;   // ���Ɏw��Ȃ�

    ID3D12DescriptorHeap* rtvHeaps = nullptr;
    result = device_->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&rtvHeaps));

    DXGI_SWAP_CHAIN_DESC swcDesc = {};
    result = swapChain_->GetDesc(&swcDesc);
    std::vector<ID3D12Resource*> backBuffers(swcDesc.BufferCount);
    D3D12_CPU_DESCRIPTOR_HANDLE handle = rtvHeaps->GetCPUDescriptorHandleForHeapStart();
    
    // SRGB �����_�[�^�[�Q�b�g�r���[�ݒ�
    D3D12_RENDER_TARGET_VIEW_DESC rtvDesc = {};

    rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; // �K���}�␳���� (sRGB)
    rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
    
    for (size_t i = 0; i < swcDesc.BufferCount; ++i)
    {
        result = swapChain_->GetBuffer(static_cast<UINT>(i), IID_PPV_ARGS(&backBuffers[i]));
        device_->CreateRenderTargetView(backBuffers[i], &rtvDesc, handle);
        handle.ptr += device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
    }

    ID3D12Fence* fence = nullptr;
    UINT64 fenceVal = 0;
    result = device_->CreateFence(fenceVal, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence));

    // �E�B���h�E�\��
    ShowWindow(hwnd, SW_SHOW);

    struct Vertex
    {
        DirectX::XMFLOAT3 pos; // xyz���W
        DirectX::XMFLOAT2 uv;  // uv���W
    };

    Vertex vertices[] =
    {
        { { -0.5f, -0.7f, 0.0f }, { 0.0f, 1.0f } }, // ����
        { { -0.5f,  0.7f, 0.0f }, { 0.0f, 0.0f } }, // ����
        { {  0.5f, -0.7f, 0.0f }, { 1.0f, 1.0f } }, // �E��
        { {  0.5f,  0.7f, 0.0f }, { 1.0f, 0.0f } }, // �E��
    };


    // ----- ���_�o�b�t�@�[���� -----
#pragma region ���_�o�b�t�@�[����

    // ---  ���_�q�[�v�̐ݒ� ---
    D3D12_HEAP_PROPERTIES heapprop = {};

    heapprop.Type = D3D12_HEAP_TYPE_UPLOAD;                     // CPU����A�N�Z�X�\
    heapprop.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN; // �l���Ȃ��Ă���
    heapprop.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;  // CUSTOM�ȊO�̂Ƃ��͂���ł悢

    // --- ���\�[�X�ݒ� ---
    D3D12_RESOURCE_DESC resDesc = {};

    resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
    resDesc.Width = sizeof(vertices);
    resDesc.Height = 1;
    resDesc.DepthOrArraySize = 1;
    resDesc.MipLevels = 1;
    resDesc.Format = DXGI_FORMAT_UNKNOWN;
    resDesc.SampleDesc.Count = 1;
    resDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
    resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

    // --- UPLOAD ( �m�ۂ͉\ ) ---
    ID3D12Resource* vertexBuffer = nullptr;

    result = device_->CreateCommittedResource(
        &heapprop,
        D3D12_HEAP_FLAG_NONE,
        &resDesc,
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(&vertexBuffer));


    // --- �o�b�t�@�[�ɒ��_�����R�s�[ ---
    Vertex* vertexMap = nullptr;

    result = vertexBuffer->Map(0, nullptr, (void**)&vertexMap);

    std::copy(std::begin(vertices), std::end(vertices), vertexMap);

    vertexBuffer->Unmap(0, nullptr);

    // --- ���_�o�b�t�@�[�r���[�𐶐� ---
    D3D12_VERTEX_BUFFER_VIEW vbView = {};
    vbView.BufferLocation = vertexBuffer->GetGPUVirtualAddress(); // �o�b�t�@�̉��z�A�h���X
    vbView.SizeInBytes = sizeof(vertices);                        // �S�o�C�g��
    vbView.StrideInBytes = sizeof(vertices[0]);                   // �P���_������̃o�C�g��

#pragma endregion// ���_�o�b�t�@�[����

#pragma region �C���f�b�N�X�o�b�t�@�[����
    unsigned short indices[] =
    {
        0, 1, 2,
        2, 1, 3,
    };

    ID3D12Resource* idxBuffer = nullptr;

    resDesc.Width = sizeof(indices);
    result = device_->CreateCommittedResource(
        &heapprop,
        D3D12_HEAP_FLAG_NONE,
        &resDesc,
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(&idxBuffer));

    // --- �o�b�t�@�[�ɃC���f�b�N�X�f�[�^���R�s�[ ---
    unsigned short* mappedIdx = nullptr;
    idxBuffer->Map(0, nullptr, (void**)&mappedIdx);
    std::copy(std::begin(indices), std::end(indices), mappedIdx);
    idxBuffer->Unmap(0, nullptr);

    // --- �C���f�b�N�X�o�b�t�@�[�r���[���쐬 ---
    D3D12_INDEX_BUFFER_VIEW ibView = {};
    ibView.BufferLocation = idxBuffer->GetGPUVirtualAddress();
    ibView.Format = DXGI_FORMAT_R16_UINT;
    ibView.SizeInBytes = sizeof(indices);

#pragma endregion// �C���f�b�N�X�o�b�t�@�[����

    ID3DBlob* vsBlob_ = nullptr;
    ID3DBlob* psBlob_ = nullptr;

    ID3DBlob* errorBlob = nullptr;

    result = D3DCompileFromFile(L"./Source/Shader/BasicVertexShader.hlsl",
        nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
        "main", "vs_5_0",
        D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
        0, &vsBlob_, &errorBlob);

#if 1
    _ASSERT_EXPR(!FAILED(result), "shader file not found");
#else
    if (FAILED(result))
    {
        if (result == HRESULT_FROM_WIN32(ERROR_FILE_NOT_FOUND))
        {
            ::OutputDebugStringA("�t�@�C������������܂���");
        }
        else
        {
            std::string errStr;
            errStr.resize(errorBlob->GetBufferSize());
            std::copy_n((char*)errorBlob->GetBufferPointer(), errorBlob->GetBufferSize(), errStr.begin());
            errStr += "\n";
            OutputDebugStringA(errStr.c_str());
        }
        exit(1);
    }
#endif

    result = D3DCompileFromFile(L"./Source/Shader/BasicPixelShader.hlsl",
        nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
        "main", "ps_5_0",
        D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
        0, &psBlob_, &errorBlob);

#if 1
    _ASSERT_EXPR(!FAILED(result), "shader file not found");
#else
    if (FAILED(result))
    {
        if (result == HRESULT_FROM_WIN32(ERROR_FILE_NOT_FOUND))
        {
            ::OutputDebugStringA("�t�@�C������������܂���");
        }
        else
        {
            std::string errStr;
            errStr.resize(errorBlob->GetBufferSize());
            std::copy_n((char*)errorBlob->GetBufferPointer(), errorBlob->GetBufferSize(), errStr.begin());
            errStr += "\n";
            OutputDebugStringA(errStr.c_str());
        }
        exit(1);
    }
#endif

    D3D12_INPUT_ELEMENT_DESC inputLayout[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
    };

    D3D12_GRAPHICS_PIPELINE_STATE_DESC gPipeline = {};
    gPipeline.pRootSignature = nullptr;
    gPipeline.VS.pShaderBytecode = vsBlob_->GetBufferPointer();
    gPipeline.VS.BytecodeLength = vsBlob_->GetBufferSize();
    gPipeline.PS.pShaderBytecode = psBlob_->GetBufferPointer();
    gPipeline.PS.BytecodeLength = psBlob_->GetBufferSize();

    gPipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // ���g�� 0xffffffff

    // blendState
    gPipeline.BlendState.AlphaToCoverageEnable = false;
    gPipeline.BlendState.IndependentBlendEnable = false;

    D3D12_RENDER_TARGET_BLEND_DESC renderTargetBlendDesc = {};

    // �ЂƂ܂����Z���Z�⃿�u�����f�B���O�͎g�p���Ȃ�
    renderTargetBlendDesc.BlendEnable = false;
    renderTargetBlendDesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

    // �ЂƂ܂��_�����Z�͎g�p���Ȃ�
    renderTargetBlendDesc.LogicOpEnable = false;

    gPipeline.BlendState.RenderTarget[0] = renderTargetBlendDesc;

    gPipeline.RasterizerState.MultisampleEnable = false;        // �܂��A���`�G�C���A�V���O�͎g��Ȃ�
    gPipeline.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;  // �J�����O���Ȃ�
    gPipeline.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID; // ���g��h��Ԃ�
    gPipeline.RasterizerState.DepthClipEnable = true;           // �[�x�����̃N���b�s���O�͗L����

    // �c��
    gPipeline.RasterizerState.FrontCounterClockwise = false;
    gPipeline.RasterizerState.DepthBias = D3D12_DEFAULT_DEPTH_BIAS;
    gPipeline.RasterizerState.DepthBiasClamp = D3D12_DEFAULT_DEPTH_BIAS_CLAMP;
    gPipeline.RasterizerState.SlopeScaledDepthBias = D3D12_DEFAULT_SLOPE_SCALED_DEPTH_BIAS;
    gPipeline.RasterizerState.AntialiasedLineEnable = false;
    gPipeline.RasterizerState.ForcedSampleCount = 0;
    gPipeline.RasterizerState.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;

    gPipeline.DepthStencilState.DepthEnable = false;
    gPipeline.DepthStencilState.StencilEnable = false;

    gPipeline.InputLayout.pInputElementDescs = inputLayout;     // ���C�A�E�g�擪�A�h���X
    gPipeline.InputLayout.NumElements = _countof(inputLayout);  // ���C�A�E�g�z��

    gPipeline.IBStripCutValue = D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_DISABLED;  // �X�g���b�v���̃J�b�g�Ȃ�
    gPipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE; // �O�p�`�ō\��

    gPipeline.NumRenderTargets = 1;                       // ���͈�̂�
    gPipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM; // 0 �` 1 �ɐ��K�����ꂽ RGBA

    gPipeline.SampleDesc.Count = 1;   // �T���v�����O�͂P�s�N�Z���ɂ��P
    gPipeline.SampleDesc.Quality = 0; // �N�I���e�B�͍Œ�

    // --- ���[�g�V�O�l�`�� ---

    ID3D12RootSignature* rootSignature = nullptr;

    D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc = {};

    rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

    D3D12_DESCRIPTOR_RANGE descTblRange = {};
    descTblRange.NumDescriptors = 1; // �e�N�X�`���ЂƂ�
    descTblRange.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV; // ��ʂ̓e�N�X�`��
    descTblRange.BaseShaderRegister = 0; // 0�ԃX���b�g����
    descTblRange.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

    D3D12_ROOT_PARAMETER rootParam = {};

    rootParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
    rootParam.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;  // �s�N�Z���V�F�[�_�[���猩����
    rootParam.DescriptorTable.pDescriptorRanges = &descTblRange; // �f�B�X�N���v�^�����W�̃A�h���X
    rootParam.DescriptorTable.NumDescriptorRanges = 1;           // �f�B�X�N���v�^�����W��

    rootSignatureDesc.pParameters = &rootParam;
    rootSignatureDesc.NumParameters = 1;

    // --- �T���v���[ ---

    D3D12_STATIC_SAMPLER_DESC samplerDesc = {};

    samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP; // �������̌J��Ԃ�
    samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP; // �c�����̌J��Ԃ�
    samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP; // ���s�̌J��Ԃ�
    samplerDesc.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK; // �{�[�_�[�͍�
    samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR; // ���`���
    samplerDesc.MaxLOD = D3D12_FLOAT32_MAX; // �~�b�v�}�b�v�ő�l
    samplerDesc.MinLOD = 0.0f;              // �~�b�v�}�b�v�ŏ��l
    samplerDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; // �s�N�Z���V�F�[�_�[���猩����
    samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER; // ���T���v�����O���Ȃ�

    rootSignatureDesc.pStaticSamplers = &samplerDesc;
    rootSignatureDesc.NumStaticSamplers = 1;

    ID3DBlob* rootSigBlob = nullptr;
    result = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, &errorBlob);
    _ASSERT_EXPR(SUCCEEDED(result), "Error! rootSignatureDesc");

    result = device_->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(), IID_PPV_ARGS(&rootSignature));
    _ASSERT_EXPR(SUCCEEDED(result), "Error! rootSignature");    
    
    rootSigBlob->Release();

    gPipeline.pRootSignature = rootSignature;
    ID3D12PipelineState* pipelineState = nullptr;
    result = device_->CreateGraphicsPipelineState(&gPipeline, IID_PPV_ARGS(&pipelineState));
    _ASSERT_EXPR(SUCCEEDED(result), L"error pipeline");

    D3D12_VIEWPORT viewport = {};
    viewport.Width = WINDOW_WIDTH;      // �o�͐�̕�   ( �s�N�Z���� )
    viewport.Height = WINDOW_HEIGHT;    // �o�͐�̍��� ( �s�N�Z���� )
    viewport.TopLeftX = 0;      // �o�͐�̍�����WX
    viewport.TopLeftY = 0;      // �o�͐�̍�����WY
    viewport.MaxDepth = 1.0f;   // �[�x�ő�l
    viewport.MinDepth = 0.0f;   // �[�x�ŏ��l

    D3D12_RECT scissorrect = {};
    scissorrect.top = 0;
    scissorrect.left = 0;
    scissorrect.right = scissorrect.left + WINDOW_WIDTH;  // �؂蔲���E���W
    scissorrect.bottom = scissorrect.top + WINDOW_HEIGHT; // �؂蔲�������W

#pragma region �e�N�X�`���ǂݍ���

    // --- WIC �e�N�X�`���̃��[�h ---
    DirectX::TexMetadata metaData = {};
    DirectX::ScratchImage scratchImg = {};

    result = DirectX::LoadFromWICFile(
        L"./Resources/Image/textest.png",
        DirectX::WIC_FLAGS_NONE,
        &metaData,
        scratchImg
    );
    _ASSERT_EXPR(SUCCEEDED(result), "not found texture");

    // ���f�[�^���o
    const DirectX::Image* img = scratchImg.GetImage(0, 0, 0);


#if 1 // ID3D12GraphicsCommandList::CopyTextureRegion()
    
    // --- �A�b�v���[�h�p���\�[�X�̃q�[�v�쐬 ---
    
    // ���ԃo�b�t�@�[�Ƃ��ẴA�b�v���[�h�q�[�v
    D3D12_HEAP_PROPERTIES uploadHeapProp = {};

    // �}�b�v�\�ɂ��邽�߁AUPLOAD �ɂ���
    uploadHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;

    // �A�b�v���[�h�p�Ɏg�p���鎖�O��Ȃ̂� UNKNOWN �ł悢
    uploadHeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
    uploadHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;

    uploadHeapProp.CreationNodeMask = 0;    // �P��A�_�v�^�[�̂��߂O
    uploadHeapProp.VisibleNodeMask = 0;     // �P��A�_�v�^�[�̂��߂O

    // --- ���\�[�X�ݒ� ---

    D3D12_RESOURCE_DESC texResourceDesc = {};

    texResourceDesc.Format = DXGI_FORMAT_UNKNOWN; // �P�Ȃ�f�[�^�̉�Ȃ̂� UNKNOWN
    texResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER; // �P�Ȃ�o�b�t�@�[�Ƃ��Ďw��

    texResourceDesc.Width = AlignmentedSize(img->rowPitch, D3D12_TEXTURE_DATA_PITCH_ALIGNMENT) * img->height; // �f�[�^�T�C�Y
    texResourceDesc.Height = 1;
    texResourceDesc.DepthOrArraySize = 1;
    texResourceDesc.MipLevels = 1;

    texResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR; // �A�������f�[�^
    texResourceDesc.Flags = D3D12_RESOURCE_FLAG_NONE;        // ���Ƀt���O�Ȃ�

    texResourceDesc.SampleDesc.Count = 1; // �ʏ�e�N�X�`���Ȃ̂ŃA���`�G�C���A�V���O���Ȃ�
    texResourceDesc.SampleDesc.Quality = 0;

    // --- ���ԃo�b�t�@�[�쐬 ---
    ID3D12Resource* uploadBuffer = nullptr;

    result = device_->CreateCommittedResource(
        &uploadHeapProp,
        D3D12_HEAP_FLAG_NONE, // ���Ɏw��Ȃ�
        &texResourceDesc,
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(&uploadBuffer)
    );

    _ASSERT_EXPR(SUCCEEDED(result), "error! create buffer");

    // --- �R�s�[��̃��\�[�X���쐬 ---

    // �e�N�X�`���̂��߂̃q�[�v�ݒ�
    D3D12_HEAP_PROPERTIES texHeapProp = {};

    texHeapProp.Type = D3D12_HEAP_TYPE_DEFAULT; // �e�N�X�`���p
    texHeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
    texHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
    texHeapProp.CreationNodeMask = 0;   // �P��A�_�v�^�[�̂��߂O
    texHeapProp.VisibleNodeMask = 0;    // �P��A�_�v�^�[�̂��߂O

    // ���\�[�X�ݒ�
    texResourceDesc.Format = metaData.format;
    texResourceDesc.Width = metaData.width;     // ��
    texResourceDesc.Height = metaData.height;   // ����
    texResourceDesc.DepthOrArraySize = metaData.arraySize;  // 2D �Ŕz��ł��Ȃ��̂� �P
    texResourceDesc.MipLevels = metaData.mipLevels;         // �~�b�v�}�b�v�����Ȃ��̂Ń~�b�v����1��
    texResourceDesc.Dimension = static_cast<D3D12_RESOURCE_DIMENSION>(metaData.dimension);
    texResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;

    // �e�N�X�`���o�b�t�@�[�̍쐬
    ID3D12Resource* texBuffer = nullptr;

    result = device_->CreateCommittedResource(
        &texHeapProp,
        D3D12_HEAP_FLAG_NONE,           // ���Ɏw��Ȃ�
        &texResourceDesc,
        D3D12_RESOURCE_STATE_COPY_DEST, // �R�s�[��
        nullptr,
        IID_PPV_ARGS(&texBuffer)
    );

    _ASSERT_EXPR(SUCCEEDED(result), "error! create buffer");

    // --- �A�b�v���[�h���\�[�X�ւ̃}�b�v ---

    uint8_t* mapforImg = nullptr; // image->pixels �Ɠ����^�ɂ���
    result = uploadBuffer->Map(0, nullptr, (void**)&mapforImg); // �}�b�v

    auto srcAddress = img->pixels;
    auto rowPitch = AlignmentedSize(img->rowPitch, D3D12_TEXTURE_DATA_PITCH_ALIGNMENT);

    for (int y = 0; y < img->height; ++y)
    {
        std::copy_n(srcAddress, rowPitch, mapforImg); // �R�s�[

        // �P�s���Ƃ̒�������킹�Ă��
        srcAddress += img->rowPitch;
        mapforImg += rowPitch;
    }

    uploadBuffer->Unmap(0, nullptr); // �A���}�b�v

    D3D12_TEXTURE_COPY_LOCATION src = {};

    // �R�s�[���i�A�b�v���[�h���j�ݒ�
    src.pResource = uploadBuffer; // ���ԃo�b�t�@�[
    src.Type = D3D12_TEXTURE_COPY_TYPE_PLACED_FOOTPRINT; // �t�B�b�g�v�����g�w��
    src.PlacedFootprint.Offset = 0;
    src.PlacedFootprint.Footprint.Width = metaData.width;
    src.PlacedFootprint.Footprint.Height = metaData.height;
    src.PlacedFootprint.Footprint.Depth = metaData.depth;
    src.PlacedFootprint.Footprint.RowPitch = static_cast<UINT>(AlignmentedSize(img->rowPitch, D3D12_TEXTURE_DATA_PITCH_ALIGNMENT));
    src.PlacedFootprint.Footprint.Format = img->format;

    D3D12_TEXTURE_COPY_LOCATION dst = {};

    // �R�s�[��ݒ�
    dst.pResource = texBuffer;
    dst.Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
    dst.SubresourceIndex = 0;

    cmdList_->CopyTextureRegion(&dst, 0, 0, 0, &src, nullptr);

    D3D12_RESOURCE_BARRIER barrierDesc = {};

    barrierDesc.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
    barrierDesc.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
    barrierDesc.Transition.pResource = texBuffer;
    barrierDesc.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
    barrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
    barrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;

    cmdList_->ResourceBarrier(1, &barrierDesc);
    cmdList_->Close();

    // �R�}���h���X�g�̎��s
    ID3D12CommandList* cmdLists[] = { cmdList_ };
    cmdQueue_->ExecuteCommandLists(1, cmdLists);

    // �҂�
    cmdQueue_->Signal(fence, ++fenceVal);

    if (fence->GetCompletedValue() != fenceVal)
    {
        auto event = CreateEvent(nullptr, false, false, nullptr);
        fence->SetEventOnCompletion(fenceVal, event);
        WaitForSingleObject(event, INFINITE);
        CloseHandle(event);
    }

    cmdAllocator_->Reset(); // �L���[���N���A
    cmdList_->Reset(cmdAllocator_, nullptr);


#else

    // WriteToSubresource �œ]�����邽�߂̃q�[�v�ݒ�
    D3D12_HEAP_PROPERTIES texHeapProp = {};

    texHeapProp.Type = D3D12_HEAP_TYPE_CUSTOM;                        // ����Ȑݒ�Ȃ̂� DEFAULT �ł� UPLOAD �ł��Ȃ�
    texHeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK; // ���C�g�o�b�N
    texHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;          // �]����L0�A�܂��CPU�����璼�ڍs��
    texHeapProp.CreationNodeMask = 0;                                 // �P��A�_�v�^�̂��߂O
    texHeapProp.VisibleNodeMask = 0;                                  // �P��A�_�v�^�̂��߂O

    D3D12_RESOURCE_DESC texResourceDesc = {};
    texResourceDesc.Format = metaData.format;               // �t�H�[�}�b�g
    texResourceDesc.Width = metaData.width;                 // ��
    texResourceDesc.Height = metaData.height;               // ����
    texResourceDesc.DepthOrArraySize = metaData.arraySize;  // �z��̃T�C�Y
    texResourceDesc.SampleDesc.Count = 1;                   // �ʏ�e�N�X�`���Ȃ̂ŃA���`�G�C���A�V���O���Ȃ�
    texResourceDesc.SampleDesc.Quality = 0;                 // �N�I���e�B�͍Œ�
    texResourceDesc.MipLevels = metaData.mipLevels;         // Mip���x���̐�
    texResourceDesc.Dimension = static_cast<D3D12_RESOURCE_DIMENSION>(metaData.dimension);
    texResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;  // ���C�A�E�g�͌��肵�Ȃ�
    texResourceDesc.Flags = D3D12_RESOURCE_FLAG_NONE;       // ���Ƀt���O�Ȃ�

    ID3D12Resource* texBuffer = nullptr;

    result = device_->CreateCommittedResource(
        &texHeapProp,
        D3D12_HEAP_FLAG_NONE, // ���Ɏw��Ȃ�
        &texResourceDesc,
        D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, // �e�N�X�`���p�ݒ�
        nullptr,
        IID_PPV_ARGS(&texBuffer)
    );

    _ASSERT_EXPR(SUCCEEDED(result), "Error! LoadTexture");

    // --- �f�[�^�]�� ---

    result = texBuffer->WriteToSubresource(
        0,
        nullptr,        // �S�̈�փR�s�[
        img->pixels,    // ���f�[�^�A�h���X
        img->rowPitch,  // �P���C���T�C�Y
        img->slicePitch // �S�f�[�^�T�C�Y
    );

    _ASSERT_EXPR(SUCCEEDED(result), "Error! TextureData");

#endif

    // --- �V�F�[�_�[���\�[�X�r���[ ---

    ID3D12DescriptorHeap* texDescHeap = nullptr;
    D3D12_DESCRIPTOR_HEAP_DESC texDescHeapDesc = {};

    texDescHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE; // �V�F�[�_�[���猩����悤��
    texDescHeapDesc.NodeMask = 0; // �}�X�N��0
    texDescHeapDesc.NumDescriptors = 1; // ���܂̂Ƃ���͂P
    texDescHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV; // �V�F�[�_�[���\�[�X�r���[�p
    
    result = device_->CreateDescriptorHeap(&texDescHeapDesc, IID_PPV_ARGS(&texDescHeap));

    _ASSERT_EXPR(SUCCEEDED(result), "Error! CreateDescriptorHeap");

    D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};

    srvDesc.Format = metaData.format; // �t�H�[�}�b�g
    srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
    srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D; // 2d�e�N�X�`��
    srvDesc.Texture2D.MipLevels = 1; // �~�b�v�}�b�v�g�p���Ȃ�

    device_->CreateShaderResourceView(
        texBuffer,  // �r���[�Ɗ֘A�t����o�b�t�@�[
        &srvDesc,   // �e�N�X�`���ݒ���
        texDescHeap->GetCPUDescriptorHandleForHeapStart() // �q�[�v�̂ǂ��Ɋ��蓖�Ă邩
    );

    



#pragma endregion// �e�N�X�`���ǂݍ���

    MSG msg = {};
    unsigned int frame = 0;

    while (true)
    {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        // �A�v���P�[�V�������I���Ƃ��� message �� WM_QUIT �ɂȂ�
        if (msg.message == WM_QUIT)
        {
            break;
        }

        // DirectX����

        // �o�b�N�o�b�t�@�̃C���f�b�N�X���擾
        UINT bbIdx = swapChain_->GetCurrentBackBufferIndex();

        D3D12_RESOURCE_BARRIER barrierDesc = {};
        barrierDesc.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
        barrierDesc.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
        barrierDesc.Transition.pResource = backBuffers[bbIdx];
        barrierDesc.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
        barrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
        barrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
        cmdList_->ResourceBarrier(1, &barrierDesc);

        cmdList_->SetPipelineState(pipelineState);

        // �����_�[�^�[�Q�b�g���w��
        auto rtvH = rtvHeaps->GetCPUDescriptorHandleForHeapStart();
        rtvH.ptr += bbIdx * device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
        cmdList_->OMSetRenderTargets(1, &rtvH, false, nullptr);

        // ��ʃN���A
        float r, g, b;
        r = (float)(0xff & frame >> 16) / 255.0f;
        g = (float)(0xff & frame >> 8) / 255.0f;
        b = (float)(0xff & frame >> 0) / 255.0f;

        float clearColor[] = { r, g, b, 1.0f }; // ���F
        cmdList_->ClearRenderTargetView(rtvH, clearColor, 0, nullptr);
        ++frame;
        cmdList_->RSSetViewports(1, &viewport);
        cmdList_->RSSetScissorRects(1, &scissorrect);
        cmdList_->SetGraphicsRootSignature(rootSignature);

        cmdList_->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
        cmdList_->IASetVertexBuffers(0, 1, &vbView);
        cmdList_->IASetIndexBuffer(&ibView);

        // �e�N�X�`���ݒ�
        cmdList_->SetGraphicsRootSignature(rootSignature);
        cmdList_->SetDescriptorHeaps(1, &texDescHeap);
        cmdList_->SetGraphicsRootDescriptorTable(
            0, // ���[�g�p�����[�^�[�C���f�b�N�X
            texDescHeap->GetGPUDescriptorHandleForHeapStart() // �q�[�v�A�h���X
        );


        cmdList_->DrawIndexedInstanced(6, 1, 0, 0, 0);

        barrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
        barrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
        cmdList_->ResourceBarrier(1, &barrierDesc);

        // ���߂̃N���[�Y
        cmdList_->Close();

        // �R�}���h���X�g�̎��s
        ID3D12CommandList* cmdLists[] = { cmdList_ };
        cmdQueue_->ExecuteCommandLists(1, cmdLists);

        // �҂�
        cmdQueue_->Signal(fence, ++fenceVal);

        if (fence->GetCompletedValue() != fenceVal)
        {
            auto event = CreateEvent(nullptr, false, false, nullptr);
            fence->SetEventOnCompletion(fenceVal, event);
            WaitForSingleObject(event, INFINITE);
            CloseHandle(event);
        }
        cmdAllocator_->Reset(); // �L���[���N���A
        cmdList_->Reset(cmdAllocator_, pipelineState); // �ĂуR�}���h���X�g�𗭂߂鏀��

        // �t���b�v
        swapChain_->Present(1, 0);
    }

    // �����N���X�͎g��Ȃ��̂œo�^��������
    UnregisterClass(w.lpszClassName, w.hInstance);
    return 0;
}