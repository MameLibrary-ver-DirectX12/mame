#include "Transform.h"

// --- ImGui用 ---
void Transform::DrawDebug()
{
    if (ImGui::TreeNode("Transform"))
    {
        ImGui::DragFloat3("Position", &position.x, 1.0f, -FLT_MAX, FLT_MAX);
        ImGui::DragFloat3("Scale", &scale.x, 0.001f, -FLT_MAX, FLT_MAX);
        
        DirectX::XMFLOAT3 rot{};
        rot.x = DirectX::XMConvertToDegrees(rotation.x);
        rot.y = DirectX::XMConvertToDegrees(rotation.y);
        rot.z = DirectX::XMConvertToDegrees(rotation.z);
        ImGui::DragFloat3("Rotation", &rot.x, 0.5f, -FLT_MAX, FLT_MAX);
        rotation.x = DirectX::XMConvertToRadians(rot.x);
        rotation.y = DirectX::XMConvertToRadians(rot.y);
        rotation.z = DirectX::XMConvertToRadians(rot.z);
        
        ImGui::DragFloat("ScaleFactor", &scaleFactor, 0.001f, 0.001f, 100.0f);
        
        ImGui::SliderInt("Coordinate System", &coordinateSystem, 0, static_cast<int>(CoordinateSystem::cLeftZup));
        ImGui::Text(coordinateSystemName[coordinateSystem].c_str());

        if (ImGui::Button("Reset"))
        {
            Reset();
        }

        ImGui::TreePop();
    }
}

// --- Transformリセット用関数 ---
void Transform::Reset()
{
    position = DirectX::XMFLOAT3(0, 0, 0);
    scale = DirectX::XMFLOAT3(1, 1, 1);
    rotation = DirectX::XMFLOAT4(0, 0, 0, 0);
}

// --- ワールド座標取得 ---
DirectX::XMMATRIX Transform::CalcWorld()
{
    const DirectX::XMMATRIX S = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z) * DirectX::XMMatrixScaling(scaleFactor, scaleFactor, scaleFactor);
    const DirectX::XMMATRIX R = DirectX::XMMatrixRotationRollPitchYaw(rotation.x, rotation.y, rotation.z);
    const DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(position.x, position.y, position.z);

    return S * R * T;
}

// --- 座標系をかけたワールド行列 ---
DirectX::XMMATRIX Transform::CalcWorldMatrix(const float& scaleFactor)
{
    const DirectX::XMMATRIX C = DirectX::XMLoadFloat4x4(&CoordinateSystemTransforms[coordinateSystem])
        * DirectX::XMMatrixScaling(scaleFactor, scaleFactor, scaleFactor);
    
    return C * CalcWorld();
}

// --- 前方向ベクトル取得 ---
DirectX::XMFLOAT3 Transform::CalcForward() const
{
    DirectX::XMMATRIX rotationMatrix = DirectX::XMMatrixRotationRollPitchYaw(rotation.x, rotation.y, rotation.z);
    DirectX::XMFLOAT3 forward{};
    DirectX::XMStoreFloat3(&forward, DirectX::XMVector3Normalize(rotationMatrix.r[2]));
    return forward;
}

// --- 上方向ベクトル取得 ---
DirectX::XMFLOAT3 Transform::CalcUp() const
{
    DirectX::XMMATRIX rotationMatrix = DirectX::XMMatrixRotationRollPitchYaw(rotation.x, rotation.y, rotation.z);
    DirectX::XMFLOAT3 up{};
    DirectX::XMStoreFloat3(&up, DirectX::XMVector3Normalize(rotationMatrix.r[1]));
    return up;
}

// --- 右方向ベクトル取得 ---
DirectX::XMFLOAT3 Transform::CalcRight() const
{
    DirectX::XMMATRIX rotationMatrix = DirectX::XMMatrixRotationRollPitchYaw(rotation.x, rotation.y, rotation.z);
    DirectX::XMFLOAT3 right{};
    DirectX::XMStoreFloat3(&right, DirectX::XMVector3Normalize(rotationMatrix.r[0]));
    return right;
}

// --- ワールド座標設定 ---
void Transform::SetWorld(DirectX::XMMATRIX world)
{
    DirectX::XMFLOAT4X4 w;
    DirectX::XMStoreFloat4x4(&w, world);

    position = { w._11, w._12, w._13 };
    scale    = { w._21, w._22, w._23 };
    rotation = { w._31, w._32, w._33, w._34 };
}
