#include "BeeEnemy.h"
#include "EnemyManager.h"

// --- コンストラクタ ---
BeeEnemy::BeeEnemy()
    : Enemy("./Resources/Model/Character/E_Bee.fbx")
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
}

// --- 終了化 ---
void BeeEnemy::Finalize()
{
}

// --- 更新 ---
void BeeEnemy::Update(const float& elapsedTime)
{
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
