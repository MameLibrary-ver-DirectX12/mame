#include "LambertShader.h"

#include <d3dcompiler.h>

#include "Graphics.h"
#include "Camera.h"

#include "../Other/Misc.h"

// --- コンストラクタ ---
LambertShader::LambertShader()
{
    HRESULT result = S_OK;

    // ----- ルートシグネチャの生成 -----
#pragma region ルートシグネチャの生成
    const static int MAX_DESCRIPTOR = 4;

    // --- ディスクリプタレンジの設定 ---
    D3D12_DESCRIPTOR_RANGE descriptorRange[MAX_DESCRIPTOR] = {};

    descriptorRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;    // レンジ種別
    descriptorRange[0].NumDescriptors = 1;                             // ディスクリプタ数
    descriptorRange[0].BaseShaderRegister = 0;                         // 先頭レジスタ番号
    descriptorRange[0].RegisterSpace = 0;                              // つじつまを合わせるためのスペース
    descriptorRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	descriptorRange[1].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;    // レンジ種別
	descriptorRange[1].NumDescriptors = 1;                             // ディスクリプタ数
	descriptorRange[1].BaseShaderRegister = 1;                         // 先頭レジスタ番号
	descriptorRange[1].RegisterSpace = 0;                              // つじつまを合わせるためのスペース
	descriptorRange[1].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

    descriptorRange[2].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;    // レンジ種別
    descriptorRange[2].NumDescriptors = 1;                             // ディスクリプタ数
    descriptorRange[2].BaseShaderRegister = 0;                         // 先頭レジスタ番号
    descriptorRange[2].RegisterSpace = 0;                              // つじつまを合わせるためのスペース
    descriptorRange[2].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

    descriptorRange[3].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;    // レンジ種別
    descriptorRange[3].NumDescriptors = 1;                             // ディスクリプタ数
    descriptorRange[3].BaseShaderRegister = 1;                         // 先頭レジスタ番号
    descriptorRange[3].RegisterSpace = 0;                              // つじつまを合わせるためのスペース
    descriptorRange[3].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

    // --- ルートパラメータの設定 ---
    D3D12_ROOT_PARAMETER rootParameter[MAX_DESCRIPTOR] = {};

    rootParameter[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
    rootParameter[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
    rootParameter[0].DescriptorTable.NumDescriptorRanges = 1;
    rootParameter[0].DescriptorTable.pDescriptorRanges = &descriptorRange[0];

	rootParameter[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootParameter[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	rootParameter[1].DescriptorTable.NumDescriptorRanges = 1;
	rootParameter[1].DescriptorTable.pDescriptorRanges = &descriptorRange[1];

    rootParameter[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
    rootParameter[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
    rootParameter[2].DescriptorTable.NumDescriptorRanges = 1;
    rootParameter[2].DescriptorTable.pDescriptorRanges = &descriptorRange[2];

    rootParameter[3].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
    rootParameter[3].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
    rootParameter[3].DescriptorTable.NumDescriptorRanges = 1;
    rootParameter[3].DescriptorTable.pDescriptorRanges = &descriptorRange[3];

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

#pragma endregion
	
	// ----- VertexShader -----
	result = D3DCompileFromFile(L"./Source/Shader/FbxModelVS.hlsl",
		nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"main", "vs_5_0",
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
		0, &vertexShader_, &errorBlob);

	_ASSERT_EXPR(SUCCEEDED(result), HRTrace(result));

	// ----- PixelShader -----
	result = D3DCompileFromFile(L"./Source/Shader/FbxModelPS.hlsl",
		nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"main", "ps_5_0",
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
		0, &pixelShader_, &errorBlob);

	_ASSERT_EXPR(SUCCEEDED(result), HRTrace(result));

	// ----- パイプラインステートの生成 -----
#pragma region パイプラインステートの生成
        // --- 入力レイアウト ---
    D3D12_INPUT_ELEMENT_DESC inputElementDescs[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
        { "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
        { "TANGENT",  0, DXGI_FORMAT_R32G32B32A32_FLOAT,    0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,       0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
        { "WEIGHTS",  0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
        { "BONES",    0, DXGI_FORMAT_R32G32B32A32_UINT,  0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
    };

    pipelineState_ = std::make_unique<PipelineState>(
        rootSignature_.Get(), vertexShader_.Get(), pixelShader_.Get(),
        inputElementDescs, _countof(inputElementDescs),
        false, false, true,
        D3D12_FILL_MODE_SOLID, D3D12_CULL_MODE_BACK
    );

#pragma endregion

    sceneConstantBuffer_ = std::make_unique<ConstantBuffer>(sizeof(Graphics::SceneConstants));
}

// --- ランバートシェーダーを使う ---
void LambertShader::SetLambertShader(ID3D12GraphicsCommandList* commandList)
{
    Camera& camera = Camera::Instance();
    Graphics::SceneConstants sceneConstants;

    DirectX::XMStoreFloat4x4(&sceneConstants.viewProjection_, camera.GetViewMatrix() * camera.GetProjectionMatrix());
    sceneConstants.lightDirection_ = { 0, 0, 1, 1 };

	// パイプライン設定
    pipelineState_->SetPipelineState(commandList);
	commandList->SetGraphicsRootSignature(rootSignature_.Get());

	// シーン定数バッファ設定
    sceneConstantBuffer_->UpdateSubresource(&sceneConstants);
    commandList->SetGraphicsRootDescriptorTable(0, sceneConstantBuffer_->GetDescriptor()->GetGpuHandle());
}
