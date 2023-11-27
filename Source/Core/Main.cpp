#include <Windows.h>
#include <tchar.h>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <DirectXMath.h>

#include <vector>
#include <string>

#include <d3dcompiler.h>

#include <DirectXTex.h>
#include <d3dx12.h>

#ifdef _DEBUG
#include <iostream>
#endif// _DEBUG



#pragma comment(lib, "DirectXTex.lib")
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

#pragma pack(push, 1)
struct PMD_VERTEX
{
    DirectX::XMFLOAT3 pos;
    DirectX::XMFLOAT3 normal;
    DirectX::XMFLOAT2 uv;
    uint16_t bone_no[2];
    uint8_t weight;
    uint8_t edgeFlag;
    uint16_t dummy;
};
#pragma pack(pop)

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

    // --- �[�x�o�b�t�@�쐬 ---
#pragma region [Create] DepthStancilBuffer
    // �[�x�o�b�t�@�̎d�l
    D3D12_RESOURCE_DESC depthResDesc = {};
    
    depthResDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D; // 2�����̃e�N�X�`���f�[�^
    depthResDesc.Width = WINDOW_WIDTH;              // ���̓����_�[�^�[�Q�b�g�Ɠ���
    depthResDesc.Height = WINDOW_HEIGHT;            // �����̓����_�[�^�[�Q�b�g�Ɠ���
    depthResDesc.DepthOrArraySize = 1;              // �e�N�X�`���z��ł��A3D�e�N�X�`���ł��Ȃ�
    depthResDesc.Format = DXGI_FORMAT_D32_FLOAT;    // �[�x�������ݗp�t�H�[�}�b�g
    depthResDesc.SampleDesc.Count = 1;              // �T���v���͂P�s�N�Z��������P��
    depthResDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL; // �f�v�X�X�e���V���Ƃ��Ďg�p
    
    // �[�x�l�p�q�[�v�v���p�e�B
    D3D12_HEAP_PROPERTIES depthHeapProp = {};
    depthHeapProp.Type = D3D12_HEAP_TYPE_DEFAULT; // DEFAULT �Ȃ̂ł��Ƃ� UNKNOWN �ł悢
    depthHeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
    depthHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;

    // ���̃N���A�o�����[���d�v�ȈӖ�������
    D3D12_CLEAR_VALUE depthClearValu = {};
    depthClearValu.DepthStencil.Depth = 1.0f;       // �[�� 1.0f�i�ő�l�j�ŃN���A
    depthClearValu.Format = DXGI_FORMAT_D32_FLOAT;  // 32�r�b�gfloat�l�Ƃ��ăN���A

    ID3D12Resource* depthBuffer = nullptr;

    result = device_->CreateCommittedResource(
        &depthHeapProp,
        D3D12_HEAP_FLAG_NONE,
        &depthResDesc,
        D3D12_RESOURCE_STATE_DEPTH_WRITE, // �[�x�l�������݂Ɏg�p
        &depthClearValu,
        IID_PPV_ARGS(&depthBuffer)
    );

    _ASSERT_EXPR(SUCCEEDED(result), "create depthBuffer");

    // �[�x�̂��߂̃f�B�X�N���v�^�q�[�v�쐬
    D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc = {};
    dsvHeapDesc.NumDescriptors = 1;                     // �[�x�r���[�͂P��
    dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;  // �f�v�X�X�e���V���r���[�Ƃ��Ďg��
    ID3D12DescriptorHeap* dsvHeap = nullptr;
    result = device_->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(&dsvHeap));

    _ASSERT_EXPR(SUCCEEDED(result), "create depthHeap");

    // �[�x�r���[�쐬
    D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
    dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;                 // �[�x�n��32�r�b�g�d�l
    dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;  // 2D�e�N�X�`��
    dsvDesc.Flags = D3D12_DSV_FLAG_NONE;                    // �t���O�͓��ɂȂ�

    device_->CreateDepthStencilView(
        depthBuffer,
        &dsvDesc,
        dsvHeap->GetCPUDescriptorHandleForHeapStart()
    );

#pragma endregion [Create] DepthStancilBuffer

    ID3D12Fence* fence = nullptr;
    UINT64 fenceVal = 0;
    result = device_->CreateFence(fenceVal, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence));

    // �E�B���h�E�\��
    ShowWindow(hwnd, SW_SHOW);

#pragma region PMD 
    // --- PMD �w�b�_�[�\���� ---
    struct PMDHeader
    {
        float version;
        char modelName[20]; // ���f����
        char comment[256];   // ���f���R�����g
    };

    PMDHeader pmdHeader = {};

    char signature[3] = {}; // �V�O�l�`��
    FILE* fp;
    auto error = fopen_s(&fp, "./Resources/Model/�����~�N.pmd", "rb");
    if (fp == nullptr)
    {
        char strerr[256];
        strerror_s(strerr, 256, error);
        MessageBoxA(hwnd, strerr, "Open Error", MB_ICONERROR);
        return -1;
    }

    std::fread(signature, sizeof(signature), 1, fp);
    std::fread(&pmdHeader, sizeof(pmdHeader), 1, fp);

    unsigned int vertexNum; // ���_��
    std::fread(&vertexNum, sizeof(vertexNum), 1, fp);

    constexpr unsigned int pmdVertexSize = 38; // ���_�P������̃T�C�Y

    std::vector<PMD_VERTEX> vertices(vertexNum);  // �o�b�t�@�[�̊m��
    for (auto i = 0; i < vertexNum; ++i)
    {
        std::fread(&vertices[i], pmdVertexSize, 1, fp); // �ǂݍ���
    }

    unsigned int indicesNum; // �C���f�b�N�X��
    std::fread(&indicesNum, sizeof(indicesNum), 1, fp);

    auto heapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
    auto resDesc = CD3DX12_RESOURCE_DESC::Buffer(vertices.size() * sizeof(PMD_VERTEX));
    // UPLOAD (�m�ۂ͉\)
    ID3D12Resource* vertexBuffer;

    result = device_->CreateCommittedResource(
        &heapProp,
        D3D12_HEAP_FLAG_NONE,
        &resDesc,
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(&vertexBuffer)
    );

    _ASSERT_EXPR(SUCCEEDED(result), "create vertexBuffer");

    PMD_VERTEX* vertMap = nullptr;
    result = vertexBuffer->Map(0, nullptr, (void**)&vertMap);
    std::copy(vertices.begin(), vertices.end(), vertMap);
    vertexBuffer->Unmap(0, nullptr);

    D3D12_VERTEX_BUFFER_VIEW vbView = {};
    vbView.BufferLocation = vertexBuffer->GetGPUVirtualAddress(); // �o�b�t�@�̉��z�A�h���X
    vbView.SizeInBytes = static_cast<UINT>(vertices.size() * sizeof(PMD_VERTEX)); // �S�o�C�g��
    vbView.StrideInBytes = sizeof(PMD_VERTEX); // �P���_������̃o�C�g��

    std::vector<unsigned short> indices(indicesNum);

    std::fread(indices.data(), indices.size() * sizeof(indices[0]), 1, fp);

    fclose(fp);

    ID3D12Resource* idxBuffer = nullptr;
    heapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
    resDesc = CD3DX12_RESOURCE_DESC::Buffer(indices.size() * sizeof(indices[0]));

    result = device_->CreateCommittedResource(
        &heapProp,
        D3D12_HEAP_FLAG_NONE,
        &resDesc,
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(&idxBuffer)
    );

    unsigned short* mappedIdx = nullptr;
    idxBuffer->Map(0, nullptr, (void**)&mappedIdx);
    std::copy(indices.begin(), indices.end(), mappedIdx);
    idxBuffer->Unmap(0, nullptr);

    D3D12_INDEX_BUFFER_VIEW ibView = {};
    ibView.BufferLocation = idxBuffer->GetGPUVirtualAddress();
    ibView.Format = DXGI_FORMAT_R16_UINT;
    ibView.SizeInBytes = static_cast<UINT>(indices.size() * sizeof(indices[0]));

#pragma endregion PMD

#if 0
    struct Vertex
    {
        DirectX::XMFLOAT3 pos; // xyz���W
        DirectX::XMFLOAT2 uv;  // uv���W
    };

    //Vertex vertices[] =
    //{
    //    { {  -1.0f, -1.0f, 0.0f }, { 0.0f, 1.0f } }, // ����
    //    { {  -1.0f,  1.0f, 0.0f }, { 0.0f, 0.0f } }, // ����
    //    { {   1.0f, -1.0f, 0.0f }, { 1.0f, 1.0f } }, // �E��
    //    { {   1.0f,  1.0f, 0.0f }, { 1.0f, 0.0f } }, // �E��
    //};


    // ----- ���_�o�b�t�@�[���� -----
#pragma region ���_�o�b�t�@�[����
    // --- UPLOAD ( �m�ۂ͉\ ) ---
    ID3D12Resource* vertexBuffer = nullptr;

    auto heapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
    auto resDesc  = CD3DX12_RESOURCE_DESC::Buffer(sizeof(vertices));

    result = device_->CreateCommittedResource(
        &heapProp,
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
#endif
#pragma endregion// ���_�o�b�t�@�[����

#pragma region �C���f�b�N�X�o�b�t�@�[����
#if 0
    unsigned short indices[] =
    {
        0, 1, 2,
        2, 1, 3,
    };

    ID3D12Resource* idxBuffer = nullptr;

    heapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
    resDesc  = CD3DX12_RESOURCE_DESC::Buffer(sizeof(indices));

    result = device_->CreateCommittedResource(
        &heapProp,
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
#endif

#pragma endregion// �C���f�b�N�X�o�b�t�@�[����

    ID3DBlob* vsBlob_ = nullptr;
    ID3DBlob* psBlob_ = nullptr;

    ID3DBlob* errorBlob = nullptr;

    // --- VertexShader ---
    result = D3DCompileFromFile(L"./Source/Shader/BasicVertexShader.hlsl",
        nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
        "main", "vs_5_0",
        D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
        0, &vsBlob_, &errorBlob);

    _ASSERT_EXPR(!FAILED(result), "shader file not found");

    // -- PixelShader ---
    result = D3DCompileFromFile(L"./Source/Shader/BasicPixelShader.hlsl",
        nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
        "main", "ps_5_0",
        D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
        0, &psBlob_, &errorBlob);

    _ASSERT_EXPR(!FAILED(result), "shader file not found");


    //D3D12_INPUT_ELEMENT_DESC inputLayout[] =
    //{
    //    { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
    //    { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
    //};
    D3D12_INPUT_ELEMENT_DESC inputLayout[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
        { "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
        { "BONE_NO",  0, DXGI_FORMAT_R16G16_UINT,     0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
        { "WEIGHT",   0, DXGI_FORMAT_R8_UINT,         0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
        { "EDGE_FLG", 0, DXGI_FORMAT_R8_UINT,         0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
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

    //  -- �[�x��� --
    gPipeline.DepthStencilState.DepthEnable = true;                          // �[�x�o�b�t�@�[���g��
    gPipeline.DepthStencilState.StencilEnable = false;
    gPipeline.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL; // ��������
    gPipeline.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;      // �����������̗p
    gPipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;

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

    D3D12_DESCRIPTOR_RANGE descTblRange[2] = {}; // �e�N�X�`���ƒ萔
    
    // �e�N�X�`���p ���W�X�^�[�O��
    descTblRange[0].NumDescriptors = 1;                             // �e�N�X�`���P��
    descTblRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;    // ��ʂ̓e�N�X�`��
    descTblRange[0].BaseShaderRegister = 0;                         // �O�ԃX���b�g����
    descTblRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

    // �萔�p ���W�X�^�[�O��
    descTblRange[1].NumDescriptors = 1;                             // �萔�P��
    descTblRange[1].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;    // ��ʂ͒萔
    descTblRange[1].BaseShaderRegister = 0;                         // �O�ԃX���b�g
    descTblRange[1].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

    D3D12_ROOT_PARAMETER rootParam = {};

    rootParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
    rootParam.DescriptorTable.pDescriptorRanges = descTblRange;  // �z��擪�A�h���X
    rootParam.DescriptorTable.NumDescriptorRanges = 2;           // �f�B�X�N���v�^�����W��
    rootParam.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;    // �S�ẴV�F�[�_�[���猩����

    rootSignatureDesc.pParameters = &rootParam; // ���[�g�p�����[�^�̐擪�A�h���X
    rootSignatureDesc.NumParameters = 1;        // ���[�g�p�����[�^�[��

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

    auto barrier = CD3DX12_RESOURCE_BARRIER::Transition(texBuffer,
        D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);

    cmdList_->ResourceBarrier(1, &barrier);
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


    // --- �V�F�[�_�[���\�[�X�r���[ ---

    ID3D12DescriptorHeap* basicDescHeap = nullptr;
    D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc = {};

    descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE; // �V�F�[�_�[���猩����悤��
    descHeapDesc.NodeMask = 0;                                      // �}�X�N��0
    descHeapDesc.NumDescriptors = 2;                                // SRV 1�� CBV �P��
    descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;     // �f�B�X�N���v�^�q�[�v���
    
    result = device_->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(&basicDescHeap));
    _ASSERT_EXPR(SUCCEEDED(result), "Error! CreateDescriptorHeap");

    // �V�F�[�_�[���ɓn�����߂̊�{�I�ȍs��f�[�^
    struct MatricesData
    {
        DirectX::XMMATRIX world;
        DirectX::XMMATRIX viewProjection;
    };

    // ----- �萔�o�b�t�@�[�쐬 -----
    DirectX::XMFLOAT3 eye       = { 0, 10, -15 };
    DirectX::XMFLOAT3 target    = { 0, 10, 0 };
    DirectX::XMFLOAT3 up        = { 0, 1, 0 };

    DirectX::XMMATRIX W = DirectX::XMMatrixIdentity();
    DirectX::XMMATRIX V = DirectX::XMMatrixLookAtLH(DirectX::XMLoadFloat3(&eye), DirectX::XMLoadFloat3(&target), DirectX::XMLoadFloat3(&up));
    DirectX::XMMATRIX P = DirectX::XMMatrixPerspectiveFovLH(DirectX::XM_PIDIV2, // ����p
        static_cast<float>(WINDOW_WIDTH) / static_cast<float>(WINDOW_HEIGHT),   // �A�X�y�N�g��
        1.0f,   // near
        100.0f   // far
    );

    heapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
    resDesc = CD3DX12_RESOURCE_DESC::Buffer((sizeof(MatricesData) + 0xff) & ~0xff);
    
    ID3D12Resource* constantBuffer = nullptr;

    device_->CreateCommittedResource(
        &heapProp,
        D3D12_HEAP_FLAG_NONE,
        &resDesc,
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(&constantBuffer)
    );

    // --- �}�b�v�ɂ��萔�̃R�s�[ ---
    MatricesData* mapMatrix;                                    // �}�b�v��������|�C���^
    result = constantBuffer->Map(0, nullptr, (void**)&mapMatrix);   // �}�b�v
    _ASSERT_EXPR(SUCCEEDED(result), "map error");
    mapMatrix->world = W;
    mapMatrix->viewProjection = V * P;


    D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};

    // �ʏ�e�N�X�`���r���[�쐬
    srvDesc.Format = metaData.format;                       // �t�H�[�}�b�g
    srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
    srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;  // 2d�e�N�X�`��
    srvDesc.Texture2D.MipLevels = 1;                        // �~�b�v�}�b�v�g�p���Ȃ�

    // �f�B�X�N���v�^�̐擪�n���h�����擾���Ă���
    auto basicHeapHandle = basicDescHeap->GetCPUDescriptorHandleForHeapStart();

    // �V�F�[�_�[���\�[�X�r���[�̍쐬
    device_->CreateShaderResourceView(
        texBuffer,      // �r���[�Ɗ֘A�t����o�b�t�@�[
        &srvDesc,       // �e�N�X�`���ݒ���
        basicHeapHandle // �q�[�v�̂ǂ��Ɋ��蓖�Ă邩
    );

    // ���̏ꏊ�Ɉړ�
    basicHeapHandle.ptr +=
        device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

    D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc = {};
    cbvDesc.BufferLocation = constantBuffer->GetGPUVirtualAddress();
    cbvDesc.SizeInBytes = constantBuffer->GetDesc().Width;

    // �萔�o�b�t�@�[�r���[�̍쐬
    device_->CreateConstantBufferView(&cbvDesc, basicHeapHandle);

#pragma endregion// �e�N�X�`���ǂݍ���



    


    MSG msg = {};
    unsigned int frame = 0;
    float angle = 0.0f;

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

        angle += 1.0f;
        W = DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(angle));
        mapMatrix->world = W;
        mapMatrix->viewProjection = V * P;

        // DirectX����

        // �o�b�N�o�b�t�@�̃C���f�b�N�X���擾
        UINT bbIdx = swapChain_->GetCurrentBackBufferIndex();

        barrier = CD3DX12_RESOURCE_BARRIER::Transition(backBuffers[bbIdx],
            D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);

        cmdList_->ResourceBarrier(1, &barrier);

        cmdList_->SetPipelineState(pipelineState);

        // �����_�[�^�[�Q�b�g���w��
        auto rtvH = rtvHeaps->GetCPUDescriptorHandleForHeapStart();
        rtvH.ptr += bbIdx * device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
        auto dsvH = dsvHeap->GetCPUDescriptorHandleForHeapStart();
        cmdList_->OMSetRenderTargets(1, &rtvH, false, &dsvH);

        // ��ʃN���A
        float clearColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
        cmdList_->ClearRenderTargetView(rtvH, clearColor, 0, nullptr);
        cmdList_->ClearDepthStencilView(dsvH, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

        ++frame;
        cmdList_->RSSetViewports(1, &viewport);
        cmdList_->RSSetScissorRects(1, &scissorrect);
        cmdList_->SetGraphicsRootSignature(rootSignature);

        cmdList_->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
        cmdList_->IASetVertexBuffers(0, 1, &vbView);
        cmdList_->IASetIndexBuffer(&ibView);

        // �e�N�X�`���ݒ�
        cmdList_->SetGraphicsRootSignature(rootSignature);
        cmdList_->SetDescriptorHeaps(1, &basicDescHeap);
        cmdList_->SetGraphicsRootDescriptorTable(
            0,                                                  // ���[�g�p�����[�^�[�C���f�b�N�X
            basicDescHeap->GetGPUDescriptorHandleForHeapStart() // �q�[�v�A�h���X
        );

        //cmdList_->DrawIndexedInstanced(6, 1, 0, 0, 0);
        //cmdList_->DrawInstanced(vertexNum, 1, 0, 0);
        cmdList_->DrawIndexedInstanced(indicesNum, 1, 0, 0, 0);

        barrier = CD3DX12_RESOURCE_BARRIER::Transition(backBuffers[bbIdx],
            D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
        cmdList_->ResourceBarrier(1, &barrier);

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