#include "PipelineState.h"
#include "Graphics.h"
#include "../Other/Misc.h"

// --- コンストラクタ ---
PipelineState::PipelineState(ID3D12RootSignature* rootSignature, ID3DBlob* vsShader, ID3DBlob* psShader,
    D3D12_INPUT_ELEMENT_DESC* inputElementDesc, int inputElementDescNum,
    bool blendAlphaToCoverageEnable, bool blendIndependentBlendEnable,
    bool depthEnable, D3D12_FILL_MODE rasterFillMode, D3D12_CULL_MODE rasterCullMode)
{
    HRESULT result = S_OK;
    D3D12_GRAPHICS_PIPELINE_STATE_DESC graphicsPipeline = {};

    // --- ルートシグネチャ ---
    graphicsPipeline.pRootSignature = rootSignature;

    // --- シェーダー ---
    graphicsPipeline.VS.pShaderBytecode = vsShader->GetBufferPointer();
    graphicsPipeline.VS.BytecodeLength  = vsShader->GetBufferSize();
    graphicsPipeline.PS.pShaderBytecode = psShader->GetBufferPointer();
    graphicsPipeline.PS.BytecodeLength  = psShader->GetBufferSize();

    // --- 入力レイアウト ---
    graphicsPipeline.InputLayout.pInputElementDescs = inputElementDesc;
    graphicsPipeline.InputLayout.NumElements = inputElementDescNum;

    // --- ブレンドステート ---
    graphicsPipeline.BlendState.AlphaToCoverageEnable = blendAlphaToCoverageEnable;
    graphicsPipeline.BlendState.IndependentBlendEnable = blendIndependentBlendEnable;
    graphicsPipeline.BlendState.RenderTarget[0].BlendEnable = true;
    graphicsPipeline.BlendState.RenderTarget[0].LogicOpEnable = false;
    graphicsPipeline.BlendState.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
    graphicsPipeline.BlendState.RenderTarget[0].DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
    graphicsPipeline.BlendState.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
    graphicsPipeline.BlendState.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;
    graphicsPipeline.BlendState.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;
    graphicsPipeline.BlendState.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
    graphicsPipeline.BlendState.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

    // --- 深度ステンシルステート ---
    graphicsPipeline.DepthStencilState.DepthEnable = depthEnable;
    graphicsPipeline.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
    graphicsPipeline.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
    graphicsPipeline.DepthStencilState.StencilEnable = false;

    // --- ラスタライザーステート ---
    graphicsPipeline.RasterizerState.FillMode = rasterFillMode;
    graphicsPipeline.RasterizerState.CullMode = rasterCullMode;
    graphicsPipeline.RasterizerState.FrontCounterClockwise = true;
    graphicsPipeline.RasterizerState.DepthBias = D3D12_DEFAULT_DEPTH_BIAS;
    graphicsPipeline.RasterizerState.DepthBiasClamp = D3D12_DEFAULT_DEPTH_BIAS_CLAMP;
    graphicsPipeline.RasterizerState.SlopeScaledDepthBias = D3D12_DEFAULT_SLOPE_SCALED_DEPTH_BIAS;
    graphicsPipeline.RasterizerState.DepthClipEnable = true;
    graphicsPipeline.RasterizerState.MultisampleEnable = false;
    graphicsPipeline.RasterizerState.AntialiasedLineEnable = false;
    graphicsPipeline.RasterizerState.ForcedSampleCount = 0;
    graphicsPipeline.RasterizerState.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;

    // --- プリミティブトポロジー ---
    graphicsPipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

    // --- ストリップ時のカット値 ---
    graphicsPipeline.IBStripCutValue = D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_DISABLED;

    // --- レンダーターゲット数 ---
    graphicsPipeline.NumRenderTargets = 1;
    graphicsPipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
    graphicsPipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;

    // --- マルチサンプリング ---
    graphicsPipeline.SampleDesc.Count = 1;
    graphicsPipeline.SampleDesc.Quality = 0;

    // --- アダプタ ---
    graphicsPipeline.NodeMask = 0;
    graphicsPipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;

    // --- パイプラインステート生成 ---
    result = Graphics::Instance().GetDevice()->CreateGraphicsPipelineState(
        &graphicsPipeline,
        IID_PPV_ARGS(&pipelineState_)
        //IID_PPV_ARGS(pipelineState_.GetAddressOf())
    );
    _ASSERT_EXPR(SUCCEEDED(result), HRTrace(result));
}
