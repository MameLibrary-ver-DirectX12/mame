#include "TaiyakiEnemy.h"
#include "EnemyManager.h"

// --- �R���X�g���N�^ ---
TaiyakiEnemy::TaiyakiEnemy()
    : Enemy("./Resources/Model/Character/Enemy/Taiyaki.fbx")
{
    // �����̎�ނ�ݒ肵�ă}�l�[�W���[�ɓo�^
    Enemy::SetType(static_cast<UINT>(EnemyManager::TYPE::Taiyaki));
    EnemyManager::Instance().Register(this);
}

TaiyakiEnemy::~TaiyakiEnemy()
{
}

void TaiyakiEnemy::Initialize()
{
}

void TaiyakiEnemy::Finalize()
{
}

void TaiyakiEnemy::Update(const float& elapsedTime)
{
    Enemy::Update(elapsedTime);
}

void TaiyakiEnemy::Render(ID3D12GraphicsCommandList* commandList, const DirectX::XMMATRIX& world)
{
}

void TaiyakiEnemy::DrawDebug()
{
}
