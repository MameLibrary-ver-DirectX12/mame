#include "BeeState.h"
#include "FlowerManager.h"
#include "Bee.h"

#include "../PoisonHoney/PoisonHoneyNormal.h"

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
        if (flowerIndex >= 0)
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
        Flower* flower = FlowerManager::Instance().GetFlower(owner_->GetFlowerIndex());

        // XZ平面で判定したいのでYに0を代入する
        DirectX::XMFLOAT3 flowerPos = { flower->GetTransform()->GetPositionX(), 0, flower->GetTransform()->GetPositionZ() };
        DirectX::XMFLOAT3 beePos = { owner_->GetTransform()->GetPositionX() , 0, owner_->GetTransform()->GetPositionZ() };

        DirectX::XMVECTOR flowerVec = DirectX::XMLoadFloat3(&flowerPos);
        DirectX::XMVECTOR beeVec = DirectX::XMLoadFloat3(&beePos);
        DirectX::XMVECTOR Vec = DirectX::XMVectorSubtract(flowerVec, beeVec);

        // 採取場所についたかの判定
        float vecLength = DirectX::XMVectorGetX(DirectX::XMVector3LengthSq(Vec));
        float length = flower->GetRadius() + owner_->GetRadius();
        if (vecLength < length)
        {
            // 次のステートに行く (採取)
            owner_->ChangeState(Bee::STATE::Collect);

            return;
        }

        // 進む速度設定
        float speed = owner_->GetMoveSpeed() * elapsedTime;

        // 進む方向を設定 (花に向かう)
        DirectX::XMFLOAT3 addPos;
        DirectX::XMStoreFloat3(&addPos, DirectX::XMVectorScale(DirectX::XMVector3Normalize(Vec), speed));

        // 移動
        owner_->GetTransform()->AddPosition(addPos);
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
        // 蜜の採取時間を設定
        owner_->SetMaxCollectTime(FlowerManager::Instance().GetFlower(owner_->GetFlowerIndex())->GetCollectTime());

        owner_->SetCollectTimer(0.0f);
    }

    // --- 更新 ---
    void CollectState::Update(const float& elapsedTime)
    {
        // 蜜を採取する
        owner_->AddCollectTimer(elapsedTime);

        // 蜜の採取が完了した
        if (owner_->GetCollectTimer() >= owner_->GetMaxCollectTime())
        {
            owner_->ChangeState(Bee::STATE::MoveToStoragePlace);

            return;
        }
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
        // 置きに行く場所を設定
        targetPos = { 10, 0, -4 };
    }

    // --- 更新 ---
    void MoveToStoragePlaceState::Update(const float& elapsedTime)
    {
        // XZ平面で判定したいのでYに0を代入する
        DirectX::XMFLOAT3 beePos = { owner_->GetTransform()->GetPositionX() , 0, owner_->GetTransform()->GetPositionZ() };

        DirectX::XMVECTOR targetVec = DirectX::XMLoadFloat3(&targetPos);
        DirectX::XMVECTOR beeVec = DirectX::XMLoadFloat3(&beePos);
        DirectX::XMVECTOR Vec = DirectX::XMVectorSubtract(targetVec, beeVec);

        // 置き場についたか判定
        float vecLength = DirectX::XMVectorGetX(DirectX::XMVector3LengthSq(Vec));
        float length = 2.0f + owner_->GetRadius();
        if (vecLength < length)
        {
            // 次のステートに行く (探す)
            owner_->ChangeState(Bee::STATE::Search);

            // ペア解除
            FlowerManager::Instance().RemovePair(owner_->GetFlowerIndex());

            PoisonHoneyNormal* poison = new PoisonHoneyNormal;
            poison->Initialize();
            poison->GetTransform()->SetPosition(owner_->GetTransform()->GetPosition());

            return;
        }

        // 進む速度設定
        float speed = owner_->GetMoveSpeed() * elapsedTime;

        // 進む方向を設定 (花に向かう)
        DirectX::XMFLOAT3 addPos;
        DirectX::XMStoreFloat3(&addPos, DirectX::XMVectorScale(DirectX::XMVector3Normalize(Vec), speed));

        // 移動
        owner_->GetTransform()->AddPosition(addPos);
    }

    // --- 終了化 ---
    void MoveToStoragePlaceState::Finalize()
    {
    }
}