#include "MushroomEnemy.h"
#include "EnemyManager.h"

// --- コンストラクタ ---
MushroomEnemy::MushroomEnemy()
    : Enemy("./Resources/Model/Chracter/Enemy/Mushroom.fbx")
{
    // 自分の種類を設定してマネージャーに登録
    Enemy::SetType(static_cast<UINT>(EnemyManager::TYPE::Mushroom));
    EnemyManager::Instance().Register(this);
}

MushroomEnemy::~MushroomEnemy()
{
}

void MushroomEnemy::Initialize()
{
}

void MushroomEnemy::Finalize()
{
}

void MushroomEnemy::Update(const float& elapsedTime)
{
}

void MushroomEnemy::Render(ID3D12GraphicsCommandList* commandList, const DirectX::XMMATRIX& world)
{
}

void MushroomEnemy::DrawDebug()
{
}
