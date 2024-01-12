#include "TaiyakiEnemy.h"
#include "EnemyManager.h"

// --- コンストラクタ ---
TaiyakiEnemy::TaiyakiEnemy()
    : Enemy("./Resources/Model/Character/Enemy/Taiyaki.fbx")
{
    // 自分の種類を設定してマネージャーに登録
    Enemy::SetType(static_cast<UINT>(EnemyManager::TYPE::Taiyaki));
    EnemyManager::Instance().Register(this);
}

TaiyakiEnemy::~TaiyakiEnemy()
{
}

void TaiyakiEnemy::Initialize()
{
}

void TaiyakiEnemy::Finalize()
{
}

void TaiyakiEnemy::Update(const float& elapsedTime)
{
    Enemy::Update(elapsedTime);
}

void TaiyakiEnemy::Render(ID3D12GraphicsCommandList* commandList, const DirectX::XMMATRIX& world)
{
}

void TaiyakiEnemy::DrawDebug()
{
}
