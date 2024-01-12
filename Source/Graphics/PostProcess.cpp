#include "PostProcess.h"
#include "Graphics.h"
#include "../Other/Misc.h"

#include <d3dcompiler.h>

// --- �R���X�g���N�^ ---
PostProcess::PostProcess()
{
    HRESULT result = S_OK;

    const int MAX_DATA_NUM = 1;

    // �f�B�X�N���v�^�����W�̐ݒ�
    D3D12_DESCRIPTOR_RANGE descriptorRange[MAX_DATA_NUM] = {};
    descriptorRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV; // �����W���
    descriptorRange[0].NumDescriptors = 1;                          // �f�B�X�N���v�^��
    descriptorRange[0].BaseShaderRegister = 0;                      // �擪���W�X�^�ԍ�
    descriptorRange[0].RegisterSpace = 0;                           // ���܂����킹�邽�߂̃X�y�[�X
    descriptorRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

    // ���[�g�p�����[�^�[�̐ݒ�
    D3D12_ROOT_PARAMETER rootParameter[MAX_DATA_NUM] = {};
    rootParameter[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;   // �p�����[�^���
    rootParameter[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;             // �ǂ̃V�F�[�_�[���痘�p�\��
    rootParameter[0].DescriptorTable.NumDescriptorRanges = 1;                      // �f�B�X�N���v�^�����W��
    rootParameter[0].DescriptorTable.pDescriptorRanges = &descriptorRange[0];      // �f�B�X�N���v�^�����W�̃A�h���X

    // �T���v���[�̐ݒ� 
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

    // ���[�g�V�O�l�`���̐ݒ� 
    D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc = {};
    rootSignatureDesc.NumParameters = MAX_DATA_NUM;
    rootSignatureDesc.pParameters = rootParameter;
    rootSignatureDesc.NumStaticSamplers = 1;
    rootSignatureDesc.pStaticSamplers = &samplerDesc;
    rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

    //  �V���A���C�Y 
    Microsoft::WRL::ComPtr<ID3DBlob> signatureBlob = nullptr;
    Microsoft::WRL::ComPtr<ID3DBlob> errorBlob = nullptr;

    result = D3D12SerializeRootSignature(
        &rootSignatureDesc,
        D3D_ROOT_SIGNATURE_VERSION_1,
        signatureBlob.GetAddressOf(),
        errorBlob.GetAddressOf()
    );
    _ASSERT_EXPR(SUCCEEDED(result), HRTrace(result));

    // ���[�g�V�O�l�`���𐶐� 
    result = Graphics::Instance().GetDevice()->CreateRootSignature(
        0,
        signatureBlob->GetBufferPointer(),
        signatureBlob->GetBufferSize(),
        IID_PPV_ARGS(rootSignature_.GetAddressOf())
    );
    _ASSERT_EXPR(SUCCEEDED(result), HRTrace(result));

    rootSignature_->SetName(L"PostProcessRottSignature");

    // �V�F�[�_�[�I�u�W�F�N�g����
    
    // ----- VertexShader -----
    result = D3DCompileFromFile(L"./Source/Shader/PostProcessVS.hlsl",
        nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
        "main", "vs_5_0",
        D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
        0, &vsBlob_, &errorBlob);

    _ASSERT_EXPR(SUCCEEDED(result), HRTrace(result));

    // ----- PixelShader -----
    result = D3DCompileFromFile(L"./Source/Shader/PostProcessPS.hlsl",
        nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
        "main", "ps_5_0",
        D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
        0, &psBlob_, &errorBlob);
    _ASSERT_EXPR(SUCCEEDED(result), HRTrace(result));

    Graphics::Instance().WaitIdle();

    // --- ���[�g�V�O�l�`�� ---
    graphicsPipeline_.pRootSignature = rootSignature_.Get();

    // --- �V�F�[�_�[ ---
    graphicsPipeline_.VS.pShaderBytecode = vsBlob_->GetBufferPointer();
    graphicsPipeline_.VS.BytecodeLength  = vsBlob_->GetBufferSize();
    graphicsPipeline_.PS.pShaderBytecode = psBlob_->GetBufferPointer();
    graphicsPipeline_.PS.BytecodeLength  = psBlob_->GetBufferSize();

    // --- ���̓��C�A�E�g ---
    D3D12_INPUT_ELEMENT_DESC inputElementDesc[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
        { "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
    };

    graphicsPipeline_.InputLayout.pInputElementDescs = inputElementDesc;
    graphicsPipeline_.InputLayout.NumElements = _countof(inputElementDesc);

    // --- �u�����h�X�e�[�g ---
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

    // --- �[�x�X�e���V���X�e�[�g ---
    graphicsPipeline_.DepthStencilState.DepthEnable = false;
    graphicsPipeline_.DepthStencilState.StencilEnable = false;

    // --- ���X�^���C�U�[�X�e�[�g ---
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

    // --- �v���~�e�B�u�g�|���W�[ ---
    graphicsPipeline_.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

    // --- �X�g���b�v���̃J�b�g�l ---
    graphicsPipeline_.IBStripCutValue = D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_DISABLED;

    // --- �����_�[�^�[�Q�b�g�� ---
    graphicsPipeline_.NumRenderTargets = 1;
    graphicsPipeline_.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
    graphicsPipeline_.DSVFormat = DXGI_FORMAT_D32_FLOAT;

    // --- �}���`�T���v�����O ---
    graphicsPipeline_.SampleDesc.Count = 1;
    graphicsPipeline_.SampleDesc.Quality = 0;

    // --- �A�_�v�^ ---
    graphicsPipeline_.NodeMask = 0;

    result = Graphics::Instance().GetDevice()->CreateGraphicsPipelineState(
        &graphicsPipeline_,
        IID_PPV_ARGS(&pipelineState_)
    );
    _ASSERT_EXPR(SUCCEEDED(result), HRTrace(result));

    pipelineState_->SetName(L"SpriteGraphicsPipelineState");

    // frameBuffer�̐���
    sceneBuffer_ = std::make_unique<FrameBuffer>();
}

PostProcess::~PostProcess()
{
}

void PostProcess::Draw(ID3D12GraphicsCommandList* commandList)
{
    commandList->SetPipelineState(pipelineState_.Get());
    commandList->SetGraphicsRootSignature(rootSignature_.Get());

    commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

    commandList->SetGraphicsRootDescriptorTable(0, sceneBuffer_->GetGpuHandle());

    commandList->DrawInstanced(4, 1, 0, 0);
}
