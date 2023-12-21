#pragma once
#include <d3d12.h>
#include <wrl.h>
#include <DirectXMath.h>

#include "ConstantBuffer.h"
#include "PipelineState.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "../Resource/Texture.h"

class SkyMap
{
public:
    SkyMap();
    ~SkyMap() {}

    void Render(ID3D12GraphicsCommandList* commandList);

private:
    Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature_;

    std::unique_ptr<PipelineState>  pipelineState_;
    std::unique_ptr<ConstantBuffer> sceneConstantBuffer_;
    std::unique_ptr<VertexBuffer>   vertexBuffer_;
    std::unique_ptr<IndexBuffer>    indexBuffer_;
    std::unique_ptr<Texture>        texture_;

    struct SceneConstants
    {
        DirectX::XMFLOAT4X4 viewProjection_;
        DirectX::XMFLOAT4   lightDirection_;
        DirectX::XMFLOAT4   cameraPostiion_;
        DirectX::XMFLOAT4X4 invViewProjection_;
    };

    struct Vertex
    {
        DirectX::XMFLOAT3 position_;
        DirectX::XMFLOAT2 texcoord_;
    };
    Vertex* vertexData_ = nullptr;
};

