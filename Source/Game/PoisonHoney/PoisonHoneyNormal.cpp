#include "PoisonHoneyNormal.h"
#include "PoisonHoneyManager.h"
#include "../Character/PlayerManager.h"
#include "PoisonHoneyState.h"

// --- コンストラクタ ---
PoisonHoneyNormal::PoisonHoneyNormal()
    : PoisonHoney("./Resources/Model/PoisonHoney/PoisonHoneyNormal.fbx")
{
    // ImGuI名前設定
    PoisonHoney::SetName("Normal");

    // 自分の種類を設定する
    PoisonHoney::SetType(static_cast<int>(PoisonHoneyManager::TYPE::Normal));

    // ステートマシン
    PoisonHoney::CreateStateMachine();
    GetStateMachine()->RegisterState(new PoisonHoneyState::MoveState(this));
    GetStateMachine()->RegisterState(new PoisonHoneyState::IdleState(this));
    GetStateMachine()->RegisterState(new PoisonHoneyState::AttackState(this));
    GetStateMachine()->SetState(static_cast<int>(PoisonHoney::STATE::Move));

    // マネージャーに登録
    PoisonHoneyManager::Instance().Register(this);
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
    PoisonHoney::SetDirection(PlayerManager::Instnace().GetPlayer()->GetTransform()->CalcForward());

    // スピード設定
    PoisonHoney::SetSpeed(30.0f);

    // 判定用半径設定
    PoisonHoney::SetRadius(2.0f);
}

// --- 終了化 ---
void PoisonHoneyNormal::Finalize()
{
}

// --- 更新 ---
void PoisonHoneyNormal::Update(const float& elapsedTime)
{
    // --- ステートマシンでの処理 ---
    PoisonHoney::Update(elapsedTime);

    // 画面外に出た玉を消去する
    Remove();
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
        GetTransform()->DrawDebug();

        ImGui::TreePop();
    }
}

// --- 画面外に出た弾を消す ---
void PoisonHoneyNormal::Remove()
{

}
