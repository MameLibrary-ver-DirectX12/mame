#pragma once
#include <d3d12.h>
#include <wrl.h>

class PipelineState
{
public:
    PipelineState(ID3D12RootSignature* rootSignature, ID3DBlob* vsShader, ID3DBlob* psShader,
        D3D12_INPUT_ELEMENT_DESC* inputElementDesc, int inputElementDescNum,
        bool blendAlphaToCoverageEnable, bool blendIndependentBlendEnable,
        bool depthEnable, D3D12_FILL_MODE rasterFillMode, D3D12_CULL_MODE rasterCullMode);

    ~PipelineState() {}

    void SetPipelineState(ID3D12GraphicsCommandList* commandList) { commandList->SetPipelineState(pipelineState_.Get()); }

private:
    Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState_;
};

