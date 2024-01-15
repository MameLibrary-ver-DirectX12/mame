#include "BeeEnemy.h"
#include "EnemyManager.h"
#include "EnemyState.h"

// --- コンストラクタ ---
BeeEnemy::BeeEnemy()
    : Enemy("./Resources/Model/Character/Enemy/Bee.fbx")
{
    // 自分の種類を設定してマネージャーに登録
    Enemy::SetType(static_cast<UINT>(EnemyManager::TYPE::Bee));
    EnemyManager::Instance().Register(this);

    // --- ImGui 名前設定 ---
    Character::SetName("Bee" + std::to_string(nameNum_++));
}

// --- デストラクタ ---
BeeEnemy::~BeeEnemy()
{
}

// --- 初期化 ---
void BeeEnemy::Initialize()
{
    // ステートマシン登録
    GetStateMachine()->RegisterState(new EnemyState::IdleState(this));
    GetStateMachine()->RegisterState(new EnemyState::SearchState(this));
    GetStateMachine()->RegisterState(new EnemyState::MoveState(this));
    GetStateMachine()->RegisterState(new EnemyState::CollectState(this));
    GetStateMachine()->RegisterState(new EnemyState::CarryState(this));
    GetStateMachine()->RegisterState(new EnemyState::ChargeState(this));

    GetStateMachine()->SetState(1);

    Enemy::SetSearchType(Enemy::SearchType::Flower);    // 誰をターゲットにするかを設定
    Enemy::SetRadius(1.0f);                             // 半径設定
    Enemy::SetSpeed(3.0f);                              // 速度設定

    // アニメーション再生
    PlayAnimation(0, true);
}

// --- 終了化 ---
void BeeEnemy::Finalize()
{
}

// --- 更新 ---
void BeeEnemy::Update(const float& elapsedTime)
{
    Enemy::Update(elapsedTime);
}

// --- 描画 ---
void BeeEnemy::Render(ID3D12GraphicsCommandList* commandList, const DirectX::XMMATRIX& world)
{
    Enemy::Render(commandList);
}

// --- ImGui用 ---
void BeeEnemy::DrawDebug()
{
    if (ImGui::TreeNode(Character::GetName()))
    {
        Enemy::DrawDebug();

        ImGui::TreePop();
    }
}
