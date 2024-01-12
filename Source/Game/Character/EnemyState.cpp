#include "EnemyState.h"
#include "EnemyManager.h"
#include "FlowerManager.h"
#include "BeeManager.h"

// ----- IdleState -----
namespace EnemyState
{
    void IdleState::Initialize()
    {

    }

    void IdleState::Update(const float& elapsedTime)
    {

    }

    void IdleState::Finalize()
    {

    }
}

// ----- SearchState -----
namespace EnemyState
{
    // --- ������ ---
    void SearchState::Initialize()
    {
        // �Ԃ��I���ǂ������^�[�Q�b�g���邩�������Ō��߂�
        if (owner_->GetSearchType() == static_cast<int>(Enemy::SearchType::FlowerAndBee))
        {
            owner_->SetSearchType((rand() % 2) ? Enemy::SearchType::Flower : Enemy::SearchType::Bee);
        }
    }

    // --- �X�V ---
    void SearchState::Update(const float& elapsedTime)
    {
        switch(owner_->GetSearchType())
        {
        case static_cast<UINT>(Enemy::SearchType::Point):
            // ����Ă������ (�͂��݂�)
            owner_->SetTargetPos({ 0.0f, 0.0f, 15.0f });

            break;

        case static_cast<UINT>(Enemy::SearchType::Flower):
            // ��
            owner_->SetTargetPos(FlowerManager::Instance().GetMostNearFlower(owner_->GetTransform()->GetPosition())->GetTransform()->GetPosition());
            owner_->SetPairIndex(FlowerManager::Instance().GetMostNearFlowerIndex(owner_->GetTransform()->GetPosition()));

            break;

        case static_cast<UINT>(Enemy::SearchType::Bee):
            // �I
            owner_->SetTargetPos(BeeManager::Instance().GetMostNearBee(owner_->GetTransform()->GetPosition())->GetTransform()->GetPosition());
            owner_->SetPairIndex(BeeManager::Instance().GetMostNearBeeIndex(owner_->GetTransform()->GetPosition()));

            break;
        }

        // ���̃X�e�[�g�� ( 2: MoveState )
        owner_->GetStateMachine()->ChangeState(2);
    }

    // --- �I���� ---
    void SearchState::Finalize()
    {

    }
}

// ----- MoveState -----
namespace EnemyState
{
    // --- ������ ---
    void MoveState::Initialize()
    {
        SetTimer(owner_->GetMoveTime());
    }

    // --- �X�V ---
    void MoveState::Update(const float& elapsedTime)
    {
        // �T�A�����Ă��A�{�b�N�X�}���A�{�b�N�X�}��EX �͎��ԂŎ��̃X�e�[�g�֍s��
        if (owner_->GetType() ==
            static_cast<int>(EnemyManager::TYPE::Turtle)  ||
            static_cast<int>(EnemyManager::TYPE::Taiyaki) ||
            static_cast<int>(EnemyManager::TYPE::BoxMan)  ||
            static_cast<int>(EnemyManager::TYPE::BoxManEx))
        {
            // �^�C�}�[���O�ɂȂ����玟�̃X�e�[�g�֍s���B�eenemy�ňႤ ( 3: �Z�ZState )
            if (GetTimer() <= 0.0f) owner_->GetStateMachine()->ChangeState(3);

            SubtractTime(elapsedTime);
        }

        // �I�A�L�m�R�͏ꏊ�ɂ�����@���̃X�e�[�g�֍s��
        if (owner_->GetType() ==
            static_cast<int>(EnemyManager::TYPE::Bee) ||
            static_cast<int>(EnemyManager::TYPE::Mushroom))
        {
            
        }

    }

    // --- �I���� ---
    void MoveState::Finalize()
    {

    }
}

// ----- CollectState ------
namespace EnemyState
{
    // --- ������ ---
    void CollectState::Initialize()
    {

    }

    // --- �X�V ---
    void CollectState::Update(const float& elapsedTime)
    {

    }

    // --- �I���� ---
    void CollectState::Finalize()
    {

    }
}