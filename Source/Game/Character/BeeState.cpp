#include "BeeState.h"
#include "FlowerManager.h"
#include "Bee.h"

// ----- SearchState -----
namespace BeeState
{
    // --- 初期化 ---
    void SearchState::Initialize()
    {
    }

    // --- 更新 ---
    void SearchState::Update(const float& elapsedTime)
    {
        // --- ペアを探してあげる ---
        int flowerIndex = FlowerManager::Instance().SearchPairIndex();
        if (flowerIndex > 0)
        {
            // --- ペアが見つかった ---
            owner_->SetFlowerIndex(flowerIndex);

            // --- 次のステートへ (花に向かう) ---
            owner_->ChangeState(Bee::STATE::MoveToFlower);
        }

        // --- ペアが見つからないのこのまま ---
    }

    // --- 終了化 ---
    void SearchState::Finalize()
    {
    }
}

// ----- MoveToFlowerState -----
namespace BeeState
{
    // --- 初期化 ---
    void MoveToFlowerState::Initialize()
    {
    }

    // --- 更新 ---
    void MoveToFlowerState::Update(const float& elapsedTime)
    {
        DirectX::XMFLOAT3 flowerPos = FlowerManager::Instance().
            GetFlower(owner_->GetFlowerIndex())->GetTransform()->GetPosition();

        DirectX::XMFLOAT3 beePos = owner_->GetTransform()->GetPosition();

        

    }

    // --- 終了化 ---
    void MoveToFlowerState::Finalize()
    {
    }
}

// ----- CollectState -----
namespace BeeState
{
    // --- 初期化 ---
    void CollectState::Initialize()
    {
    }

    // --- 更新 ---
    void CollectState::Update(const float& elapsedTime)
    {
    }

    // --- 終了化 ---
    void CollectState::Finalize()
    {
    }
}

// ----- MoveToStoragePlaceState -----
namespace BeeState
{
    // --- 初期化 ---
    void MoveToStoragePlaceState::Initialize()
    {
    }

    // --- 更新 ---
    void MoveToStoragePlaceState::Update(const float& elapsedTime)
    {
    }

    // --- 終了化 ---
    void MoveToStoragePlaceState::Finalize()
    {
    }
}