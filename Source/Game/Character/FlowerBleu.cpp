#include "FlowerBleu.h"
#include "FlowerManager.h"
#include "PlayerManager.h"

// --- コンストラクタ ---
FlowerBleu::FlowerBleu()
    : Flower("./Resources/Model/Character/Flower/flower.fbx")
{
    // マネージャーに登録
    FlowerManager::Instance().Register(this);

    // 種類を設定
    Flower::SetFlowerType(FlowerType::Blue);
}

// --- 初期化 ---
void FlowerBleu::Initialize()
{
    // プレイヤーの前方向に設置する
    {
        DirectX::XMFLOAT3 playerPos = PlayerManager::Instnace().GetPlayer()->GetTransform()->GetPosition();
        PlayerManager::Instnace().GetPlayer()->GetTransform()->AddRotationY(DirectX::XMConvertToRadians(90.0f));
        DirectX::XMFLOAT3 playerFront = PlayerManager::Instnace().GetPlayer()->GetTransform()->CalcForward();
        PlayerManager::Instnace().GetPlayer()->GetTransform()->AddRotationY(DirectX::XMConvertToRadians(-90.0f));
        float length = 3.0f;
        DirectX::XMStoreFloat3(&playerPos, DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&playerPos),
            DirectX::XMVectorScale(DirectX::XMVector3Normalize(DirectX::XMLoadFloat3(&playerFront)), length)));
        playerPos.y = 0.0f;

        GetTransform()->SetPosition(playerPos);
    }

    // 蜂が蜜を採取できる範囲を設定する 
    Flower::SetRadius(3.0f);

    // 採取にかかる時間を設定する
    Flower::SetCollectTime(1.5f);
}

// --- 終了化 ---
void FlowerBleu::Finalize()
{
}

// --- 更新 ---
void FlowerBleu::Update(const float& elapsedTime)
{
}

// --- 描画 ---
void FlowerBleu::Render(ID3D12GraphicsCommandList* commandList, const DirectX::XMMATRIX& world)
{
    GetTransform()->SetScaleFactor(100);
    Flower::Render(commandList, GetTransform()->CalcWorldMatrix(1.0f));
}

// --- ImGui用 ---
void FlowerBleu::DrawDebug()
{
    GetTransform()->DrawDebug();
}
