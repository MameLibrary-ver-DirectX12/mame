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

// --- デストラクタ ---
TurtleEnemy::~TurtleEnemy()
{
}

// --- 初期化 ---
void TurtleEnemy::Initialize()
{
    SetMoveTime(2.0f); // 移動時間を設定
}

// --- 終了化 ---
void TurtleEnemy::Finalize()
{
}

// --- 更新 ---
void TurtleEnemy::Update(const float& elapsedTime)
{
    Enemy::Update(elapsedTime);
}

// --- 描画 ---
void TurtleEnemy::Render(ID3D12GraphicsCommandList* commandList, const DirectX::XMMATRIX& world)
{
    Enemy::Render(commandList);
}

// --- ImGui用 ---
void TurtleEnemy::DrawDebug()
{
}
