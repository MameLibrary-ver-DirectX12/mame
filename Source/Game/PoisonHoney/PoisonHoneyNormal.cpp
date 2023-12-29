#include "PoisonHoneyNormal.h"
#include "PoisonHoneyManager.h"
#include "../Character/PlayerManager.h"

// --- コンストラクタ ---
PoisonHoneyNormal::PoisonHoneyNormal()
    : PoisonHoney("./Resources/Model/PoisonHoneyNormal.fbx")
{
    // ImGuI名前設定
    PoisonHoney::SetName("Normal");

    // 自分の種類を設定する
    PoisonHoney::SetType(static_cast<int>(PoisonHoneyManager::TYPE::Normal));
}

// --- デストラクタ ---
PoisonHoneyNormal::~PoisonHoneyNormal()
{
    // ImGui後処理は基底クラスで行っている
}

// --- 初期化 ---
void PoisonHoneyNormal::Initialize()
{
    // 進行方向取得
    direction_ = PlayerManager::Instnace().GetPlayer()->GetTransform()->CalcForward();

    // スピード設定
    speed_ = 10.0f;
}

// --- 終了化 ---
void PoisonHoneyNormal::Finalize()
{
}

// --- 更新 ---
void PoisonHoneyNormal::Update(const float& elapsedTime)
{
    DirectX::XMFLOAT3 moveVec;
    DirectX::XMStoreFloat3(&moveVec, DirectX::XMVectorScale(DirectX::XMLoadFloat3(&direction_), speed_ * elapsedTime));
    
    GetTransform()->AddPosition(moveVec);
}

// --- 描画 ---
void PoisonHoneyNormal::Render(ID3D12GraphicsCommandList* commandList, const DirectX::XMMATRIX& world)
{
    PoisonHoney::Render(commandList, GetTransform()->CalcWorldMatrix(1.0f));
}

// --- ImGui用 ---
void PoisonHoneyNormal::DrawDebug()
{
    if (ImGui::TreeNode(PoisonHoney::GetName()))
    {
        ImGui::DragFloat("speed", &speed_);

        ImGui::TreePop();
    }
}
