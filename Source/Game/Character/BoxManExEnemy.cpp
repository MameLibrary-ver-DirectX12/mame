#include "BoxManExEnemy.h"
#include "EnemyManager.h"

// --- コンストラクタ ---
BoxManExEnemy::BoxManExEnemy()
    : Enemy("./Resources/Model/Character/Enemy/BoxManEx.fbx")
{
    // 自分の種類を設定してマネージャーに登録
    Enemy::SetType(static_cast<UINT>(EnemyManager::TYPE::BoxManEx));
    EnemyManager::Instance().Register(this);
}

BoxManExEnemy::~BoxManExEnemy()
{
}

void BoxManExEnemy::Initialize()
{
}

void BoxManExEnemy::Finalize()
{
}

void BoxManExEnemy::Update(const float& elapsedTime)
{
    Enemy::Update(elapsedTime);
}

void BoxManExEnemy::Render(ID3D12GraphicsCommandList* commandList, const DirectX::XMMATRIX& world)
{
}

void BoxManExEnemy::DrawDebug()
{
}
