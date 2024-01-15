#include "Enemy.h"

int Enemy::nameNum_ = 0;

// --- コンストラクタ ---
Enemy::Enemy(const char* fbxFilename, bool triangulate, float samplingRate)
    : Character(fbxFilename, triangulate, samplingRate)
{
    stateMachine_.reset(new StateMachine<State<Enemy>>);
}

// --- 更新 ---
void Enemy::Update(const float& elapsedTime)
{
    // ステートマシン更新
    GetStateMachine()->Update(elapsedTime);

    // アニメーション更新
    Character::UpdateAnimation(elapsedTime);

    // 回転角度制御
    if (GetTransform()->GetRotationY() > DirectX::XMConvertToRadians(360)) GetTransform()->AddRotationY(-DirectX::XMConvertToRadians(360));
    if (GetTransform()->GetRotationY() < DirectX::XMConvertToRadians(0))   GetTransform()->AddRotationY( DirectX::XMConvertToRadians(360));
}

// --- 描画 ---
void Enemy::Render(ID3D12GraphicsCommandList* commandList, const DirectX::XMMATRIX& world)
{
    Character::Render(commandList, GetTransform()->CalcWorldMatrix(0.01f));
}

// --- ImGui用 ---
void Enemy::DrawDebug()
{
    GetTransform()->DrawDebug();
    GetStateMachine()->DrawDebug();
}
