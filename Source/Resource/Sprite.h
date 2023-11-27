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
        DirectX::XMFLOAT3 pos_;  // xyz座標
        DirectX::XMFLOAT2 uv_;   // uv座標
    };

    Vertex vertices_[4] =
    {
        { {  -1.0f, -1.0f, 0.0f }, { 0.0f, 1.0f } }, // 左下
        { {  -1.0f,  1.0f, 0.0f }, { 0.0f, 0.0f } }, // 左上
        { {   1.0f, -1.0f, 0.0f }, { 1.0f, 1.0f } }, // 右下
        { {   1.0f,  1.0f, 0.0f }, { 1.0f, 0.0f } }, // 右上
    };

    ID3D12Resource* vertexBuffer_ = nullptr;

};

