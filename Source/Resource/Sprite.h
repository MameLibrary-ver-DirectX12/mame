#pragma once

#include <DirectXMath.h>
#include <d3dx12.h>

class Sprite
{
public:
    Sprite();
    ~Sprite();

    void Initialize();
    void Finalize();
    void Update();
    void Render();

private:
    struct Vertex
    {
        DirectX::XMFLOAT3 pos_;  // xyz���W
        DirectX::XMFLOAT2 uv_;   // uv���W
    };

    Vertex vertices_[4] =
    {
        { {  -1.0f, -1.0f, 0.0f }, { 0.0f, 1.0f } }, // ����
        { {  -1.0f,  1.0f, 0.0f }, { 0.0f, 0.0f } }, // ����
        { {   1.0f, -1.0f, 0.0f }, { 1.0f, 1.0f } }, // �E��
        { {   1.0f,  1.0f, 0.0f }, { 1.0f, 0.0f } }, // �E��
    };

    ID3D12Resource* vertexBuffer_ = nullptr;

};

