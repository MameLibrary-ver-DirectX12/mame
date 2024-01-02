#include "PoisonHoneyState.h"
#include "../Character/PlayerManager.h"

// ----- MoveState -----
namespace PoisonHoneyState
{
    void MoveState::Initialize()
    {
        // ��Raduis�͊e�N���X�̏������ōs��

        // �ϐ�������
        timer_ = 0.0f;        
    }

    void MoveState::Update(const float& elapsedTime)
    {
        // --- �ړ� ( �ӂ�ӂ킷�鏈�� ) ---
        {
            timer_ += elapsedTime;
            float addPosY = sinf(timer_) + 2.0f;

            owner_->GetTransform()->SetPositionY(addPosY);
        }

        // --- ��]���� ---
        {
            float rotY = owner_->GetTransform()->GetRotationY();

            rotY += DirectX::XMConvertToRadians(90) * elapsedTime;

            // ��]�l�� 0 ~ 360 �x �Ɏ��߂�
            if (rotY >= DirectX::XMConvertToRadians(360))   rotY -= DirectX::XMConvertToRadians(360);
            if (rotY <= DirectX::XMConvertToRadians(0))     rotY += DirectX::XMConvertToRadians(360);


            owner_->GetTransform()->SetRotationY(rotY);
        }

        // --- ���� ( �v���C���[���擾������ )  ---
        {
            DirectX::XMFLOAT3 playerPos = PlayerManager::Instnace().GetPlayer()->GetTransform()->GetPosition();
            DirectX::XMFLOAT3 ownerPos = owner_->GetTransform()->GetPosition();
            float len = DirectX::XMVectorGetX(DirectX::XMVector3LengthSq(DirectX::XMVectorSubtract(DirectX::XMLoadFloat3(&playerPos), DirectX::XMLoadFloat3(&ownerPos))));

            float length = owner_->GetRadius() + PlayerManager::Instnace().GetPlayer()->GetRadius();

            if (len < length)
            {
                owner_->SetState(PoisonHoney::STATE::Idle);
                return;
            }
        }
    }

    void MoveState::Finalize()
    {

    }
}

// ----- IdleState -----
namespace PoisonHoneyState
{
    // --- ������ ---
    void IdleState::Initialize()
    {
        // �ҋ@���͕`�悵�Ȃ�
        owner_->SetIsRender(false);

        // �U���ł���
        owner_->SetIsAbleAttack(true);
    }

    // --- �X�V ---
    void IdleState::Update(const float& elapsedTime)
    {

    }

    // --- �I���� ---
    void IdleState::Finalize()
    {

    }
}

namespace PoisonHoneyState
{
    void AttackState::Initialize()
    {
        // ���x�͊e�N���X�Őݒ�

        // �`��t���O�𗧂Ă�
        owner_->SetIsRender(true);
    }

    void AttackState::Update(const float& elapsedTime)
    {
        DirectX::XMFLOAT3 moveVec;
        DirectX::XMFLOAT3 direction = owner_->GetDirection();
        DirectX::XMStoreFloat3(&moveVec, DirectX::XMVectorScale(DirectX::XMLoadFloat3(&direction), owner_->GetSpeed() * elapsedTime));

        owner_->GetTransform()->AddPosition(moveVec);
    }

    void AttackState::Finalize()
    {
    }
}