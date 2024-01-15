#include "TurtleEnemy.h"
#include "EnemyManager.h"
#include "EnemyState.h"

// --- �R���X�g���N�^ ---
TurtleEnemy::TurtleEnemy()
    : Enemy("./Resources/Model/Character/Enemy/Turtle.fbx")
{
    // �����̎�ނ�ݒ肵�ă}�l�[�W���[�ɓo�^
    Enemy::SetType(static_cast<UINT>(EnemyManager::TYPE::Turtle));
    EnemyManager::Instance().Register(this);

    // --- ImGui ���O�ݒ� ---
    Character::SetName("Turtle" + std::to_string(nameNum_++));
}

// --- �f�X�g���N�^ ---
TurtleEnemy::~TurtleEnemy()
{
}

// --- ������ ---
void TurtleEnemy::Initialize()
{
    // �X�e�[�g�}�V���o�^
    GetStateMachine()->RegisterState(new EnemyState::IdleState(this));
    GetStateMachine()->RegisterState(new EnemyState::SearchState(this));
    GetStateMachine()->RegisterState(new EnemyState::MoveState(this));
    GetStateMachine()->RegisterState(new EnemyState::CreateEnemyState(this));

    GetStateMachine()->SetState(1);

    Enemy::SetSearchType(Enemy::SearchType::Point); // �N���^�[�Q�b�g�ɂ��邩
    Enemy::SetRadius(1.0f);                         // ���a�ݒ�
    Enemy::SetSpeed(2.0f);                          // ���x�ݒ�

    SetMoveTime(2.0f); // �ړ����Ԃ�ݒ�
}

// --- �I���� ---
void TurtleEnemy::Finalize()
{
}

// --- �X�V ---
void TurtleEnemy::Update(const float& elapsedTime)
{
    Enemy::Update(elapsedTime);
}

// --- �`�� ---
void TurtleEnemy::Render(ID3D12GraphicsCommandList* commandList, const DirectX::XMMATRIX& world)
{
    Enemy::Render(commandList);
}

// --- ImGui�p ---
void TurtleEnemy::DrawDebug()
{
}
