#include "BeeState.h"
#include "FlowerManager.h"
#include "Bee.h"

#include "../PoisonHoney/PoisonHoneyNormal.h"

// ----- SearchState -----
namespace BeeState
{
    // --- ������ ---
    void SearchState::Initialize()
    {
    }

    // --- �X�V ---
    void SearchState::Update(const float& elapsedTime)
    {
        // --- �y�A��T���Ă����� ---
        int flowerIndex = FlowerManager::Instance().SearchPairIndex();
        if (flowerIndex >= 0)
        {
            // --- �y�A���������� ---
            owner_->SetFlowerIndex(flowerIndex);

            // --- ���̃X�e�[�g�� (�ԂɌ�����) ---
            owner_->ChangeState(Bee::STATE::MoveToFlower);
        }

        // --- �y�A��������Ȃ��̂��̂܂� ---
    }

    // --- �I���� ---
    void SearchState::Finalize()
    {
    }
}

// ----- MoveToFlowerState -----
namespace BeeState
{
    // --- ������ ---
    void MoveToFlowerState::Initialize()
    {
    }

    // --- �X�V ---
    void MoveToFlowerState::Update(const float& elapsedTime)
    {
        Flower* flower = FlowerManager::Instance().GetFlower(owner_->GetFlowerIndex());

        // XZ���ʂŔ��肵�����̂�Y��0��������
        DirectX::XMFLOAT3 flowerPos = { flower->GetTransform()->GetPositionX(), 0, flower->GetTransform()->GetPositionZ() };
        DirectX::XMFLOAT3 beePos = { owner_->GetTransform()->GetPositionX() , 0, owner_->GetTransform()->GetPositionZ() };

        DirectX::XMVECTOR flowerVec = DirectX::XMLoadFloat3(&flowerPos);
        DirectX::XMVECTOR beeVec = DirectX::XMLoadFloat3(&beePos);
        DirectX::XMVECTOR Vec = DirectX::XMVectorSubtract(flowerVec, beeVec);

        // �̎�ꏊ�ɂ������̔���
        float vecLength = DirectX::XMVectorGetX(DirectX::XMVector3LengthSq(Vec));
        float length = flower->GetRadius() + owner_->GetRadius();
        if (vecLength < length)
        {
            // ���̃X�e�[�g�ɍs�� (�̎�)
            owner_->ChangeState(Bee::STATE::Collect);

            return;
        }

        // �i�ޑ��x�ݒ�
        float speed = owner_->GetMoveSpeed() * elapsedTime;

        // �i�ޕ�����ݒ� (�ԂɌ�����)
        DirectX::XMFLOAT3 addPos;
        DirectX::XMStoreFloat3(&addPos, DirectX::XMVectorScale(DirectX::XMVector3Normalize(Vec), speed));

        // �ړ�
        owner_->GetTransform()->AddPosition(addPos);
    }

    // --- �I���� ---
    void MoveToFlowerState::Finalize()
    {
    }
}

// ----- CollectState -----
namespace BeeState
{
    // --- ������ ---
    void CollectState::Initialize()
    {
        // ���̍̎掞�Ԃ�ݒ�
        owner_->SetMaxCollectTime(FlowerManager::Instance().GetFlower(owner_->GetFlowerIndex())->GetCollectTime());

        owner_->SetCollectTimer(0.0f);
    }

    // --- �X�V ---
    void CollectState::Update(const float& elapsedTime)
    {
        // �����̎悷��
        owner_->AddCollectTimer(elapsedTime);

        // ���̍̎悪��������
        if (owner_->GetCollectTimer() >= owner_->GetMaxCollectTime())
        {
            owner_->ChangeState(Bee::STATE::MoveToStoragePlace);

            return;
        }
    }

    // --- �I���� ---
    void CollectState::Finalize()
    {
    }
}

// ----- MoveToStoragePlaceState -----
namespace BeeState
{
    // --- ������ ---
    void MoveToStoragePlaceState::Initialize()
    {
        // �u���ɍs���ꏊ��ݒ�
        targetPos = { 10, 0, -4 };
    }

    // --- �X�V ---
    void MoveToStoragePlaceState::Update(const float& elapsedTime)
    {
        // XZ���ʂŔ��肵�����̂�Y��0��������
        DirectX::XMFLOAT3 beePos = { owner_->GetTransform()->GetPositionX() , 0, owner_->GetTransform()->GetPositionZ() };

        DirectX::XMVECTOR targetVec = DirectX::XMLoadFloat3(&targetPos);
        DirectX::XMVECTOR beeVec = DirectX::XMLoadFloat3(&beePos);
        DirectX::XMVECTOR Vec = DirectX::XMVectorSubtract(targetVec, beeVec);

        // �u����ɂ���������
        float vecLength = DirectX::XMVectorGetX(DirectX::XMVector3LengthSq(Vec));
        float length = 2.0f + owner_->GetRadius();
        if (vecLength < length)
        {
            // ���̃X�e�[�g�ɍs�� (�T��)
            owner_->ChangeState(Bee::STATE::Search);

            // �y�A����
            FlowerManager::Instance().RemovePair(owner_->GetFlowerIndex());

            PoisonHoneyNormal* poison = new PoisonHoneyNormal;
            poison->Initialize();
            poison->GetTransform()->SetPosition(owner_->GetTransform()->GetPosition());

            return;
        }

        // �i�ޑ��x�ݒ�
        float speed = owner_->GetMoveSpeed() * elapsedTime;

        // �i�ޕ�����ݒ� (�ԂɌ�����)
        DirectX::XMFLOAT3 addPos;
        DirectX::XMStoreFloat3(&addPos, DirectX::XMVectorScale(DirectX::XMVector3Normalize(Vec), speed));

        // �ړ�
        owner_->GetTransform()->AddPosition(addPos);
    }

    // --- �I���� ---
    void MoveToStoragePlaceState::Finalize()
    {
    }
}