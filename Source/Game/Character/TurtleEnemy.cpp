#include "TurtleEnemy.h"
#include "EnemyManager.h"

// --- �R���X�g���N�^ ---
TurtleEnemy::TurtleEnemy()
    : Enemy("./Resources/Model/Chracter/Enemy/Turtle.fbx")
{
    // �����̎�ނ�ݒ肵�ă}�l�[�W���[�ɓo�^
    Enemy::SetType(static_cast<UINT>(EnemyManager::TYPE::Turtle));
    EnemyManager::Instance().Register(this);
}

TurtleEnemy::~TurtleEnemy()
{
}

void TurtleEnemy::Initialize()
{
    SetMoveTime(2.0f); // �ړ����Ԃ�ݒ�
}

void TurtleEnemy::Finalize()
{
}

void TurtleEnemy::Update(const float& elapsedTime)
{
    Enemy::Update(elapsedTime);
}

void TurtleEnemy::Render(ID3D12GraphicsCommandList* commandList, const DirectX::XMMATRIX& world)
{
}

void TurtleEnemy::DrawDebug()
{
}
