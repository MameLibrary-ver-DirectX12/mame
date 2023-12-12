#pragma once

#include <DirectXMath.h>

#include "../Resource/Transform.h"

class Camera
{
private:
    Camera();
    ~Camera() {}

public:
    static Camera& Instance()
    {
        static Camera camera;
        return camera;
    }

    Transform* GetTransform() { return &transform; }
    DirectX::XMMATRIX GetViewMatrix() { return viewMatrix_; }
    DirectX::XMMATRIX GetProjectionMatrix() { return projectionMatrix_; }

    struct View
    {
        DirectX::XMFLOAT3 eye   = { 0.0f, 0.0f, -10.0f };
        DirectX::XMFLOAT3 focus = { 0.0f, 0.0f, 0.0f };
        DirectX::XMFLOAT3 up    = { 0.0f, 1.0f, 0.0f };
    }view_;

    
    void Initialize();
    void SetPerSpectiveFov();

    void DrawDebug();

private:
    Transform transform;

    DirectX::XMMATRIX viewMatrix_;
    DirectX::XMMATRIX projectionMatrix_;

    float fov_ = 30.0f; // Ž‹–ìŠp

};

