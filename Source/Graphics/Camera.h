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

    struct View
    {
        DirectX::XMFLOAT3 eye   = { 0.0f, 0.0f, -10.0f };
        DirectX::XMFLOAT3 focus = { 0.0f, 0.0f, 0.0f };
        DirectX::XMFLOAT3 up    = { 0.0f, 1.0f, 0.0f };
    }view_;
        
    void Initialize();
    void SetPerSpectiveFov();
    void DrawDebug();

    // --- Game用カメラ ---
    void InitializeGame();
    void UpdateGame(const float& elapsedTime);
    void SetPerSpectiveFovGame();


    DirectX::XMFLOAT3 offset = { 0, 1, 2 };

public:// --- 取得・設定 ---
    Transform* GetTransform() { return &transform; }
    DirectX::XMMATRIX GetViewMatrix() { return viewMatrix_; }
    DirectX::XMMATRIX GetProjectionMatrix() { return projectionMatrix_; }

private:
    Transform transform;

    DirectX::XMMATRIX viewMatrix_;
    DirectX::XMMATRIX projectionMatrix_;

    float fov_ = 30.0f; // 視野角

    // --- Game用カメラ変数 ---
    float moveLength_ = 12.0f;
    float moveSpeed_ = 10.0f;

};

