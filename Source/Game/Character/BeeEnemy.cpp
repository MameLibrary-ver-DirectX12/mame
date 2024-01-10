#include "BeeEnemy.h"
#include "EnemyManager.h"

// --- �R���X�g���N�^ ---
BeeEnemy::BeeEnemy()
    : Enemy("./Resources/Model/Character/E_Bee.fbx")
{
    // �����̎�ނ�ݒ肵�ă}�l�[�W���[�ɓo�^
    Enemy::SetType(static_cast<UINT>(EnemyManager::TYPE::Bee));
    EnemyManager::Instance().Register(this);
}

// --- �f�X�g���N�^ ---
BeeEnemy::~BeeEnemy()
{
}

// --- ������ ---
void BeeEnemy::Initialize()
{
}

// --- �I���� ---
void BeeEnemy::Finalize()
{
}

// --- �X�V ---
void BeeEnemy::Update(const float& elapsedTime)
{
}

// --- �`�� ---
void BeeEnemy::Render(ID3D12GraphicsCommandList* commandList, const DirectX::XMMATRIX& world)
{
    Enemy::Render(commandList);
}

// --- ImGui�p ---
void BeeEnemy::DrawDebug()
{
}
