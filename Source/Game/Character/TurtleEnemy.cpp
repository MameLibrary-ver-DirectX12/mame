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
    SetMoveTime(2.0f); // 移動時間を設定
}

void TurtleEnemy::Finalize()
{
}

void TurtleEnemy::Update(const float& elapsedTime)
{
    Enemy::Update(elapsedTime);
}

void TurtleEnemy::Render(ID3D12GraphicsCommandList* commandList, const DirectX::XMMATRIX& world)
{
}

void TurtleEnemy::DrawDebug()
{
}
