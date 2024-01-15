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

    // --- ImGui ���O�ݒ� ---
    Character::SetName("Bee" + std::to_string(nameNum_++));
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
    GetStateMachine()->RegisterState(new EnemyState::CollectState(this));
    GetStateMachine()->RegisterState(new EnemyState::CarryState(this));
    GetStateMachine()->RegisterState(new EnemyState::ChargeState(this));

    GetStateMachine()->SetState(1);

    Enemy::SetSearchType(Enemy::SearchType::Flower);    // �N���^�[�Q�b�g�ɂ��邩��ݒ�
    Enemy::SetRadius(1.0f);                             // ���a�ݒ�
    Enemy::SetSpeed(3.0f);                              // ���x�ݒ�

    // �A�j���[�V�����Đ�
    PlayAnimation(0, true);
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
    if (ImGui::TreeNode(Character::GetName()))
    {
        Enemy::DrawDebug();

        ImGui::TreePop();
    }
}
