#include "BoxManEnemy.h"
#include "EnemyManager.h"

// --- �R���X�g���N�^ ---
BoxManEnemy::BoxManEnemy()
    : Enemy("./Resources/Model/Chracter/Enemy/BoxMan.fbx")
{
    // �����̎�ނ�ݒ肵�ă}�l�[�W���[�ɓo�^
    Enemy::SetType(static_cast<UINT>(EnemyManager::TYPE::BoxMan));
    EnemyManager::Instance().Register(this);
}

BoxManEnemy::~BoxManEnemy()
{
}

void BoxManEnemy::Initialize()
{
}

void BoxManEnemy::Finalize()
{
}

void BoxManEnemy::Update(const float& elapsedTime)
{
    Enemy::Update(elapsedTime);
}

void BoxManEnemy::Render(ID3D12GraphicsCommandList* commandList, const DirectX::XMMATRIX& world)
{
}

void BoxManEnemy::DrawDebug()
{
}
