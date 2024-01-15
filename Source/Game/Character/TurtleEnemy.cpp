#include "TurtleEnemy.h"
#include "EnemyManager.h"
#include "EnemyState.h"

// --- コンストラクタ ---
TurtleEnemy::TurtleEnemy()
    : Enemy("./Resources/Model/Character/Enemy/Turtle.fbx")
{
    // 自分の種類を設定してマネージャーに登録
    Enemy::SetType(static_cast<UINT>(EnemyManager::TYPE::Turtle));
    EnemyManager::Instance().Register(this);

    // --- ImGui 名前設定 ---
    Character::SetName("Turtle" + std::to_string(nameNum_++));
}

// --- デストラクタ ---
TurtleEnemy::~TurtleEnemy()
{
}

// --- 初期化 ---
void TurtleEnemy::Initialize()
{
    // ステートマシン登録
    GetStateMachine()->RegisterState(new EnemyState::IdleState(this));
    GetStateMachine()->RegisterState(new EnemyState::SearchState(this));
    GetStateMachine()->RegisterState(new EnemyState::MoveState(this));
    GetStateMachine()->RegisterState(new EnemyState::CreateEnemyState(this));

    GetStateMachine()->SetState(1);

    Enemy::SetSearchType(Enemy::SearchType::Point); // 誰をターゲットにするか
    Enemy::SetRadius(1.0f);                         // 半径設定
    Enemy::SetSpeed(2.0f);                          // 速度設定

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
