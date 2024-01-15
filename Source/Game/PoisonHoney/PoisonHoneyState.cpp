#include "PoisonHoneyState.h"
#include "../Character/PlayerManager.h"

// ----- MoveState -----
namespace PoisonHoneyState
{
    void MoveState::Initialize()
    {
        // ��Raduis�͊e�N���X�̏������ōs��

        SetTimer(1.5f);

        // �ϐ�������
        timer_ = 0.0f;        
        speed_ = 0.0f;
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
            rotY += DirectX::XMConvertToRadians(25) * speed_;            

            // ��]�l�� 0 ~ 360 �x �Ɏ��߂�
            if (rotY >= DirectX::XMConvertToRadians(360))   rotY -= DirectX::XMConvertToRadians(360);
            if (rotY <= DirectX::XMConvertToRadians(0))     rotY += DirectX::XMConvertToRadians(360);

            owner_->GetTransform()->SetRotationY(rotY);
        }

        // �ړ�����
        SubtractTime(elapsedTime);
        if (GetTimer() < 0.0f)
        {
            if (owner_->GetTransform()->GetRotationX() < DirectX::XMConvertToRadians(45))
            {
                float rotX = owner_->GetTransform()->GetRotationX();

                rotX += elapsedTime;
                //rotX += DirectX::XMConvertToRadians(45) * elapsedTime;

                owner_->GetTransform()->SetRotationX(rotX);
            }
            else
            {
                owner_->GetTransform()->SetRotationX(DirectX::XMConvertToRadians(45));
            }

            if (owner_->GetTransform()->GetRotationZ() < DirectX::XMConvertToRadians(45))
            {
                float rotZ = owner_->GetTransform()->GetRotationZ();

                rotZ += elapsedTime;
                //rotZ += DirectX::XMConvertToRadians(10) * elapsedTime;

                owner_->GetTransform()->SetRotationZ(rotZ);
            }
            else
            {
                owner_->GetTransform()->SetRotationZ(DirectX::XMConvertToRadians(45));
            }

            DirectX::XMFLOAT3 ownerPos = owner_->GetTransform()->GetPosition();
            DirectX::XMFLOAT3 targetPos = { 0.0f, 0.0f, 17.0f };

            if (speed_ < maxSpeed_)
            {
                speed_ += elapsedTime;
            }
            DirectX::XMFLOAT3 moveVec = {};
            DirectX::XMStoreFloat3(&moveVec,
                DirectX::XMVectorScale(DirectX::XMVector3Normalize(
                    DirectX::XMVectorSubtract(DirectX::XMLoadFloat3(&targetPos), DirectX::XMLoadFloat3(&ownerPos))), speed_));
            moveVec.y = 0;
            owner_->GetTransform()->AddPosition(moveVec);

            if (owner_->GetTransform()->GetPositionZ() >= 15.0f)
            {
                owner_->SetState(PoisonHoney::STATE::Idle);
                return;
            }
        }

        //// --- ���� ( �v���C���[���擾������ )  ---
        //{
        //    DirectX::XMFLOAT3 playerPos = PlayerManager::Instnace().GetPlayer()->GetTransform()->GetPosition();
        //    DirectX::XMFLOAT3 ownerPos = owner_->GetTransform()->GetPosition();
        //    float len = DirectX::XMVectorGetX(DirectX::XMVector3LengthSq(DirectX::XMVectorSubtract(DirectX::XMLoadFloat3(&playerPos), DirectX::XMLoadFloat3(&ownerPos))));

        //    float length = owner_->GetRadius() + PlayerManager::Instnace().GetPlayer()->GetRadius();

        //    if (len < length)
        //    {
        //        owner_->SetState(PoisonHoney::STATE::Idle);
        //        return;
        //    }
        //}
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

        // �U���\�t���O�� false �ɂ���
        owner_->SetIsAbleAttack(false);

        // �p�x��ݒ�
        owner_->GetTransform()->SetRotationX(0);
        owner_->GetTransform()->SetRotationZ(0);
        owner_->GetTransform()->SetRotationY(PlayerManager::Instnace().GetPlayer()->GetTransform()->GetRotationY());

        // �ʒu�E������ݒ�
        owner_->GetTransform()->SetPosition(PlayerManager::Instnace().GetPlayer()->GetTransform()->GetPosition());
        PlayerManager::Instnace().GetPlayer()->GetTransform()->AddRotationY(DirectX::XMConvertToRadians(90.0f));
        owner_->SetDirection(PlayerManager::Instnace().GetPlayer()->GetTransform()->CalcForward());
        PlayerManager::Instnace().GetPlayer()->GetTransform()->AddRotationY(DirectX::XMConvertToRadians(-90.0f));
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