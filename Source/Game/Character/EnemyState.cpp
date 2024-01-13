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
        int type = owner_->GetSearchType();
        switch(type)
        {
        case static_cast<UINT>(Enemy::SearchType::Point):   // ����Ă������ (�͂��݂�)
            owner_->SetTargetPos({ 0.0f, 0.0f, 15.0f });

            // ���̃X�e�[�g�� ( 2: MoveState )
            owner_->GetStateMachine()->ChangeState(2);
            return;

            break;

        case static_cast<UINT>(Enemy::SearchType::Flower): // ��
        {
            // �y�A�ɂȂ��Ԃ�T��
            Flower* flower = FlowerManager::Instance().GetMostNearFlower(owner_->GetTransform()->GetPosition());
            if (flower != nullptr)
            {
                // �y�A��g��
                owner_->SetTargetPos(flower->GetTransform()->GetPosition());
                owner_->SetPairIndex(FlowerManager::Instance().GetMostNearFlowerIndex(owner_->GetTransform()->GetPosition()));

                // ���̃X�e�[�g�� ( 2: MoveState )
                owner_->GetStateMachine()->ChangeState(2);
                return;
            }
            // ������Ȃ������ꍇ�͂����������

        }
            break;
        case static_cast<UINT>(Enemy::SearchType::Bee):
            // �I
            owner_->SetTargetPos(BeeManager::Instance().GetMostNearBee(owner_->GetTransform()->GetPosition())->GetTransform()->GetPosition());
            owner_->SetPairIndex(BeeManager::Instance().GetMostNearBeeIndex(owner_->GetTransform()->GetPosition()));

            break;
        }


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
        // �^�C�}�[�ݒ�
        SetTimer(owner_->GetMoveTime());

        // �A�j���[�V�����ݒ�
        owner_->PlayAnimation(1, true);
    }

    // --- �X�V ---
    void MoveState::Update(const float& elapsedTime)
    {
#pragma region ���̃X�e�[�g�ֈڍs���邩�̔���

        // �T�A�����Ă��A�{�b�N�X�}���A�{�b�N�X�}��EX �͎��ԂŎ��̃X�e�[�g�֍s��
        if (owner_->GetType() == static_cast<int>(EnemyManager::TYPE::Turtle)   ||
            owner_->GetType() == static_cast<int>(EnemyManager::TYPE::Taiyaki)  ||
            owner_->GetType() == static_cast<int>(EnemyManager::TYPE::BoxMan)   ||
            owner_->GetType() == static_cast<int>(EnemyManager::TYPE::BoxManEx))
        {
            // �^�C�}�[���O�ɂȂ����玟�̃X�e�[�g�֍s���B�eenemy�ňႤ ( 3: �Z�ZState )
            if (GetTimer() <= 0.0f)
            {
                owner_->GetStateMachine()->ChangeState(3);
                return;
            }

            // �^�C�}�[�����炷
            SubtractTime(elapsedTime);
        }

        // �I�A�L�m�R�͏ꏊ�ɂ�����@���̃X�e�[�g�֍s��
        if (owner_->GetType() == static_cast<int>(EnemyManager::TYPE::Bee) ||
            owner_->GetType() == static_cast<int>(EnemyManager::TYPE::Mushroom))
        {
            float targetRadius = 0;

            // �^�[�Q�b�g���I�̏ꍇ
            if (owner_->GetSearchType() == static_cast<int>(Enemy::SearchType::Bee))
            {
                targetRadius = BeeManager::Instance().GetBee(owner_->GetPairIndex())->GetRadius();
            }
            // �^�[�Q�b�g���Ԃ̏ꍇ
            else if (owner_->GetSearchType() == static_cast<int>(Enemy::SearchType::Flower))
            {
                targetRadius = FlowerManager::Instance().GetFlower(owner_->GetPairIndex())->GetRadius();
            }

            DirectX::XMFLOAT3 targetPos = { owner_->GetTargetPos().x, 0, owner_->GetTargetPos().z };
            DirectX::XMFLOAT3 ownerPos = { owner_->GetTransform()->GetPositionX(), 0, owner_->GetTransform()->GetPositionZ() };

            float length = DirectX::XMVectorGetX(DirectX::XMVector3Length(
                DirectX::XMVectorSubtract(DirectX::XMLoadFloat3(&targetPos), DirectX::XMLoadFloat3(&ownerPos))));

            // �ꏊ�ɂ������̔���
            if (length < targetRadius + owner_->GetRadius())
            {
                // ���̃X�e�[�g�ɍs�� �eenemy�ňႤ ( 3: �Z�ZState )
                owner_->GetStateMachine()->ChangeState(3);
                return;
            }

        }

#pragma endregion ���̃X�e�[�g�ֈڍs���邩�̔���

        // �ړ�����
        Move(elapsedTime);

        // ��]����
        Turn(elapsedTime);
    }

    // --- �I���� ---
    void MoveState::Finalize()
    {

    }

    // --- �ړ����� ---
    void MoveState::Move(const float& elapsedTime)
    {
        // �I�͓����Ă��邽�߁A�I�Ɍ������Ă����ꍇ���t���[���ʒu���X�V������
        if (owner_->GetSearchType() == static_cast<int>(Enemy::SearchType::Bee))
        {
            owner_->SetTargetPos(BeeManager::Instance().GetBee(owner_->GetPairIndex())->GetTransform()->GetPosition());
        }

        // �ړ����x�ݒ�
        float speed = owner_->GetSpeed() * elapsedTime;

        // �i�s������ݒ�
        DirectX::XMFLOAT3 addPos = {};
        DirectX::XMFLOAT3 targetPos = owner_->GetTargetPos();
        DirectX::XMFLOAT3 ownerPos = owner_->GetTransform()->GetPosition();
        DirectX::XMVECTOR directionVec = DirectX::XMVectorSubtract(DirectX::XMLoadFloat3(&targetPos), DirectX::XMLoadFloat3(&ownerPos));
        DirectX::XMStoreFloat3(&addPos, DirectX::XMVectorScale(DirectX::XMVector3Normalize(directionVec), speed));

        // �ړ�
        owner_->GetTransform()->AddPosition(addPos);
    }

    // --- ��]���� ---
    void MoveState::Turn(const float& elapsedTime)
    {
        DirectX::XMFLOAT3 targetPos = owner_->GetTargetPos();
        DirectX::XMFLOAT3 ownerPos = owner_->GetTransform()->GetPosition();
        DirectX::XMFLOAT3 direction;
        DirectX::XMStoreFloat3(&direction, DirectX::XMVector3Normalize(
            DirectX::XMVectorSubtract(DirectX::XMLoadFloat3(&targetPos), DirectX::XMLoadFloat3(&ownerPos))));
        DirectX::XMFLOAT3 ownerFront = owner_->GetTransform()->CalcForward();

        // ��]�p�x�����߂� (cos��)
        float dot = (direction.x * ownerFront.x) + (direction.z * ownerFront.z);
        float rot = 1.0f - dot;
        float speed = 2.0f * elapsedTime;

        // �����������]�͂��Ȃ��B�l�����ł��܂��̂�
        if (acosf(dot) < DirectX::XMConvertToRadians(3)) return;

        // ���E����
        float cross = (direction.x * ownerFront.z) - (direction.z * ownerFront.x);
        float rotY = (cross > 0.0f) ? acosf(dot) * speed : -acosf(dot) * speed;
        owner_->GetTransform()->AddRotationY(rotY);
    }
}

// ----- CollectState ------
namespace EnemyState
{
    // --- ������ ---
    void CollectState::Initialize()
    {
        // �A�j���[�V�����ݒ�
        owner_->PlayAnimation(0, true);

        // �̎�ɂ����鎞�Ԃ�ݒ�
        maxCollectTime_ = 1.5f;

        // �ϐ�������
        collectTimer_ = 0.0f;
    }

    // --- �X�V ---
    void CollectState::Update(const float& elapsedTime)
    {
        collectTimer_ += elapsedTime;

        if (collectTimer_ >= maxCollectTime_)
        {
            // ���̃X�e�[�g�� 4: �A��
            owner_->GetStateMachine()->ChangeState(4);
        }

    }

    // --- �I���� ---
    void CollectState::Finalize()
    {

    }
}

// ----- CarryState -----
namespace EnemyState
{
    // --- ������ ---
    void CarryState::Initialize()
    {
        // �A�j���[�V�����ݒ�
        owner_->PlayAnimation(1, true);

        // �߂�ʒu��ݒ� ( �G�w ) 
        owner_->SetTargetPos({ 0, 0, -86.0f });
    }

    // --- �X�V ---
    void CarryState::Update(const float& elapsedTime)
    {
        // �ړ�
        Move(elapsedTime);

        // ��] 
        Turn(elapsedTime);
    }

    // --- �I���� ---
    void CarryState::Finalize()
    {
    }

    // --- �ړ� ---
    void CarryState::Move(const float& elapsedTime)
    {
        DirectX::XMFLOAT3 ownerPos = { owner_->GetTransform()->GetPositionX(), 0, owner_->GetTransform()->GetPositionZ() };
        DirectX::XMFLOAT3 targetPos = { owner_->GetTargetPos().x, 0, owner_->GetTargetPos().z };
        DirectX::XMVECTOR directionVec = DirectX::XMVectorSubtract(DirectX::XMLoadFloat3(&targetPos), DirectX::XMLoadFloat3(&ownerPos));

        float speed = owner_->GetSpeed() * elapsedTime;
        DirectX::XMFLOAT3 addPos = {};
        DirectX::XMStoreFloat3(&addPos, DirectX::XMVectorScale(DirectX::XMVector3Normalize(directionVec), speed));

        owner_->GetTransform()->AddPosition(addPos);
    }

    // --- ��] ---
    void CarryState::Turn(const float& elapsedTime)
    {
        DirectX::XMFLOAT3 targetPos = owner_->GetTargetPos();
        DirectX::XMFLOAT3 ownerPos = owner_->GetTransform()->GetPosition();
        DirectX::XMFLOAT3 direction;
        DirectX::XMStoreFloat3(&direction, DirectX::XMVector3Normalize(
            DirectX::XMVectorSubtract(DirectX::XMLoadFloat3(&targetPos), DirectX::XMLoadFloat3(&ownerPos))));
        DirectX::XMFLOAT3 ownerFront = owner_->GetTransform()->CalcForward();

        // ��]�p�x�����߂� (cos��)
        float dot = (direction.x * ownerFront.x) + (direction.z * ownerFront.z);
        float rot = 1.0f - dot;
        float speed = 2.0f * elapsedTime;

        // �����������]�͂��Ȃ��B�l�����ł��܂��̂�
        if (acosf(dot) < DirectX::XMConvertToRadians(3)) return;

        // ���E����
        float cross = (direction.x * ownerFront.z) - (direction.z * ownerFront.x);
        float rotY = (cross > 0.0f) ? acosf(dot) * speed : -acosf(dot) * speed;
        owner_->GetTransform()->AddRotationY(rotY);
    }
}