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
}
