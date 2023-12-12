#pragma once

#include <d3d12.h>
#include <DirectXMath.h>
#include <memory>

#include "LambertShader.h"

class Shader
{
public:
    struct SceneConstants
    {
        DirectX::XMFLOAT4X4			viewProjection;
        DirectX::XMFLOAT4			lightDirection;
    };

public:
    Shader();
    ~Shader() {}

    void SetLambertShader(ID3D12GraphicsCommandList* commandList) { lambertShader_->SetLambertShader(commandList); }

private:
    std::unique_ptr<LambertShader> lambertShader_;
};

