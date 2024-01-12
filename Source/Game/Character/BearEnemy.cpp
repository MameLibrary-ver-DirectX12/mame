#include "BearEnemy.h"
#include "EnemyManager.h"

// --- �R���X�g���N�^ ---
BearEnemy::BearEnemy()
    : Enemy("./Resources/Model/Character/Bear.fbx")
{
    // �����̎�ނ�ݒ肵�ă}�l�[�W���[�ɓo�^
    Enemy::SetType(static_cast<UINT>(EnemyManager::TYPE::Bear));
    EnemyManager::Instance().Register(this);
}

BearEnemy::~BearEnemy()
{
}

void BearEnemy::Initialize()
{
}

void BearEnemy::Finalize()
{
}

// --- �X�V ---
void BearEnemy::Update(const float& elapsedTime)
{
    Enemy::Update(elapsedTime);
}

void BearEnemy::Render(ID3D12GraphicsCommandList* commandList, const DirectX::XMMATRIX& world)
{
}

void BearEnemy::DrawDebug()
{
}
