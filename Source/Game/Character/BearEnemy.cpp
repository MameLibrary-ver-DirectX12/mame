#include "BearEnemy.h"
#include "EnemyManager.h"

// --- コンストラクタ ---
BearEnemy::BearEnemy()
    : Enemy("./Resources/Model/Character/Bear.fbx")
{
    // 自分の種類を設定してマネージャーに登録
    Enemy::SetType(static_cast<UINT>(EnemyManager::TYPE::Bear));
    EnemyManager::Instance().Register(this);
}

BearEnemy::~BearEnemy()
{
}

void BearEnemy::Initialize()
{
}

void BearEnemy::Finalize()
{
}

// --- 更新 ---
void BearEnemy::Update(const float& elapsedTime)
{
    Enemy::Update(elapsedTime);
}

void BearEnemy::Render(ID3D12GraphicsCommandList* commandList, const DirectX::XMMATRIX& world)
{
}

void BearEnemy::DrawDebug()
{
}
