#include "BeeState.h"
#include "FlowerManager.h"
#include "Bee.h"

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
        if (flowerIndex > 0)
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
        DirectX::XMFLOAT3 flowerPos = FlowerManager::Instance().
            GetFlower(owner_->GetFlowerIndex())->GetTransform()->GetPosition();

        DirectX::XMFLOAT3 beePos = owner_->GetTransform()->GetPosition();

        

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

// ----- MoveToStoragePlaceState -----
namespace BeeState
{
    // --- ������ ---
    void MoveToStoragePlaceState::Initialize()
    {
    }

    // --- �X�V ---
    void MoveToStoragePlaceState::Update(const float& elapsedTime)
    {
    }

    // --- �I���� ---
    void MoveToStoragePlaceState::Finalize()
    {
    }
}