#include "BoxManEnemy.h"
#include "EnemyManager.h"

// --- コンストラクタ ---
BoxManEnemy::BoxManEnemy()
    : Enemy("./Resources/Model/Chracter/Enemy/BoxMan.fbx")
{
    // 自分の種類を設定してマネージャーに登録
    Enemy::SetType(static_cast<UINT>(EnemyManager::TYPE::BoxMan));
    EnemyManager::Instance().Register(this);
}

BoxManEnemy::~BoxManEnemy()
{
}

void BoxManEnemy::Initialize()
{
}

void BoxManEnemy::Finalize()
{
}

void BoxManEnemy::Update(const float& elapsedTime)
{
    Enemy::Update(elapsedTime);
}

void BoxManEnemy::Render(ID3D12GraphicsCommandList* commandList, const DirectX::XMMATRIX& world)
{
}

void BoxManEnemy::DrawDebug()
{
}
