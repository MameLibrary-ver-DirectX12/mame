#include "Camera.h"

#include "../Core/Application.h"

#include "../Game/Character/PlayerManager.h"

Camera::Camera()
{
}

void Camera::Initialize()
{
    transform.SetPosition(DirectX::XMFLOAT3(0.0f, 25.0f, 100.0f));
    transform.SetScale(DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f));
    transform.SetRotation(DirectX::XMFLOAT4(0.0f, DirectX::XMConvertToRadians(180), 0.0f, 0.0f));
}

void Camera::SetPerSpectiveFov()
{
    float aspectRatio = SCREEN_WIDTH / (float)SCREEN_HEIGHT;

    projectionMatrix_ = DirectX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(fov_), aspectRatio, 0.1f, 1000.0f);

    DirectX::XMVECTOR eye, focus, up;

    DirectX::XMFLOAT3 pos = transform.GetPosition();
    DirectX::XMFLOAT3 forward = transform.CalcForward();

    eye = DirectX::XMVectorSet(pos.x, pos.y, pos.z, 1.0f);
    focus = DirectX::XMVectorSet(pos.x + forward.x, pos.y + forward.y, pos.z + forward.z, 1.0f);
    up = DirectX::XMVectorSet(view_.up.x, view_.up.y, view_.up.z, 0.0f);
    
    viewMatrix_ = DirectX::XMMatrixLookAtLH(eye, focus, up);
}

void Camera::InitializeGame()
{
    transform.SetPosition(DirectX::XMFLOAT3(-110, 50, 0));
    transform.SetScale(DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f));
    transform.SetRotation(DirectX::XMFLOAT4(DirectX::XMConvertToRadians(30), DirectX::XMConvertToRadians(90), 0.0f, 0.0f));
}

// --- Game用カメラ更新 ---
void Camera::UpdateGame(const float& elapsedTime)
{
    DirectX::XMFLOAT3 ownerPos = GetTransform()->GetPosition();
    DirectX::XMFLOAT3 playerPos = PlayerManager::Instnace().GetPlayer()->GetTransform()->GetPosition();

    ownerPos.x = playerPos.x + offset.x;
    ownerPos.y = playerPos.y + offset.y;
    ownerPos.z = playerPos.z + offset.z;

    GetTransform()->SetPosition(ownerPos);
}

// --- Game用カメラ ---
void Camera::SetPerSpectiveFovGame()
{
    float aspectRatio = SCREEN_WIDTH / (float)SCREEN_HEIGHT;

    projectionMatrix_ = DirectX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(fov_), aspectRatio, 0.1f, 1000.0f);

    DirectX::XMVECTOR eye, focus, up;

    // DirectX::XMFLOAT3 playerPos = PlayerManager::Instnace().GetPlayer()->GetTransform()->GetPosition();


    DirectX::XMFLOAT3 pos = transform.GetPosition();
    DirectX::XMFLOAT3 forward = transform.CalcForward();

    eye = DirectX::XMVectorSet(pos.x, pos.y, pos.z, 1.0f);
    focus = DirectX::XMVectorSet(pos.x + forward.x, pos.y + forward.y, pos.z + forward.z, 1.0f);
    up = DirectX::XMVectorSet(view_.up.x, view_.up.y, view_.up.z, 0.0f);

    viewMatrix_ = DirectX::XMMatrixLookAtLH(eye, focus, up);
}

void Camera::DrawDebug()
{
    if (ImGui::BeginMenu("Camera"))
    {
        transform.DrawDebug();

        ImGui::DragFloat3("offset", &offset.x);

        ImGui::EndMenu();
    }
}
