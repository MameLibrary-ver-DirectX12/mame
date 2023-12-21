#pragma once

#include <d3d12.h>
#include <wrl.h>

#include "ConstantBuffer.h"
#include "PipelineState.h"

class LambertShader
{
public:
    LambertShader();
    ~LambertShader() {}

    void SetLambertShader(ID3D12GraphicsCommandList* commandList);

private:
    Microsoft::WRL::ComPtr<ID3DBlob> vertexShader_;
    Microsoft::WRL::ComPtr<ID3DBlob> pixelShader_;

    Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature_;

    std::unique_ptr<PipelineState> pipelineState_;
    std::unique_ptr<ConstantBuffer> sceneConstantBuffer_ = {};
};

