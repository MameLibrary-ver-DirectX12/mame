#include "Sprite.h"

#include <d3dcompiler.h>
#include <DirectXTex.h>

#include "../Graphics/Graphics.h"

#include "../Other/Misc.h"

#include "../Graphics/NoiseTexture.h"


//---ImGui名前かぶり防止用---//
int Sprite::nameNum = 0;
//---ImGui名前かぶり防止用---//

// ----- コンストラクタ -----
Sprite::Sprite(const char* filename, const char* imguiName, const char* emissiveFilename)
{
    HRESULT result = { S_OK };
    Graphics& graphics = Graphics::Instance();

    // --- ルートシグネチャの生成 ---
#pragma region ルートシグネチャの生成
    const int MAX_DATA_NUM = 5;

    // --- ディスクリプタレンジの設定 ---
    D3D12_DESCRIPTOR_RANGE descriptorRange[MAX_DATA_NUM] = {};
    descriptorRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;    // レンジ種別
    descriptorRange[0].NumDescriptors = 1;                             // ディスクリプタ数
    descriptorRange[0].BaseShaderRegister = 0;                         // 先頭レジスタ番号
    descriptorRange[0].RegisterSpace = 0;                              // つじつまを合わせるためのスペース
    descriptorRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

    descriptorRange[1].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;    // レンジ種別
    descriptorRange[1].NumDescriptors = 1;                             // ディスクリプタ数
    descriptorRange[1].BaseShaderRegister = 1;                         // 先頭レジスタ番号
    descriptorRange[1].RegisterSpace = 0;                              // つじつまを合わせるためのスペース
    descriptorRange[1].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

    descriptorRange[2].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;    // レンジ種別
    descriptorRange[2].NumDescriptors = 1;                             // ディスクリプタ数
    descriptorRange[2].BaseShaderRegister = 2;                         // 先頭レジスタ番号
    descriptorRange[2].RegisterSpace = 0;                              // つじつまを合わせるためのスペース
    descriptorRange[2].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

    descriptorRange[3].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;    // レンジ種別
    descriptorRange[3].NumDescriptors = 1;                             // ディスクリプタ数
    descriptorRange[3].BaseShaderRegister = 0;                         // 先頭レジスタ番号
    descriptorRange[3].RegisterSpace = 0;                              // つじつまを合わせるためのスペース
    descriptorRange[3].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

    descriptorRange[4].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;    // レンジ種別
    descriptorRange[4].NumDescriptors = 1;                             // ディスクリプタ数
    descriptorRange[4].BaseShaderRegister = 3;                         // 先頭レジスタ番号
    descriptorRange[4].RegisterSpace = 0;                              // つじつまを合わせるためのスペース
    descriptorRange[4].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

    // --- ルートパラメータの設定 ---
    D3D12_ROOT_PARAMETER rootParameter[MAX_DATA_NUM] = {};
    rootParameter[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;   // パラメータ種別
    rootParameter[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;             // どのシェーダーから利用可能か
    rootParameter[0].DescriptorTable.NumDescriptorRanges = 1;                      // ディスクリプタレンジ数
    rootParameter[0].DescriptorTable.pDescriptorRanges = &descriptorRange[0];      // ディスクリプタレンジのアドレス

    rootParameter[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;   // パラメータ種別
    rootParameter[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;             // どのシェーダーから利用可能か
    rootParameter[1].DescriptorTable.NumDescriptorRanges = 1;                      // ディスクリプタレンジ数
    rootParameter[1].DescriptorTable.pDescriptorRanges = &descriptorRange[1];      // ディスクリプタレンジのアドレス

    rootParameter[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;   // パラメータ種別
    rootParameter[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;             // どのシェーダーから利用可能か
    rootParameter[2].DescriptorTable.NumDescriptorRanges = 1;                      // ディスクリプタレンジ数
    rootParameter[2].DescriptorTable.pDescriptorRanges = &descriptorRange[2];      // ディスクリプタレンジのアドレス

    rootParameter[3].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;   // パラメータ種別
    rootParameter[3].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;             // どのシェーダーから利用可能か
    rootParameter[3].DescriptorTable.NumDescriptorRanges = 1;                      // ディスクリプタレンジ数
    rootParameter[3].DescriptorTable.pDescriptorRanges = &descriptorRange[3];      // ディスクリプタレンジのアドレス

    rootParameter[4].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;   // パラメータ種別
    rootParameter[4].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;             // どのシェーダーから利用可能か
    rootParameter[4].DescriptorTable.NumDescriptorRanges = 1;                      // ディスクリプタレンジ数
    rootParameter[4].DescriptorTable.pDescriptorRanges = &descriptorRange[4];      // ディスクリプタレンジのアドレス

    // --- サンプラーの設定 ---
    D3D12_STATIC_SAMPLER_DESC samplerDesc = {};
    samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
    samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
    samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
    samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
    samplerDesc.MipLODBias = 0;
    samplerDesc.MaxAnisotropy = 0;
    samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
    samplerDesc.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
    samplerDesc.MinLOD = 0.0f;
    samplerDesc.MaxLOD = D3D12_FLOAT32_MAX;
    samplerDesc.ShaderRegister = 0;
    samplerDesc.RegisterSpace = 0;
    samplerDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

    // --- ルートシグネチャの設定 ---
    D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc = {};
    rootSignatureDesc.NumParameters = MAX_DATA_NUM;
    rootSignatureDesc.pParameters = rootParameter;
    rootSignatureDesc.NumStaticSamplers = 1;
    rootSignatureDesc.pStaticSamplers = &samplerDesc;
    rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

    // --- シリアライズ ---
    Microsoft::WRL::ComPtr<ID3DBlob> signatureBlob = nullptr;
    Microsoft::WRL::ComPtr<ID3DBlob> errorBlob = nullptr;

    result = D3D12SerializeRootSignature(
        &rootSignatureDesc,
        D3D_ROOT_SIGNATURE_VERSION_1,
        signatureBlob.GetAddressOf(),
        errorBlob.GetAddressOf()
    );
    _ASSERT_EXPR(SUCCEEDED(result), HRTrace(result));

    // --- ルートシグネチャを生成 ---
    result = graphics.GetDevice()->CreateRootSignature(
        0,
        signatureBlob->GetBufferPointer(),
        signatureBlob->GetBufferSize(),
        IID_PPV_ARGS(rootSignature_.GetAddressOf())
    );
    _ASSERT_EXPR(SUCCEEDED(result), HRTrace(result));

    rootSignature_->SetName(L"SpriteRootSignature");

#pragma endregion ルートシグネチャの生成
    

    // --- シェーダーオブジェクト作成 ---
#pragma region シェーダーオブジェクト作成
    
    // ----- VertexShader -----
    result = D3DCompileFromFile(L"./Source/Shader/SpriteVS.hlsl",
        nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
        "main", "vs_5_0",
        D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
        0, &vsBlob_, &errorBlob);

    _ASSERT_EXPR(SUCCEEDED(result), HRTrace(result));

    // ----- PixelShader -----

    // --- Default ---
    result = D3DCompileFromFile(L"./Source/Shader/SpritePS.hlsl",
        nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
        "main", "ps_5_0",
        D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
        0, &psBlob_, &errorBlob);

    _ASSERT_EXPR(SUCCEEDED(result), HRTrace(result));

    // --- Dissolve ---
    result = D3DCompileFromFile(L"./Source/Shader/SpriteDissolvePS.hlsl",
        nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
        "main", "ps_5_0",
        D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
        0, &psDissolveBlob_, &errorBlob);

    _ASSERT_EXPR(SUCCEEDED(result), HRTrace(result));

    // --- Emissive ---
    result = D3DCompileFromFile(L"./Source/Shader/SpriteEmissivePS.hlsl",
        nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
        "main", "ps_5_0",
        D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
        0, &psEmissiveBlob_, &errorBlob);

    _ASSERT_EXPR(SUCCEEDED(result), HRTrace(result));

    // --- Charge ---
    result = D3DCompileFromFile(L"./Source/Shader/SpriteChargePS.hlsl",
        nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
        "main", "ps_5_0",
        D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
        0, &psChargeBlob_, &errorBlob);

    _ASSERT_EXPR(SUCCEEDED(result), HRTrace(result));
    

#pragma endregion シェーダーオブジェクト作成

    // --- パイプラインステートの生成 ---
#pragma region パイプラインステートの生成

    UseDefaultPixelShader();

    pipelineState_->SetName(L"SpriteGraphicsPipelineState");

#pragma endregion パイプラインステートの生成


    // --- テクスチャの生成 ---
#pragma region テクスチャの生成
    // --- BaseColor ---
#pragma region baseColor
    if (filename != nullptr)
    {
        // --- テクスチャの読み込み ---
        result = Graphics::Instance().LoadTexture(filename, texture_.GetAddressOf());
        _ASSERT_EXPR(SUCCEEDED(result), HRTrace(result));
    }
    else
    {
        // --- ダミーテクスチャ作成 ---
        result = Graphics::Instance().CreateDummyTexture(texture_.GetAddressOf());
        _ASSERT_EXPR(SUCCEEDED(result), HRTrace(result));
    }

    // --- ディスクリプタの取得 ---
    shaderResourceViewDescriptor_ = Graphics::Instance().GetSrvCbvUavDescriptorHeap()->PopDescriptor();

    // --- シェーダーリソースビューの生成 ---
    D3D12_RESOURCE_DESC resourceDesc = texture_->GetDesc();

    // --- シェーダーリソースビューの設定 ---
    D3D12_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc = {};
    shaderResourceViewDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
    shaderResourceViewDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
    shaderResourceViewDesc.Format = resourceDesc.Format;
    shaderResourceViewDesc.Texture2D.MipLevels = resourceDesc.MipLevels;
    shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;

    // --- シェーダーリソースビューを生成 ---
    graphics.GetDevice()->CreateShaderResourceView(
        texture_.Get(),
        &shaderResourceViewDesc,
        shaderResourceViewDescriptor_->GetCpuHandle()
    );

    // --- テクスチャ情報の取得 ---
    transform_.SetSize(static_cast<float>(resourceDesc.Width), static_cast<float>(resourceDesc.Height));
    transform_.SetTexSize(static_cast<float>(resourceDesc.Width), static_cast<float>(resourceDesc.Height));
    transform_.SetOriginalSize({ static_cast<float>(resourceDesc.Width), static_cast<float>(resourceDesc.Height) });

#pragma endregion

    // --- Emissive ---
#pragma region Emissive
    if (emissiveFilename != nullptr)
    {
        // --- テクスチャの読み込み ---
        result = Graphics::Instance().LoadTexture(emissiveFilename, emissiveTexture_.GetAddressOf());
        _ASSERT_EXPR(SUCCEEDED(result), HRTrace(result));
    }
    else
    {
        // --- ダミーテクスチャ作成 ---
        result = Graphics::Instance().CreateDummyTexture(emissiveTexture_.GetAddressOf());
        _ASSERT_EXPR(SUCCEEDED(result), HRTrace(result));
    }

    // --- ディスクリプタの取得 ---
    emissiceShaderResourceViewDescriptor_ = Graphics::Instance().GetSrvCbvUavDescriptorHeap()->PopDescriptor();

    // --- シェーダーリソースビューの生成 ---
    resourceDesc = emissiveTexture_->GetDesc();

    // --- シェーダーリソースビューの設定 ---
    shaderResourceViewDesc = {};
    shaderResourceViewDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
    shaderResourceViewDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
    shaderResourceViewDesc.Format = resourceDesc.Format;
    shaderResourceViewDesc.Texture2D.MipLevels = resourceDesc.MipLevels;
    shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;

    // --- シェーダーリソースビューを生成 ---
    graphics.GetDevice()->CreateShaderResourceView(
        emissiveTexture_.Get(),
        &shaderResourceViewDesc,
        emissiceShaderResourceViewDescriptor_->GetCpuHandle()
    );

#pragma endregion

    // ----- 定数バッファー -----
#pragma region 定数バッファー

    D3D12_HEAP_PROPERTIES heapProp = {};
    heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;
    heapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
    heapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
    heapProp.CreationNodeMask = 1;
    heapProp.VisibleNodeMask = 1;

    D3D12_RESOURCE_DESC constantResourceDesc = {};
    constantResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
    constantResourceDesc.Alignment = 0;
    constantResourceDesc.Width = (sizeof(Dissolve) + 255) & ~255;
    constantResourceDesc.Height = 1;
    constantResourceDesc.DepthOrArraySize = 1;
    constantResourceDesc.MipLevels = 1;
    constantResourceDesc.Format = DXGI_FORMAT_UNKNOWN;
    constantResourceDesc.SampleDesc.Count = 1;
    constantResourceDesc.SampleDesc.Quality = 0;
    constantResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
    constantResourceDesc.Flags = D3D12_RESOURCE_FLAG_NONE;

    result = graphics.GetDevice()->CreateCommittedResource(
        &heapProp,
        D3D12_HEAP_FLAG_NONE,
        &constantResourceDesc,
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(dissolveConstantBuffer_.GetAddressOf())
    );
    _ASSERT_EXPR(SUCCEEDED(result), HRTrace(result));

    dissolveConstantsDescriptor_ = Graphics::Instance().GetSrvCbvUavDescriptorHeap()->PopDescriptor();

    D3D12_CONSTANT_BUFFER_VIEW_DESC constantBufferViewDesc = {};
    constantBufferViewDesc.BufferLocation = dissolveConstantBuffer_->GetGPUVirtualAddress();
    constantBufferViewDesc.SizeInBytes = static_cast<UINT>(dissolveConstantBuffer_->GetDesc().Width);

    graphics.GetDevice()->CreateConstantBufferView(
        &constantBufferViewDesc,
        dissolveConstantsDescriptor_->GetCpuHandle()
    );

#pragma endregion


#pragma endregion テクスチャの生成


    // --- フレームリソース ---
#pragma region フレームリソース
    frameResource_.resize(graphics.GetBufferCount());
    for (FrameResource& frameResource : frameResource_)
    {
        // --- 頂点バッファー生成 ---
#pragma region 頂点バッファー生成
        // --- ヒーププロパティの設定 ---
        D3D12_HEAP_PROPERTIES vertexBufferHeapProp = {};
        vertexBufferHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;
        vertexBufferHeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
        vertexBufferHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
        vertexBufferHeapProp.CreationNodeMask = 1;
        vertexBufferHeapProp.VisibleNodeMask = 1;

        // --- リソースの設定 ---
        D3D12_RESOURCE_DESC vertexBufferResourceDesc = {};
        vertexBufferResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
        vertexBufferResourceDesc.Alignment = 0;
        vertexBufferResourceDesc.Width = sizeof(Vertex) * 4;
        vertexBufferResourceDesc.Height = 1;
        vertexBufferResourceDesc.DepthOrArraySize = 1;
        vertexBufferResourceDesc.MipLevels = 1;
        vertexBufferResourceDesc.Format = DXGI_FORMAT_UNKNOWN;
        vertexBufferResourceDesc.SampleDesc.Count = 1;
        vertexBufferResourceDesc.SampleDesc.Quality = 0;
        vertexBufferResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
        vertexBufferResourceDesc.Flags = D3D12_RESOURCE_FLAG_NONE;

        // --- リソース生成 ---

        result = graphics.GetDevice()->CreateCommittedResource(
            &vertexBufferHeapProp,
            D3D12_HEAP_FLAG_NONE,
            &vertexBufferResourceDesc,
            D3D12_RESOURCE_STATE_GENERIC_READ,
            nullptr,
            IID_PPV_ARGS(frameResource.vertexBuffer_.GetAddressOf())
        );
        result = graphics.GetDevice()->GetDeviceRemovedReason();
        _ASSERT_EXPR(SUCCEEDED(result), HRTrace(result));

        frameResource.vertexBuffer_->SetName(L"SpriteVertexBuffer");

        // --- 頂点バッファービュー ---
        frameResource.vertexBufferView_.BufferLocation = frameResource.vertexBuffer_->GetGPUVirtualAddress();
        frameResource.vertexBufferView_.SizeInBytes = static_cast<UINT>(vertexBufferResourceDesc.Width);
        frameResource.vertexBufferView_.StrideInBytes = sizeof(Vertex);

        // --- マップ ---
        frameResource.vertexBuffer_->Map(0, nullptr, reinterpret_cast<void**>(&frameResource.vertexData_));

#pragma endregion


        // --- インデックスバッファーの生成 ---
#pragma region インデックスバッファーの生成
        // --- ヒーププロパティの設定 ---
        vertexBufferHeapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);

        // --- リソースの設定 ---
        vertexBufferResourceDesc = CD3DX12_RESOURCE_DESC::Buffer(sizeof(UINT) * 6);

        // --- リソース生成 ---
        result = graphics.GetDevice()->CreateCommittedResource(
            &vertexBufferHeapProp,
            D3D12_HEAP_FLAG_NONE,
            &vertexBufferResourceDesc,
            D3D12_RESOURCE_STATE_GENERIC_READ,
            nullptr,
            IID_PPV_ARGS(frameResource.indexBuffer_.GetAddressOf())
        );
        _ASSERT_EXPR(SUCCEEDED(result), HRTrace(result));

        frameResource.indexBuffer_->SetName(L"SpriteIndexBuffer");

        // --- インデックスバッファービュー設定 ---
        frameResource.indexBufferView_.BufferLocation = frameResource.indexBuffer_->GetGPUVirtualAddress();
        frameResource.indexBufferView_.SizeInBytes = static_cast<UINT>(vertexBufferResourceDesc.Width);
        frameResource.indexBufferView_.Format = DXGI_FORMAT_R32_UINT;

        // --- インデックスデータ設定 ---
        UINT* index;
        frameResource.indexBuffer_->Map(0, nullptr, reinterpret_cast<void**>(&index));

        // --- 四角形を三角形２つに展開
        // 0---1      0---1  4
        // |   |  →  |／  ／|
        // 2---3      2  3---5
        index[0] =  0;
        index[1] =  1;
        index[2] =  2;
        index[3] =  2;
        index[4] =  1;
        index[5] =  3;

        frameResource.indexBuffer_->Unmap(0, nullptr);

#pragma endregion

    }

#pragma endregion フレームリソース


    chargeConstants_ = std::make_unique<ConstantBuffer>(sizeof(ChargeConstant));

    //---ImGui名前かぶり防止用---//
    std::string space = " ";
    std::string name = imguiName + space + std::to_string(nameNum++);
    SetName(name.c_str());
    //---ImGui名前かぶり防止用---//
}

// ----- デストラクタ -----
Sprite::~Sprite()
{
    for (FrameResource& frameResource : frameResource_)
    {
        if (frameResource.vertexData_ != nullptr)
        {
            frameResource.vertexBuffer_->Unmap(0, nullptr);
        }
    }

    --nameNum;
}

// ----- 初期化 -----
void Sprite::Initialize()
{
}

// ----- 更新 -----
void Sprite::Update(const float& elapsedTime)
{
    if (isVibration_) GetTransform()->Vibration(elapsedTime);

    if (isDissolve_)
    {
        
    }
}

// ----- 描画 -----
void Sprite::Render(ID3D12GraphicsCommandList* commandList, const int& noiseTextureType)
{
    Graphics& graphics = Graphics::Instance();

    // --- スクリーンサイズを取得する ---
    float screenWidth = graphics.GetScreenWidth();
    float screenHeight = graphics.GetScrennHeight();

    // --- スプライトを構成する４頂点のスクリーン座標を計算する ---
    DirectX::XMFLOAT2 positions[] =
    {
        // 左上
        { transform_.GetPosX(), transform_.GetPosY() },
        // 右上
        { transform_.GetPosX() + transform_.GetSizeX(), transform_.GetPosY() },
        // 左下
        { transform_.GetPosX(), transform_.GetPosY() + transform_.GetSizeY() },
        // 右下
        { transform_.GetPosX() + transform_.GetSizeX(), transform_.GetPosY() + transform_.GetSizeY() },
    };

    // --- スプライトを構成する４頂点のテクスチャ座標を計算する ---
    DirectX::XMFLOAT2 texcoords[] =
    {
        // 左上
        { transform_.GetTexPosX(), transform_.GetTexPosY() },
        // 右上
        { transform_.GetTexPosX() + transform_.GetTexSizeX(), transform_.GetTexPosY() },
        // 左下
        { transform_.GetTexPosX(), transform_.GetTexPosY() + transform_.GetTexSizeY() },
        // 右下
        { transform_.GetTexPosX() + transform_.GetTexSizeX(), transform_.GetTexPosY() + transform_.GetTexSizeY() },
    };

    // --- スプライトの中心で回転させるために４頂点の中心位置が原点になるように一旦頂点を移動させる ---
    float mx = transform_.GetPosX() + transform_.GetSizeX() * 0.5f;
    float my = transform_.GetPosY() + transform_.GetSizeY() * 0.5f;
    for (auto& p : positions)
    {
        p.x -= mx;
        p.y -= my;
    }

    // --- 頂点を回転させる ---
    float theta = DirectX::XMConvertToRadians(transform_.GetAngle()); // 角度をラジアン(Θ)に変換
    float c = cosf(theta);
    float s = sinf(theta);
    for (auto& p : positions)
    {
        DirectX::XMFLOAT2 r = p;
        p.x = c * r.x + -s * r.y;
        p.y = s * r.x + c * r.y;
    }

    // --- 回転のために移動させた頂点を元の位置に戻す ---
    for (auto& p : positions)
    {
        p.x += mx;
        p.y += my;
    }

    // --- スクリーン座標系からNDC座標系へ変換する ---
    for (auto& p : positions)
    {
        p.x = 2.0f * p.x / screenWidth - 1.0f;
        p.y = 1.0f - 2.0f * p.y / screenHeight;
    }

    // --- 頂点バッファーの内容の編集する ---
    FrameResource& frameResource = frameResource_.at(graphics.GetCurrentBufferIndex());
    Vertex* v = frameResource.vertexData_;

    for (int i = 0; i < 4; ++i)
    {
        v[i].position_.x = positions[i].x;
        v[i].position_.y = positions[i].y;
        v[i].position_.z = 0.0f;

        v[i].color_.x = transform_.GetColorR();
        v[i].color_.y = transform_.GetColorG();
        v[i].color_.z = transform_.GetColorB();
        v[i].color_.w = transform_.GetColorA();

        v[i].texcoord_.x = texcoords[i].x / transform_.GetOriginalSizeX();
        v[i].texcoord_.y = texcoords[i].y / transform_.GetOriginalSizeY();
    }



    // パイプライン設定
    commandList->SetPipelineState(pipelineState_.Get());
    commandList->SetGraphicsRootSignature(rootSignature_.Get());

    // ディスクリプタテーブル
    commandList->SetGraphicsRootDescriptorTable(0, shaderResourceViewDescriptor_->GetGpuHandle());
    commandList->SetGraphicsRootDescriptorTable(1, emissiceShaderResourceViewDescriptor_->GetGpuHandle());

    NoiseTexture::Instance().PSSetShaderResourceView(commandList, noiseTextureType);

    //DissolveConstants* dissolve;
    //dissolveConstantBuffer_->Map(0, nullptr, reinterpret_cast<void**>(&dissolve));
    //dissolve->dissolveValue_ = dissolve_.GetDissolveValue();
    //dissolve->edgeThrehold_ = dissolve_.GetEdgeThrehold();
    //dissolve->edgeColor_ = dissolve_.GetEdgeColor();
    //dissolveConstantBuffer_->Unmap(0, nullptr);

    //commandList->SetGraphicsRootDescriptorTable(3, dissolveConstantsDescriptor_->GetGpuHandle());


    // 頂点バッファ設定
    commandList->IASetVertexBuffers(0, 1, &frameResource.vertexBufferView_);
    commandList->IASetIndexBuffer(&frameResource.indexBufferView_);
    commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    chargeConstants_->UpdateSubresource(&charge_);
    commandList->SetGraphicsRootDescriptorTable(4, chargeConstants_->GetDescriptor()->GetGpuHandle());

    // 描画
    commandList->DrawIndexedInstanced(6, 1, 0, 0, 0);
}

// ----- ImGui用 -----
void Sprite::DrawDebug()
{
    if (ImGui::BeginMenu(GetName()))
    {
        // --- Transform ---
        GetTransform()->DrawDebug();

        ImGui::DragFloat("charge_", &charge_.value_);

        // --- 振動 ---
        ImGui::Checkbox(reinterpret_cast<const char*>(u8"振動"), &isVibration_);
        if (isVibration_) GetVibration()->DrawDebug();

        // --- ディゾルブ ---
        ImGui::Checkbox(reinterpret_cast<const char*>(u8"ディゾルブ"), &isDissolve_);
        if (isDissolve_) dissolve_.DrawDebug();

        // --- PixelShader切り替え ---       
        if (ImGui::BeginCombo(reinterpret_cast<const char*>(u8"ピクセルシェーダー"), currentItem))
        {
            for (int index = 0; index < 3; ++index)
            {
                if (ImGui::Selectable(list[index].c_str()))
                {
                    currentItem = list[index].c_str();
                    (this->*usePSFunc[index])();
                }
            }

            ImGui::EndCombo();
        }

        ImGui::EndMenu();
    }
}

void Sprite::UseDefaultPixelShader()
{
    HRESULT result = S_OK;

    Graphics::Instance().WaitIdle();

    // --- ルートシグネチャ ---
    graphicsPipeline_.pRootSignature = rootSignature_.Get();

    // --- シェーダー ---
    graphicsPipeline_.VS.pShaderBytecode = vsBlob_->GetBufferPointer();
    graphicsPipeline_.VS.BytecodeLength = vsBlob_->GetBufferSize();
    graphicsPipeline_.PS.pShaderBytecode = psBlob_->GetBufferPointer();
    graphicsPipeline_.PS.BytecodeLength  = psBlob_->GetBufferSize();

    // --- 入力レイアウト ---
    D3D12_INPUT_ELEMENT_DESC inputElementDesc[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
        { "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
    };

    graphicsPipeline_.InputLayout.pInputElementDescs = inputElementDesc;
    graphicsPipeline_.InputLayout.NumElements = _countof(inputElementDesc);

    // --- ブレンドステート ---
    graphicsPipeline_.BlendState.AlphaToCoverageEnable = false;
    graphicsPipeline_.BlendState.IndependentBlendEnable = false;
    graphicsPipeline_.BlendState.RenderTarget[0].BlendEnable = true;
    graphicsPipeline_.BlendState.RenderTarget[0].LogicOpEnable = false;
    graphicsPipeline_.BlendState.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
    graphicsPipeline_.BlendState.RenderTarget[0].DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
    graphicsPipeline_.BlendState.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
    graphicsPipeline_.BlendState.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;
    graphicsPipeline_.BlendState.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;
    graphicsPipeline_.BlendState.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
    graphicsPipeline_.BlendState.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

    // --- 深度ステンシルステート ---
    graphicsPipeline_.DepthStencilState.DepthEnable = false;
    graphicsPipeline_.DepthStencilState.StencilEnable = false;

    // --- ラスタライザーステート ---
    graphicsPipeline_.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;
    graphicsPipeline_.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;
    graphicsPipeline_.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
    graphicsPipeline_.RasterizerState.FrontCounterClockwise = false;
    graphicsPipeline_.RasterizerState.DepthBias = D3D12_DEFAULT_DEPTH_BIAS;
    graphicsPipeline_.RasterizerState.DepthBiasClamp = D3D12_DEFAULT_DEPTH_BIAS_CLAMP;
    graphicsPipeline_.RasterizerState.SlopeScaledDepthBias = D3D12_DEFAULT_SLOPE_SCALED_DEPTH_BIAS;
    graphicsPipeline_.RasterizerState.DepthClipEnable = true;
    graphicsPipeline_.RasterizerState.MultisampleEnable = false;
    graphicsPipeline_.RasterizerState.AntialiasedLineEnable = false;
    graphicsPipeline_.RasterizerState.ForcedSampleCount = 0;
    graphicsPipeline_.RasterizerState.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;

    // --- プリミティブトポロジー ---
    graphicsPipeline_.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

    // --- ストリップ時のカット値 ---
    graphicsPipeline_.IBStripCutValue = D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_DISABLED;

    // --- レンダーターゲット数 ---
    graphicsPipeline_.NumRenderTargets = 1;
    graphicsPipeline_.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
    graphicsPipeline_.DSVFormat = DXGI_FORMAT_D32_FLOAT;

    // --- マルチサンプリング ---
    graphicsPipeline_.SampleDesc.Count = 1;
    graphicsPipeline_.SampleDesc.Quality = 0;

    // --- アダプタ ---
    graphicsPipeline_.NodeMask = 0;

    result = Graphics::Instance().GetDevice()->CreateGraphicsPipelineState(
        &graphicsPipeline_,
        IID_PPV_ARGS(&pipelineState_)
    );
    _ASSERT_EXPR(SUCCEEDED(result), HRTrace(result));
}

void Sprite::UseDissolvePixelShader()
{
    HRESULT result = S_OK;

    Graphics::Instance().WaitIdle();

    // --- ルートシグネチャ ---
    graphicsPipeline_.pRootSignature = rootSignature_.Get();

    // --- シェーダー ---
    graphicsPipeline_.VS.pShaderBytecode = vsBlob_->GetBufferPointer();
    graphicsPipeline_.VS.BytecodeLength  = vsBlob_->GetBufferSize();

    graphicsPipeline_.PS.pShaderBytecode = psDissolveBlob_->GetBufferPointer();
    graphicsPipeline_.PS.BytecodeLength  = psDissolveBlob_->GetBufferSize();

    // --- 入力レイアウト ---
    D3D12_INPUT_ELEMENT_DESC inputElementDesc[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
        { "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
    };

    graphicsPipeline_.InputLayout.pInputElementDescs = inputElementDesc;
    graphicsPipeline_.InputLayout.NumElements = _countof(inputElementDesc);

    // --- ブレンドステート ---
    graphicsPipeline_.BlendState.AlphaToCoverageEnable = false;
    graphicsPipeline_.BlendState.IndependentBlendEnable = false;
    graphicsPipeline_.BlendState.RenderTarget[0].BlendEnable = true;
    graphicsPipeline_.BlendState.RenderTarget[0].LogicOpEnable = false;
    graphicsPipeline_.BlendState.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
    graphicsPipeline_.BlendState.RenderTarget[0].DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
    graphicsPipeline_.BlendState.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
    graphicsPipeline_.BlendState.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;
    graphicsPipeline_.BlendState.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;
    graphicsPipeline_.BlendState.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
    graphicsPipeline_.BlendState.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

    // --- 深度ステンシルステート ---
    graphicsPipeline_.DepthStencilState.DepthEnable = true;
    graphicsPipeline_.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
    graphicsPipeline_.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_ALWAYS;
    graphicsPipeline_.DepthStencilState.StencilEnable = false;

    // --- ラスタライザーステート ---
    graphicsPipeline_.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;
    graphicsPipeline_.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;
    graphicsPipeline_.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
    graphicsPipeline_.RasterizerState.FrontCounterClockwise = false;
    graphicsPipeline_.RasterizerState.DepthBias = D3D12_DEFAULT_DEPTH_BIAS;
    graphicsPipeline_.RasterizerState.DepthBiasClamp = D3D12_DEFAULT_DEPTH_BIAS_CLAMP;
    graphicsPipeline_.RasterizerState.SlopeScaledDepthBias = D3D12_DEFAULT_SLOPE_SCALED_DEPTH_BIAS;
    graphicsPipeline_.RasterizerState.DepthClipEnable = true;
    graphicsPipeline_.RasterizerState.MultisampleEnable = false;
    graphicsPipeline_.RasterizerState.AntialiasedLineEnable = false;
    graphicsPipeline_.RasterizerState.ForcedSampleCount = 0;
    graphicsPipeline_.RasterizerState.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;

    // --- プリミティブトポロジー ---
    graphicsPipeline_.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

    // --- ストリップ時のカット値 ---
    graphicsPipeline_.IBStripCutValue = D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_DISABLED;

    // --- レンダーターゲット数 ---
    graphicsPipeline_.NumRenderTargets = 1;
    graphicsPipeline_.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
    graphicsPipeline_.DSVFormat = DXGI_FORMAT_D32_FLOAT;

    // --- マルチサンプリング ---
    graphicsPipeline_.SampleDesc.Count = 1;
    graphicsPipeline_.SampleDesc.Quality = 0;

    // --- アダプタ ---
    graphicsPipeline_.NodeMask = 0;

    result = Graphics::Instance().GetDevice()->CreateGraphicsPipelineState(
        &graphicsPipeline_,
        IID_PPV_ARGS(&pipelineState_)
    );
    _ASSERT_EXPR(SUCCEEDED(result), HRTrace(result));
}

void Sprite::UseEmissivePixelShader()
{
    HRESULT result = S_OK;

    Graphics::Instance().WaitIdle();

    // --- ルートシグネチャ ---
    graphicsPipeline_.pRootSignature = rootSignature_.Get();

    // --- シェーダー ---
    graphicsPipeline_.VS.pShaderBytecode = vsBlob_->GetBufferPointer();
    graphicsPipeline_.VS.BytecodeLength  = vsBlob_->GetBufferSize();

    graphicsPipeline_.PS.pShaderBytecode = psEmissiveBlob_->GetBufferPointer();
    graphicsPipeline_.PS.BytecodeLength  = psEmissiveBlob_->GetBufferSize();

    // --- 入力レイアウト ---
    D3D12_INPUT_ELEMENT_DESC inputElementDesc[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
        { "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
    };

    graphicsPipeline_.InputLayout.pInputElementDescs = inputElementDesc;
    graphicsPipeline_.InputLayout.NumElements = _countof(inputElementDesc);

    // --- ブレンドステート ---
    graphicsPipeline_.BlendState.AlphaToCoverageEnable = false;
    graphicsPipeline_.BlendState.IndependentBlendEnable = false;
    graphicsPipeline_.BlendState.RenderTarget[0].BlendEnable = true;
    graphicsPipeline_.BlendState.RenderTarget[0].LogicOpEnable = false;
    graphicsPipeline_.BlendState.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
    graphicsPipeline_.BlendState.RenderTarget[0].DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
    graphicsPipeline_.BlendState.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
    graphicsPipeline_.BlendState.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;
    graphicsPipeline_.BlendState.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;
    graphicsPipeline_.BlendState.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
    graphicsPipeline_.BlendState.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

    // --- 深度ステンシルステート ---
    graphicsPipeline_.DepthStencilState.DepthEnable = true;
    graphicsPipeline_.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
    graphicsPipeline_.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_ALWAYS;
    graphicsPipeline_.DepthStencilState.StencilEnable = false;

    // --- ラスタライザーステート ---
    graphicsPipeline_.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;
    graphicsPipeline_.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;
    graphicsPipeline_.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
    graphicsPipeline_.RasterizerState.FrontCounterClockwise = false;
    graphicsPipeline_.RasterizerState.DepthBias = D3D12_DEFAULT_DEPTH_BIAS;
    graphicsPipeline_.RasterizerState.DepthBiasClamp = D3D12_DEFAULT_DEPTH_BIAS_CLAMP;
    graphicsPipeline_.RasterizerState.SlopeScaledDepthBias = D3D12_DEFAULT_SLOPE_SCALED_DEPTH_BIAS;
    graphicsPipeline_.RasterizerState.DepthClipEnable = true;
    graphicsPipeline_.RasterizerState.MultisampleEnable = false;
    graphicsPipeline_.RasterizerState.AntialiasedLineEnable = false;
    graphicsPipeline_.RasterizerState.ForcedSampleCount = 0;
    graphicsPipeline_.RasterizerState.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;

    // --- プリミティブトポロジー ---
    graphicsPipeline_.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

    // --- ストリップ時のカット値 ---
    graphicsPipeline_.IBStripCutValue = D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_DISABLED;

    // --- レンダーターゲット数 ---
    graphicsPipeline_.NumRenderTargets = 1;
    graphicsPipeline_.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
    graphicsPipeline_.DSVFormat = DXGI_FORMAT_D32_FLOAT;

    // --- マルチサンプリング ---
    graphicsPipeline_.SampleDesc.Count = 1;
    graphicsPipeline_.SampleDesc.Quality = 0;

    // --- アダプタ ---
    graphicsPipeline_.NodeMask = 0;

    result = Graphics::Instance().GetDevice()->CreateGraphicsPipelineState(
        &graphicsPipeline_,
        IID_PPV_ARGS(&pipelineState_)
    );
    _ASSERT_EXPR(SUCCEEDED(result), HRTrace(result));

}

void Sprite::UseChargePixelShader()
{
    HRESULT result = S_OK;

    Graphics::Instance().WaitIdle();

    // --- ルートシグネチャ ---
    graphicsPipeline_.pRootSignature = rootSignature_.Get();

    // --- シェーダー ---
    graphicsPipeline_.VS.pShaderBytecode = vsBlob_->GetBufferPointer();
    graphicsPipeline_.VS.BytecodeLength = vsBlob_->GetBufferSize();

    graphicsPipeline_.PS.pShaderBytecode = psChargeBlob_->GetBufferPointer();
    graphicsPipeline_.PS.BytecodeLength = psChargeBlob_->GetBufferSize();

    // --- 入力レイアウト ---
    D3D12_INPUT_ELEMENT_DESC inputElementDesc[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
        { "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
    };

    graphicsPipeline_.InputLayout.pInputElementDescs = inputElementDesc;
    graphicsPipeline_.InputLayout.NumElements = _countof(inputElementDesc);

    // --- ブレンドステート ---
    graphicsPipeline_.BlendState.AlphaToCoverageEnable = false;
    graphicsPipeline_.BlendState.IndependentBlendEnable = false;
    graphicsPipeline_.BlendState.RenderTarget[0].BlendEnable = true;
    graphicsPipeline_.BlendState.RenderTarget[0].LogicOpEnable = false;
    graphicsPipeline_.BlendState.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
    graphicsPipeline_.BlendState.RenderTarget[0].DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
    graphicsPipeline_.BlendState.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
    graphicsPipeline_.BlendState.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;
    graphicsPipeline_.BlendState.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;
    graphicsPipeline_.BlendState.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
    graphicsPipeline_.BlendState.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

    // --- 深度ステンシルステート ---
    graphicsPipeline_.DepthStencilState.DepthEnable = true;
    graphicsPipeline_.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
    graphicsPipeline_.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_ALWAYS;
    graphicsPipeline_.DepthStencilState.StencilEnable = false;

    // --- ラスタライザーステート ---
    graphicsPipeline_.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;
    graphicsPipeline_.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;
    graphicsPipeline_.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
    graphicsPipeline_.RasterizerState.FrontCounterClockwise = false;
    graphicsPipeline_.RasterizerState.DepthBias = D3D12_DEFAULT_DEPTH_BIAS;
    graphicsPipeline_.RasterizerState.DepthBiasClamp = D3D12_DEFAULT_DEPTH_BIAS_CLAMP;
    graphicsPipeline_.RasterizerState.SlopeScaledDepthBias = D3D12_DEFAULT_SLOPE_SCALED_DEPTH_BIAS;
    graphicsPipeline_.RasterizerState.DepthClipEnable = true;
    graphicsPipeline_.RasterizerState.MultisampleEnable = false;
    graphicsPipeline_.RasterizerState.AntialiasedLineEnable = false;
    graphicsPipeline_.RasterizerState.ForcedSampleCount = 0;
    graphicsPipeline_.RasterizerState.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;

    // --- プリミティブトポロジー ---
    graphicsPipeline_.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

    // --- ストリップ時のカット値 ---
    graphicsPipeline_.IBStripCutValue = D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_DISABLED;

    // --- レンダーターゲット数 ---
    graphicsPipeline_.NumRenderTargets = 1;
    graphicsPipeline_.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
    graphicsPipeline_.DSVFormat = DXGI_FORMAT_D32_FLOAT;

    // --- マルチサンプリング ---
    graphicsPipeline_.SampleDesc.Count = 1;
    graphicsPipeline_.SampleDesc.Quality = 0;

    // --- アダプタ ---
    graphicsPipeline_.NodeMask = 0;

    result = Graphics::Instance().GetDevice()->CreateGraphicsPipelineState(
        &graphicsPipeline_,
        IID_PPV_ARGS(&pipelineState_)
    );
    _ASSERT_EXPR(SUCCEEDED(result), HRTrace(result));
}

// ----- Sprite::Transform ImGui用 -----
void Sprite::Transform::DrawDebug()
{
    if (ImGui::TreeNode("Transform"))
    {
        ImGui::DragFloat2(reinterpret_cast<const char*>(u8"position"), &position_.x);
        ImGui::DragFloat2(reinterpret_cast<const char*>(u8"size"), &size_.x);

        if (ImGui::Button(reinterpret_cast<const char*>(u8"sizeFactor使用する")))
        {
            isSizeFactor_ = !isSizeFactor_;
        }
        if (isSizeFactor_)
        {
            float sizeFactor = size_.x;
            ImGui::DragFloat("sizeFactor", &sizeFactor);
            SetSize(sizeFactor);
        }

        ImGui::ColorEdit4(reinterpret_cast<const char*>(u8"color"), &color_.x);
        ImGui::DragFloat(reinterpret_cast<const char*>(u8"angle"), &angle_);
        ImGui::DragFloat2(reinterpret_cast<const char*>(u8"texPos"), &texPos_.x);
        ImGui::DragFloat2(reinterpret_cast<const char*>(u8"texSize"), &texSize_.x);

        ImGui::TreePop();
    }
}

// ----- 振動 -----
void Sprite::Transform::Vibration(const float& elapsedTime)
{
    if (GetVibration()->GetVolume() <= 0) return; // 振動値がない場合は処理しない

    // 振動の間の時間を空けるための処理
    if (GetVibration()->GetVibrationBreakTimer() >= 0.0f)
    {   
        // breakTimeが残っているときは処理しない
        GetVibration()->SubtractVibrationBreakTimer(elapsedTime);

        return;
    }

    // breakTimeを設定
    GetVibration()->SetVibrationBreakTimer(GetVibration()->GetBreakTime());

    // 前回の結果をリセットする
    SubtractPos(GetVibration()->GetOldVibration());

    // 振動の幅、方向を算出する
    DirectX::XMFLOAT2 vibration = { (rand() % 100 - 50.0f), (rand() % 100 - 50.0f) };
    DirectX::XMVECTOR Vibration = DirectX::XMVector2Normalize(DirectX::XMLoadFloat2(&vibration));

    float vibrationVolume = GetVibration()->GetVolume() * 1000 * elapsedTime;

    Vibration = DirectX::XMVectorScale(Vibration, vibrationVolume);
    DirectX::XMStoreFloat2(&vibration, Vibration);

    // 振動の値を入れる。
    AddPos(vibration);

    // 現在の振動値を保存しておく
    GetVibration()->SetOldVibration(vibration);
}

// ----- Sprite::VibrationTransfom ImGui用 -----
void Sprite::VibrationTransform::DrawDebug()
{
    if (ImGui::TreeNode(reinterpret_cast<const char*>(u8"振動")))
    {
        ImGui::DragFloat(reinterpret_cast<const char*>(u8"振動の振幅"), &volume_);
        ImGui::DragFloat(reinterpret_cast<const char*>(u8"次の振動までの時間"), &breakTime_);
        ImGui::InputFloat(reinterpret_cast<const char*>(u8"タイマー"), &vibrationBreakTimer_);

        ImGui::TreePop();
    }
}

// ----- Sprite::Dissolve ImGui用 -----
void Sprite::Dissolve::DrawDebug()
{
    if (ImGui::TreeNode(reinterpret_cast<const char*>(u8"ディゾルブ")))
    {
        ImGui::SliderFloat(reinterpret_cast<const char*>(u8"適応値"), &dissolveValue_, 0.0f, 1.0f);
        ImGui::DragFloat(reinterpret_cast<const char*>(u8"縁の閾値"), &edgeThrehold_);
        ImGui::ColorEdit4(reinterpret_cast<const char*>(u8"縁の色"), &edgeColor_.x);

        ImGui::TreePop();
    }
}
