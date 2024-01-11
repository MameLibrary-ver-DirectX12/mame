#include "MushroomEnemy.h"
#include "EnemyManager.h"

// --- �R���X�g���N�^ ---
MushroomEnemy::MushroomEnemy()
    : Enemy("./Resources/Model/Chracter/Enemy/Mushroom.fbx")
{
    // �����̎�ނ�ݒ肵�ă}�l�[�W���[�ɓo�^
    Enemy::SetType(static_cast<UINT>(EnemyManager::TYPE::Mushroom));
    EnemyManager::Instance().Register(this);
}

MushroomEnemy::~MushroomEnemy()
{
}

void MushroomEnemy::Initialize()
{
}

void MushroomEnemy::Finalize()
{
}

void MushroomEnemy::Update(const float& elapsedTime)
{
}

void MushroomEnemy::Render(ID3D12GraphicsCommandList* commandList, const DirectX::XMMATRIX& world)
{
}

void MushroomEnemy::DrawDebug()
{
}
