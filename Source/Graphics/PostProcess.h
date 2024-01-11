#pragma once

#include <d3dx12.h>
#include <memory>
#include "VertexBuffer.h"
#include "IndexBuffer.h"

class PostProcess
{
public:
    PostProcess();
    ~PostProcess();

    void Draw(ID3D12GraphicsCommandList* commandList,
        D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle);

private:
    D3D12_GRAPHICS_PIPELINE_STATE_DESC graphicsPipeline_ = {};

    Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState_;
    Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature_;

    Microsoft::WRL::ComPtr<ID3DBlob> vsBlob_;
    Microsoft::WRL::ComPtr<ID3DBlob> psBlob_;

    std::unique_ptr<VertexBuffer> vertexBuffer_;
    std::unique_ptr<IndexBuffer>  indexBuffer_;
};

