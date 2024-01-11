#include "TurtleEnemy.h"
#include "EnemyManager.h"

// --- コンストラクタ ---
TurtleEnemy::TurtleEnemy()
    : Enemy("./Resources/Model/Chracter/Enemy/Turtle.fbx")
{
    // 自分の種類を設定してマネージャーに登録
    Enemy::SetType(static_cast<UINT>(EnemyManager::TYPE::Turtle));
    EnemyManager::Instance().Register(this);
}

TurtleEnemy::~TurtleEnemy()
{
}

void TurtleEnemy::Initialize()
{
}

void TurtleEnemy::Finalize()
{
}

void TurtleEnemy::Update(const float& elapsedTime)
{
}

void TurtleEnemy::Render(ID3D12GraphicsCommandList* commandList, const DirectX::XMMATRIX& world)
{
}

void TurtleEnemy::DrawDebug()
{
}
