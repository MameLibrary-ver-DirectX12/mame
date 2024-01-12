#include "BeeEnemy.h"
#include "EnemyManager.h"
#include "EnemyState.h"

// --- �R���X�g���N�^ ---
BeeEnemy::BeeEnemy()
    : Enemy("./Resources/Model/Character/Enemy/Bee.fbx")
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
    // �X�e�[�g�}�V���o�^
    GetStateMachine()->RegisterState(new EnemyState::IdleState(this));
    GetStateMachine()->RegisterState(new EnemyState::SearchState(this));
    GetStateMachine()->RegisterState(new EnemyState::MoveState(this));
}

// --- �I���� ---
void BeeEnemy::Finalize()
{
}

// --- �X�V ---
void BeeEnemy::Update(const float& elapsedTime)
{
    Enemy::Update(elapsedTime);
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
