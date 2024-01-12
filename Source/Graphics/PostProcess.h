#pragma once

#include <d3dx12.h>
#include <memory>
#include "FrameBuffer.h"

class PostProcess
{
public:
    PostProcess();
    ~PostProcess();

    void ActivateSceneBuffer(ID3D12GraphicsCommandList* commandList) { sceneBuffer_->Activate(commandList); }
    void DeactivateSceneBuffer(ID3D12GraphicsCommandList* commandList) { sceneBuffer_->Deactivate(commandList); }

    void Draw(ID3D12GraphicsCommandList* commandList);

private:
    D3D12_GRAPHICS_PIPELINE_STATE_DESC graphicsPipeline_ = {};

    Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState_;
    Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature_;

    Microsoft::WRL::ComPtr<ID3DBlob> vsBlob_;
    Microsoft::WRL::ComPtr<ID3DBlob> psBlob_;

    std::unique_ptr<FrameBuffer> sceneBuffer_; // シーン描画用バッファー

};

