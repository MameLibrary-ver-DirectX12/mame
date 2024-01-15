#include "Enemy.h"

int Enemy::nameNum_ = 0;

// --- �R���X�g���N�^ ---
Enemy::Enemy(const char* fbxFilename, bool triangulate, float samplingRate)
    : Character(fbxFilename, triangulate, samplingRate)
{
    stateMachine_.reset(new StateMachine<State<Enemy>>);
}

// --- �X�V ---
void Enemy::Update(const float& elapsedTime)
{
    // �X�e�[�g�}�V���X�V
    GetStateMachine()->Update(elapsedTime);

    // �A�j���[�V�����X�V
    Character::UpdateAnimation(elapsedTime);

    // ��]�p�x����
    if (GetTransform()->GetRotationY() > DirectX::XMConvertToRadians(360)) GetTransform()->AddRotationY(-DirectX::XMConvertToRadians(360));
    if (GetTransform()->GetRotationY() < DirectX::XMConvertToRadians(0))   GetTransform()->AddRotationY( DirectX::XMConvertToRadians(360));
}

// --- �`�� ---
void Enemy::Render(ID3D12GraphicsCommandList* commandList, const DirectX::XMMATRIX& world)
{
    Character::Render(commandList, GetTransform()->CalcWorldMatrix(0.01f));
}

// --- ImGui�p ---
void Enemy::DrawDebug()
{
    GetTransform()->DrawDebug();
    GetStateMachine()->DrawDebug();
}
