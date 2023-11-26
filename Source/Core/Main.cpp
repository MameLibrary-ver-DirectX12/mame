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

// コンソール画面にフォーマット付きの文字列を表示
void DebugOutputFormatString(const char* format /*フォーマット(%dとか%f とかの)*/, ... /*可変長引数*/)
{
    // この関数はデバッグ用です。デバック時にしか動作しません
#ifdef _DEBUG
    va_list valist;
    va_start(valist, format);
    printf(format, valist);
    va_end(valist);
#endif// _DEBUG
}

LRESULT WindowProcedure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    // ウィンドウが破棄されたら呼ばれる
    if (msg == WM_DESTROY)
    {
        PostQuitMessage(0); // OS に対して「もうこのアプリは終わる」と伝える
        return 0;
    }
    return DefWindowProc(hwnd, msg, wparam, lparam); // 基底の処理を行う 
}

const unsigned int WINDOW_WIDTH = 1280;
const unsigned int WINDOW_HEIGHT = 720;

IDXGIFactory6* dxgiFactory_ = nullptr;
ID3D12Device* device_ = nullptr;
ID3D12CommandAllocator* cmdAllocator_ = nullptr;
ID3D12GraphicsCommandList* cmdList_ = nullptr;
ID3D12CommandQueue* cmdQueue_ = nullptr;
IDXGISwapChain4* swapChain_ = nullptr;

// アライメントに揃えたサイズを返す
size_t AlignmentedSize(size_t size/*元のサイズ*/, size_t alignment/*アライメントサイズ*/)
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

    // ウィンドウクラスの生成＆登録
    WNDCLASSEX w = {};
    w.cbSize = sizeof(WNDCLASSEX);
    w.lpfnWndProc = (WNDPROC)WindowProcedure;    // コールバック関数の指定
    w.lpszClassName = _T("Mame");                   // アプリケーションクラス名
    w.hInstance = GetModuleHandle(nullptr);      // ハンドルの取得

    RegisterClassEx(&w); // アプリケーションクラス ( ウィンドウクラスの指定を OS に伝える ）

    RECT wrc = { 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT }; // ウィンドウサイズを決める

    // 関数を使ってウィンドウのサイズを補正する
    AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, false);

    // ウィンドウオブジェクトの生成
    HWND hwnd = CreateWindow(
        w.lpszClassName,        // クラス名指定
        _T("Mame"),             // タイトルバーの文字
        WS_OVERLAPPEDWINDOW,    // タイトルバーと境界線があるウィンドウ
        CW_USEDEFAULT,          // 表示ｘ座標は OS にお任せ
        CW_USEDEFAULT,          // 表示ｙ座標は OS にお任せ
        wrc.right - wrc.left,   // ウィンドウ幅
        wrc.bottom - wrc.top,   // ウィンドウ高
        nullptr,                // 親ウィンドウハンドル
        nullptr,                // メニューハンドル
        w.hInstance,            // 呼び出しアプリケーションハンドル
        nullptr);               // 追加パラメーター

#ifdef _DEBUG
    // デバッグレイヤーをオンに
    // デバイス生成時前にやっておかないと、デバイス生成後にやると
    // デバイスがロスとしてしまうので注意
    EnableDebugLayer();
#endif// _DDEBUG

    // DirectX12 まわりの初期化
    // フィーチャーレベル列挙
    D3D_FEATURE_LEVEL levels[] =
    {
        D3D_FEATURE_LEVEL_12_1,
        D3D_FEATURE_LEVEL_12_0,
        D3D_FEATURE_LEVEL_11_1,
        D3D_FEATURE_LEVEL_11_0,
    };

    HRESULT result = CreateDXGIFactory1(IID_PPV_ARGS(&dxgiFactory_));


    // アダプターの列挙列
    std::vector<IDXGIAdapter*> adapters;

    // ここに特定の名前を持つアダプターオブジェクトが入る
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

        // 探したいアダプターの名前を確認
        if (strDesc.find(L"NVIDIA") != std::string::npos)
        {
            tmpAdapter = adpt;
            break;
        }
    }

    // Direct3D デバイス初期化
    D3D_FEATURE_LEVEL featureLevel;

    for (auto lv : levels)
    {
        if (D3D12CreateDevice(nullptr, lv, IID_PPV_ARGS(&device_)) == S_OK)
        {
            featureLevel = lv;
            break; // 生成可能なバージョンが見つかったらループを打ち切り
        }
    }

    result = device_->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&cmdAllocator_));
    result = device_->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, cmdAllocator_, nullptr, IID_PPV_ARGS(&cmdList_));

    // Queue 作成
    D3D12_COMMAND_QUEUE_DESC cmdQueueDesc = {};
    cmdQueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;          // タイムアウトなし
    cmdQueueDesc.NodeMask = 0;                                   // アダプターを１つしか使わないときは０でよい
    cmdQueueDesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL; // プライオリティーは特に指定なし
    cmdQueueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;          // コマンドリストと合わせる
    result = device_->CreateCommandQueue(&cmdQueueDesc, IID_PPV_ARGS(&cmdQueue_)); // キュー生成

    // SwapChain 作成
    DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
    swapChainDesc.Width = WINDOW_WIDTH;
    swapChainDesc.Height = WINDOW_HEIGHT;
    swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapChainDesc.Stereo = false;
    swapChainDesc.SampleDesc.Count = 1;
    swapChainDesc.SampleDesc.Quality = 0;
    swapChainDesc.BufferUsage = DXGI_USAGE_BACK_BUFFER;
    swapChainDesc.BufferCount = 2;
    swapChainDesc.Scaling = DXGI_SCALING_STRETCH;                   // バックバッファーは伸び縮み可能
    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;       // フリップ後は速やかに破棄
    swapChainDesc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;          // 特に指定なし
    swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;   // ウィンドウ <=> フルスクリーン切り替え可能

    result = dxgiFactory_->CreateSwapChainForHwnd(
        cmdQueue_,
        hwnd,
        &swapChainDesc,
        nullptr,
        nullptr,
        (IDXGISwapChain1**)&swapChain_
        // 本来は QueryInterface などを用いて
        // IDXGISwapChain4* への変換チェックをするが、
        // ここではわかりやすさ重視のためキャストで対応
    );

    // Descriptor heap 作成
    D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};
    heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;     // レンダーターゲットビューなので RTV
    heapDesc.NodeMask = 0;
    heapDesc.NumDescriptors = 2;                        // 表裏の２つ
    heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;   // 特に指定なし

    ID3D12DescriptorHeap* rtvHeaps = nullptr;
    result = device_->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&rtvHeaps));

    DXGI_SWAP_CHAIN_DESC swcDesc = {};
    result = swapChain_->GetDesc(&swcDesc);
    std::vector<ID3D12Resource*> backBuffers(swcDesc.BufferCount);
    D3D12_CPU_DESCRIPTOR_HANDLE handle = rtvHeaps->GetCPUDescriptorHandleForHeapStart();
    
    // SRGB レンダーターゲットビュー設定
    D3D12_RENDER_TARGET_VIEW_DESC rtvDesc = {};

    rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; // ガンマ補正あり (sRGB)
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

    // ウィンドウ表示
    ShowWindow(hwnd, SW_SHOW);

    struct Vertex
    {
        DirectX::XMFLOAT3 pos; // xyz座標
        DirectX::XMFLOAT2 uv;  // uv座標
    };

    Vertex vertices[] =
    {
        { { -0.5f, -0.7f, 0.0f }, { 0.0f, 1.0f } }, // 左下
        { { -0.5f,  0.7f, 0.0f }, { 0.0f, 0.0f } }, // 左上
        { {  0.5f, -0.7f, 0.0f }, { 1.0f, 1.0f } }, // 右下
        { {  0.5f,  0.7f, 0.0f }, { 1.0f, 0.0f } }, // 右上
    };


    // ----- 頂点バッファー生成 -----
#pragma region 頂点バッファー生成

    // ---  頂点ヒープの設定 ---
    D3D12_HEAP_PROPERTIES heapprop = {};

    heapprop.Type = D3D12_HEAP_TYPE_UPLOAD;                     // CPUからアクセス可能
    heapprop.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN; // 考えなくていい
    heapprop.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;  // CUSTOM以外のときはこれでよい

    // --- リソース設定 ---
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

    // --- UPLOAD ( 確保は可能 ) ---
    ID3D12Resource* vertexBuffer = nullptr;

    result = device_->CreateCommittedResource(
        &heapprop,
        D3D12_HEAP_FLAG_NONE,
        &resDesc,
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(&vertexBuffer));


    // --- バッファーに頂点情報をコピー ---
    Vertex* vertexMap = nullptr;

    result = vertexBuffer->Map(0, nullptr, (void**)&vertexMap);

    std::copy(std::begin(vertices), std::end(vertices), vertexMap);

    vertexBuffer->Unmap(0, nullptr);

    // --- 頂点バッファービューを生成 ---
    D3D12_VERTEX_BUFFER_VIEW vbView = {};
    vbView.BufferLocation = vertexBuffer->GetGPUVirtualAddress(); // バッファの仮想アドレス
    vbView.SizeInBytes = sizeof(vertices);                        // 全バイト数
    vbView.StrideInBytes = sizeof(vertices[0]);                   // １頂点あたりのバイト数

#pragma endregion// 頂点バッファー生成

#pragma region インデックスバッファー生成
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

    // --- バッファーにインデックスデータをコピー ---
    unsigned short* mappedIdx = nullptr;
    idxBuffer->Map(0, nullptr, (void**)&mappedIdx);
    std::copy(std::begin(indices), std::end(indices), mappedIdx);
    idxBuffer->Unmap(0, nullptr);

    // --- インデックスバッファービューを作成 ---
    D3D12_INDEX_BUFFER_VIEW ibView = {};
    ibView.BufferLocation = idxBuffer->GetGPUVirtualAddress();
    ibView.Format = DXGI_FORMAT_R16_UINT;
    ibView.SizeInBytes = sizeof(indices);

#pragma endregion// インデックスバッファー生成

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
            ::OutputDebugStringA("ファイルが見当たりません");
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
            ::OutputDebugStringA("ファイルが見当たりません");
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

    gPipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // 中身は 0xffffffff

    // blendState
    gPipeline.BlendState.AlphaToCoverageEnable = false;
    gPipeline.BlendState.IndependentBlendEnable = false;

    D3D12_RENDER_TARGET_BLEND_DESC renderTargetBlendDesc = {};

    // ひとまず加算や乗算やαブレンディングは使用しない
    renderTargetBlendDesc.BlendEnable = false;
    renderTargetBlendDesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

    // ひとまず論理演算は使用しない
    renderTargetBlendDesc.LogicOpEnable = false;

    gPipeline.BlendState.RenderTarget[0] = renderTargetBlendDesc;

    gPipeline.RasterizerState.MultisampleEnable = false;        // まだアンチエイリアシングは使わない
    gPipeline.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;  // カリングしない
    gPipeline.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID; // 中身を塗りつぶす
    gPipeline.RasterizerState.DepthClipEnable = true;           // 深度方向のクリッピングは有効に

    // 残り
    gPipeline.RasterizerState.FrontCounterClockwise = false;
    gPipeline.RasterizerState.DepthBias = D3D12_DEFAULT_DEPTH_BIAS;
    gPipeline.RasterizerState.DepthBiasClamp = D3D12_DEFAULT_DEPTH_BIAS_CLAMP;
    gPipeline.RasterizerState.SlopeScaledDepthBias = D3D12_DEFAULT_SLOPE_SCALED_DEPTH_BIAS;
    gPipeline.RasterizerState.AntialiasedLineEnable = false;
    gPipeline.RasterizerState.ForcedSampleCount = 0;
    gPipeline.RasterizerState.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;

    gPipeline.DepthStencilState.DepthEnable = false;
    gPipeline.DepthStencilState.StencilEnable = false;

    gPipeline.InputLayout.pInputElementDescs = inputLayout;     // レイアウト先頭アドレス
    gPipeline.InputLayout.NumElements = _countof(inputLayout);  // レイアウト配列数

    gPipeline.IBStripCutValue = D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_DISABLED;  // ストリップ時のカットなし
    gPipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE; // 三角形で構成

    gPipeline.NumRenderTargets = 1;                       // 今は一つのみ
    gPipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM; // 0 ～ 1 に正規化された RGBA

    gPipeline.SampleDesc.Count = 1;   // サンプリングは１ピクセルにつき１
    gPipeline.SampleDesc.Quality = 0; // クオリティは最低

    // --- ルートシグネチャ ---

    ID3D12RootSignature* rootSignature = nullptr;

    D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc = {};

    rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

    D3D12_DESCRIPTOR_RANGE descTblRange = {};
    descTblRange.NumDescriptors = 1; // テクスチャひとつ
    descTblRange.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV; // 種別はテクスチャ
    descTblRange.BaseShaderRegister = 0; // 0番スロットから
    descTblRange.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

    D3D12_ROOT_PARAMETER rootParam = {};

    rootParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
    rootParam.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;  // ピクセルシェーダーから見える
    rootParam.DescriptorTable.pDescriptorRanges = &descTblRange; // ディスクリプタレンジのアドレス
    rootParam.DescriptorTable.NumDescriptorRanges = 1;           // ディスクリプタレンジ数

    rootSignatureDesc.pParameters = &rootParam;
    rootSignatureDesc.NumParameters = 1;

    // --- サンプラー ---

    D3D12_STATIC_SAMPLER_DESC samplerDesc = {};

    samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP; // 横方向の繰り返し
    samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP; // 縦方向の繰り返し
    samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP; // 奥行の繰り返し
    samplerDesc.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK; // ボーダーは黒
    samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR; // 線形補間
    samplerDesc.MaxLOD = D3D12_FLOAT32_MAX; // ミップマップ最大値
    samplerDesc.MinLOD = 0.0f;              // ミップマップ最小値
    samplerDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; // ピクセルシェーダーから見える
    samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER; // リサンプリングしない

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
    viewport.Width = WINDOW_WIDTH;      // 出力先の幅   ( ピクセル数 )
    viewport.Height = WINDOW_HEIGHT;    // 出力先の高さ ( ピクセル数 )
    viewport.TopLeftX = 0;      // 出力先の左上座標X
    viewport.TopLeftY = 0;      // 出力先の左上座標Y
    viewport.MaxDepth = 1.0f;   // 深度最大値
    viewport.MinDepth = 0.0f;   // 深度最小値

    D3D12_RECT scissorrect = {};
    scissorrect.top = 0;
    scissorrect.left = 0;
    scissorrect.right = scissorrect.left + WINDOW_WIDTH;  // 切り抜き右座標
    scissorrect.bottom = scissorrect.top + WINDOW_HEIGHT; // 切り抜き下座標

#pragma region テクスチャ読み込み

    // --- WIC テクスチャのロード ---
    DirectX::TexMetadata metaData = {};
    DirectX::ScratchImage scratchImg = {};

    result = DirectX::LoadFromWICFile(
        L"./Resources/Image/textest.png",
        DirectX::WIC_FLAGS_NONE,
        &metaData,
        scratchImg
    );
    _ASSERT_EXPR(SUCCEEDED(result), "not found texture");

    // 生データ抽出
    const DirectX::Image* img = scratchImg.GetImage(0, 0, 0);


#if 1 // ID3D12GraphicsCommandList::CopyTextureRegion()
    
    // --- アップロード用リソースのヒープ作成 ---
    
    // 中間バッファーとしてのアップロードヒープ
    D3D12_HEAP_PROPERTIES uploadHeapProp = {};

    // マップ可能にするため、UPLOAD にする
    uploadHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;

    // アップロード用に使用する事前提なので UNKNOWN でよい
    uploadHeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
    uploadHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;

    uploadHeapProp.CreationNodeMask = 0;    // 単一アダプターのため０
    uploadHeapProp.VisibleNodeMask = 0;     // 単一アダプターのため０

    // --- リソース設定 ---

    D3D12_RESOURCE_DESC texResourceDesc = {};

    texResourceDesc.Format = DXGI_FORMAT_UNKNOWN; // 単なるデータの塊なので UNKNOWN
    texResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER; // 単なるバッファーとして指定

    texResourceDesc.Width = AlignmentedSize(img->rowPitch, D3D12_TEXTURE_DATA_PITCH_ALIGNMENT) * img->height; // データサイズ
    texResourceDesc.Height = 1;
    texResourceDesc.DepthOrArraySize = 1;
    texResourceDesc.MipLevels = 1;

    texResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR; // 連続したデータ
    texResourceDesc.Flags = D3D12_RESOURCE_FLAG_NONE;        // 特にフラグなし

    texResourceDesc.SampleDesc.Count = 1; // 通常テクスチャなのでアンチエイリアシングしない
    texResourceDesc.SampleDesc.Quality = 0;

    // --- 中間バッファー作成 ---
    ID3D12Resource* uploadBuffer = nullptr;

    result = device_->CreateCommittedResource(
        &uploadHeapProp,
        D3D12_HEAP_FLAG_NONE, // 特に指定なし
        &texResourceDesc,
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(&uploadBuffer)
    );

    _ASSERT_EXPR(SUCCEEDED(result), "error! create buffer");

    // --- コピー先のリソースを作成 ---

    // テクスチャのためのヒープ設定
    D3D12_HEAP_PROPERTIES texHeapProp = {};

    texHeapProp.Type = D3D12_HEAP_TYPE_DEFAULT; // テクスチャ用
    texHeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
    texHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
    texHeapProp.CreationNodeMask = 0;   // 単一アダプターのため０
    texHeapProp.VisibleNodeMask = 0;    // 単一アダプターのため０

    // リソース設定
    texResourceDesc.Format = metaData.format;
    texResourceDesc.Width = metaData.width;     // 幅
    texResourceDesc.Height = metaData.height;   // 高さ
    texResourceDesc.DepthOrArraySize = metaData.arraySize;  // 2D で配列でもないので １
    texResourceDesc.MipLevels = metaData.mipLevels;         // ミップマップもしないのでミップ数は1つ
    texResourceDesc.Dimension = static_cast<D3D12_RESOURCE_DIMENSION>(metaData.dimension);
    texResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;

    // テクスチャバッファーの作成
    ID3D12Resource* texBuffer = nullptr;

    result = device_->CreateCommittedResource(
        &texHeapProp,
        D3D12_HEAP_FLAG_NONE,           // 特に指定なし
        &texResourceDesc,
        D3D12_RESOURCE_STATE_COPY_DEST, // コピー先
        nullptr,
        IID_PPV_ARGS(&texBuffer)
    );

    _ASSERT_EXPR(SUCCEEDED(result), "error! create buffer");

    // --- アップロードリソースへのマップ ---

    uint8_t* mapforImg = nullptr; // image->pixels と同じ型にする
    result = uploadBuffer->Map(0, nullptr, (void**)&mapforImg); // マップ

    auto srcAddress = img->pixels;
    auto rowPitch = AlignmentedSize(img->rowPitch, D3D12_TEXTURE_DATA_PITCH_ALIGNMENT);

    for (int y = 0; y < img->height; ++y)
    {
        std::copy_n(srcAddress, rowPitch, mapforImg); // コピー

        // １行ごとの辻褄を合わせてやる
        srcAddress += img->rowPitch;
        mapforImg += rowPitch;
    }

    uploadBuffer->Unmap(0, nullptr); // アンマップ

    D3D12_TEXTURE_COPY_LOCATION src = {};

    // コピー元（アップロード側）設定
    src.pResource = uploadBuffer; // 中間バッファー
    src.Type = D3D12_TEXTURE_COPY_TYPE_PLACED_FOOTPRINT; // フィットプリント指定
    src.PlacedFootprint.Offset = 0;
    src.PlacedFootprint.Footprint.Width = metaData.width;
    src.PlacedFootprint.Footprint.Height = metaData.height;
    src.PlacedFootprint.Footprint.Depth = metaData.depth;
    src.PlacedFootprint.Footprint.RowPitch = static_cast<UINT>(AlignmentedSize(img->rowPitch, D3D12_TEXTURE_DATA_PITCH_ALIGNMENT));
    src.PlacedFootprint.Footprint.Format = img->format;

    D3D12_TEXTURE_COPY_LOCATION dst = {};

    // コピー先設定
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

    // コマンドリストの実行
    ID3D12CommandList* cmdLists[] = { cmdList_ };
    cmdQueue_->ExecuteCommandLists(1, cmdLists);

    // 待ち
    cmdQueue_->Signal(fence, ++fenceVal);

    if (fence->GetCompletedValue() != fenceVal)
    {
        auto event = CreateEvent(nullptr, false, false, nullptr);
        fence->SetEventOnCompletion(fenceVal, event);
        WaitForSingleObject(event, INFINITE);
        CloseHandle(event);
    }

    cmdAllocator_->Reset(); // キューをクリア
    cmdList_->Reset(cmdAllocator_, nullptr);


#else

    // WriteToSubresource で転送するためのヒープ設定
    D3D12_HEAP_PROPERTIES texHeapProp = {};

    texHeapProp.Type = D3D12_HEAP_TYPE_CUSTOM;                        // 特殊な設定なので DEFAULT でも UPLOAD でもない
    texHeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK; // ライトバック
    texHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;          // 転送はL0、つまりはCPU側から直接行う
    texHeapProp.CreationNodeMask = 0;                                 // 単一アダプタのため０
    texHeapProp.VisibleNodeMask = 0;                                  // 単一アダプタのため０

    D3D12_RESOURCE_DESC texResourceDesc = {};
    texResourceDesc.Format = metaData.format;               // フォーマット
    texResourceDesc.Width = metaData.width;                 // 幅
    texResourceDesc.Height = metaData.height;               // 高さ
    texResourceDesc.DepthOrArraySize = metaData.arraySize;  // 配列のサイズ
    texResourceDesc.SampleDesc.Count = 1;                   // 通常テクスチャなのでアンチエイリアシングしない
    texResourceDesc.SampleDesc.Quality = 0;                 // クオリティは最低
    texResourceDesc.MipLevels = metaData.mipLevels;         // Mipレベルの数
    texResourceDesc.Dimension = static_cast<D3D12_RESOURCE_DIMENSION>(metaData.dimension);
    texResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;  // レイアウトは決定しない
    texResourceDesc.Flags = D3D12_RESOURCE_FLAG_NONE;       // 特にフラグなし

    ID3D12Resource* texBuffer = nullptr;

    result = device_->CreateCommittedResource(
        &texHeapProp,
        D3D12_HEAP_FLAG_NONE, // 特に指定なし
        &texResourceDesc,
        D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, // テクスチャ用設定
        nullptr,
        IID_PPV_ARGS(&texBuffer)
    );

    _ASSERT_EXPR(SUCCEEDED(result), "Error! LoadTexture");

    // --- データ転送 ---

    result = texBuffer->WriteToSubresource(
        0,
        nullptr,        // 全領域へコピー
        img->pixels,    // 元データアドレス
        img->rowPitch,  // １ラインサイズ
        img->slicePitch // 全データサイズ
    );

    _ASSERT_EXPR(SUCCEEDED(result), "Error! TextureData");

#endif

    // --- シェーダーリソースビュー ---

    ID3D12DescriptorHeap* texDescHeap = nullptr;
    D3D12_DESCRIPTOR_HEAP_DESC texDescHeapDesc = {};

    texDescHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE; // シェーダーから見えるように
    texDescHeapDesc.NodeMask = 0; // マスクは0
    texDescHeapDesc.NumDescriptors = 1; // いまのところは１
    texDescHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV; // シェーダーリソースビュー用
    
    result = device_->CreateDescriptorHeap(&texDescHeapDesc, IID_PPV_ARGS(&texDescHeap));

    _ASSERT_EXPR(SUCCEEDED(result), "Error! CreateDescriptorHeap");

    D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};

    srvDesc.Format = metaData.format; // フォーマット
    srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
    srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D; // 2dテクスチャ
    srvDesc.Texture2D.MipLevels = 1; // ミップマップ使用しない

    device_->CreateShaderResourceView(
        texBuffer,  // ビューと関連付けるバッファー
        &srvDesc,   // テクスチャ設定情報
        texDescHeap->GetCPUDescriptorHandleForHeapStart() // ヒープのどこに割り当てるか
    );

    



#pragma endregion// テクスチャ読み込み

    MSG msg = {};
    unsigned int frame = 0;

    while (true)
    {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        // アプリケーションが終わるときに message が WM_QUIT になる
        if (msg.message == WM_QUIT)
        {
            break;
        }

        // DirectX処理

        // バックバッファのインデックスを取得
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

        // レンダーターゲットを指定
        auto rtvH = rtvHeaps->GetCPUDescriptorHandleForHeapStart();
        rtvH.ptr += bbIdx * device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
        cmdList_->OMSetRenderTargets(1, &rtvH, false, nullptr);

        // 画面クリア
        float r, g, b;
        r = (float)(0xff & frame >> 16) / 255.0f;
        g = (float)(0xff & frame >> 8) / 255.0f;
        b = (float)(0xff & frame >> 0) / 255.0f;

        float clearColor[] = { r, g, b, 1.0f }; // 黄色
        cmdList_->ClearRenderTargetView(rtvH, clearColor, 0, nullptr);
        ++frame;
        cmdList_->RSSetViewports(1, &viewport);
        cmdList_->RSSetScissorRects(1, &scissorrect);
        cmdList_->SetGraphicsRootSignature(rootSignature);

        cmdList_->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
        cmdList_->IASetVertexBuffers(0, 1, &vbView);
        cmdList_->IASetIndexBuffer(&ibView);

        // テクスチャ設定
        cmdList_->SetGraphicsRootSignature(rootSignature);
        cmdList_->SetDescriptorHeaps(1, &texDescHeap);
        cmdList_->SetGraphicsRootDescriptorTable(
            0, // ルートパラメーターインデックス
            texDescHeap->GetGPUDescriptorHandleForHeapStart() // ヒープアドレス
        );


        cmdList_->DrawIndexedInstanced(6, 1, 0, 0, 0);

        barrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
        barrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
        cmdList_->ResourceBarrier(1, &barrierDesc);

        // 命令のクローズ
        cmdList_->Close();

        // コマンドリストの実行
        ID3D12CommandList* cmdLists[] = { cmdList_ };
        cmdQueue_->ExecuteCommandLists(1, cmdLists);

        // 待ち
        cmdQueue_->Signal(fence, ++fenceVal);

        if (fence->GetCompletedValue() != fenceVal)
        {
            auto event = CreateEvent(nullptr, false, false, nullptr);
            fence->SetEventOnCompletion(fenceVal, event);
            WaitForSingleObject(event, INFINITE);
            CloseHandle(event);
        }
        cmdAllocator_->Reset(); // キューをクリア
        cmdList_->Reset(cmdAllocator_, pipelineState); // 再びコマンドリストを溜める準備

        // フリップ
        swapChain_->Present(1, 0);
    }

    // もうクラスは使わないので登録解除する
    UnregisterClass(w.lpszClassName, w.hInstance);
    return 0;
}