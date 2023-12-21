#include "SkyMap.h"

#include <d3dcompiler.h>

#include "Graphics.h"
#include "Camera.h"
#include "../Other/Misc.h"

// --- コンストラクタ ---
SkyMap::SkyMap()
{
    HRESULT result = S_OK;

    texture_ = std::make_unique<Texture>(L"./Resources/Image/SkyMap/Sky.png");

    // ----- ルートシグネチャの生成 ---
#pragma region ルートシグネチャの生成
    const static int MAX_DESCRIPTOR = 2;

    // --- ディスクリプタレンジの設定 ---
    D3D12_DESCRIPTOR_RANGE descriptorRange[MAX_DESCRIPTOR] = {};

    descriptorRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV; // レンジ種別
    descriptorRange[0].NumDescriptors = 1;                          // ディスクリプタ数
    descriptorRange[0].BaseShaderRegister = 0;                      // 先頭レジスタ番号
    descriptorRange[0].RegisterSpace = 0;                           // つじつまを合わせるためのスペース
    descriptorRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

    descriptorRange[1].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV; // レンジ種別
    descriptorRange[1].NumDescriptors = 1;                          // ディスクリプタ数
    descriptorRange[1].BaseShaderRegister = 0;                      // 先頭レジスタ番号
    descriptorRange[1].RegisterSpace = 0;                           // つじつまを合わせるためのスペース
    descriptorRange[1].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

    // --- ルートパラメータの設定 ---
    D3D12_ROOT_PARAMETER rootParameter[MAX_DESCRIPTOR] = {};

    rootParameter[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
    rootParameter[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
    rootParameter[0].DescriptorTable.NumDescriptorRanges = 1;
    rootParameter[0].DescriptorTable.pDescriptorRanges = &descriptorRange[0];
    
    rootParameter[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
    rootParameter[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
    rootParameter[1].DescriptorTable.NumDescriptorRanges = 1;
    rootParameter[1].DescriptorTable.pDescriptorRanges = &descriptorRange[1];

    // --- サンプラーの設定 ---
    D3D12_STATIC_SAMPLER_DESC samplerDesc[1] = {};

    samplerDesc[0].Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
    samplerDesc[0].AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
    samplerDesc[0].AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
    samplerDesc[0].AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
    samplerDesc[0].MipLODBias = 0;
    samplerDesc[0].MaxAnisotropy = 0;
    samplerDesc[0].ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
    samplerDesc[0].BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
    samplerDesc[0].MinLOD = 0.0f;
    samplerDesc[0].MaxLOD = D3D12_FLOAT32_MAX;
    samplerDesc[0].ShaderRegister = 0;
    samplerDesc[0].RegisterSpace = 0;
    samplerDesc[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

    // --- ルートシグネチャの設定 ---
    D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc = {};
    rootSignatureDesc.NumParameters = _countof(rootParameter);
    rootSignatureDesc.pParameters = rootParameter;
    rootSignatureDesc.NumStaticSamplers = _countof(samplerDesc);
    rootSignatureDesc.pStaticSamplers = samplerDesc;
    rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

    // --- シリアライズ ---
    Microsoft::WRL::ComPtr<ID3DBlob> signatureBlob;
    Microsoft::WRL::ComPtr<ID3DBlob> errorBlob;

    result = D3D12SerializeRootSignature(
        &rootSignatureDesc,
        D3D_ROOT_SIGNATURE_VERSION_1,
        signatureBlob.GetAddressOf(),
        errorBlob.GetAddressOf()
    );
    _ASSERT_EXPR(SUCCEEDED(result), HRTrace(result));

    // --- ルートシグネチャの生成 ---
    result = Graphics::Instance().GetDevice()->CreateRootSignature(
        0,
        signatureBlob->GetBufferPointer(),
        signatureBlob->GetBufferSize(),
        IID_PPV_ARGS(rootSignature_.GetAddressOf())
    );
    _ASSERT_EXPR(SUCCEEDED(result), HRTrace(result));

#pragma endregion ルートシグネチャの生成

    // ----- Shader -----
    Microsoft::WRL::ComPtr<ID3DBlob> vertexShader;
    Microsoft::WRL::ComPtr<ID3DBlob> pixelShader;

    result = D3DCompileFromFile(L"./Source/Shader/SkyMapVS.hlsl",
        nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
        "main", "vs_5_0",
        D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
        0, &vertexShader, &errorBlob);

    _ASSERT_EXPR(SUCCEEDED(result), HRTrace(result));

    result = D3DCompileFromFile(L"./Source/Shader/SkyMapPS.hlsl",
        nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
        "main", "ps_5_0",
        D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
        0, &pixelShader, &errorBlob);

    _ASSERT_EXPR(SUCCEEDED(result), HRTrace(result));

    // ----- パイプラインステートの設定 -----
    D3D12_INPUT_ELEMENT_DESC inputElementDescs[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,       0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
    };

    pipelineState_ = std::make_unique<PipelineState>(
        rootSignature_.Get(), vertexShader.Get(), pixelShader.Get(),
        inputElementDescs, _countof(inputElementDescs),
        false, false, false,
        D3D12_FILL_MODE_SOLID, D3D12_CULL_MODE_NONE
    );

    // --- SceneConstantBuffer ---
    sceneConstantBuffer_ = std::make_unique<ConstantBuffer>(sizeof(SceneConstants));

    // --- VertexBuffer ---
    vertexBuffer_ = std::make_unique<VertexBuffer>(&vertexData_, 4, sizeof(Vertex));

    // --- IndexBuffer ---
    UINT index[6] = {0, 1, 2, 2, 1, 3};
    indexBuffer_ = std::make_unique<IndexBuffer>(index, 6, sizeof(UINT));
}

// --- 描画 ---
void SkyMap::Render(ID3D12GraphicsCommandList* commandList)
{
    Camera& camera = Camera::Instance();
    SceneConstants sceneConstants;

    // --- パイプライン設定 ---
    pipelineState_->SetPipelineState(commandList);
    commandList->SetGraphicsRootSignature(rootSignature_.Get());

    // --- 頂点バッファー設定 ---
    commandList->IASetVertexBuffers(0, 1, vertexBuffer_->GetVertexBufferView());
    commandList->IASetIndexBuffer(indexBuffer_->GetIndexBufferView());
    commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    

    DirectX::XMStoreFloat4x4(&sceneConstants.viewProjection_, camera.GetViewMatrix() * camera.GetProjectionMatrix());
    sceneConstants.lightDirection_ = { 0, 0, 1, 1 };
    sceneConstants.cameraPostiion_ = { 0, 0, 0, 1 };
    DirectX::XMStoreFloat4x4(&sceneConstants.invViewProjection_, DirectX::XMMatrixInverse(NULL, camera.GetViewMatrix() * camera.GetProjectionMatrix()));

    sceneConstantBuffer_->UpdateSubresource(&sceneConstants);
    commandList->SetGraphicsRootDescriptorTable(0, sceneConstantBuffer_->GetDescriptor()->GetGpuHandle());
    commandList->SetGraphicsRootDescriptorTable(1, texture_->GetDescriptor()->GetGpuHandle());

    commandList->DrawIndexedInstanced(6, 1, 0, 0, 0);
}
