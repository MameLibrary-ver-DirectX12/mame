#include "PoisonHoneyState.h"
#include "../Character/PlayerManager.h"

// ----- MoveState -----
namespace PoisonHoneyState
{
    void MoveState::Initialize()
    {
        // ※Raduisは各クラスの初期化で行う

        // 変数初期化
        timer_ = 0.0f;        
    }

    void MoveState::Update(const float& elapsedTime)
    {
        // --- 移動 ( ふわふわする処理 ) ---
        {
            timer_ += elapsedTime;
            float addPosY = sinf(timer_) + 2.0f;

            owner_->GetTransform()->SetPositionY(addPosY);
        }

        // --- 回転処理 ---
        {
            float rotY = owner_->GetTransform()->GetRotationY();

            rotY += DirectX::XMConvertToRadians(90) * elapsedTime;

            // 回転値を 0 ~ 360 度 に収める
            if (rotY >= DirectX::XMConvertToRadians(360))   rotY -= DirectX::XMConvertToRadians(360);
            if (rotY <= DirectX::XMConvertToRadians(0))     rotY += DirectX::XMConvertToRadians(360);


            owner_->GetTransform()->SetRotationY(rotY);
        }

        // --- 判定 ( プレイヤーが取得したか )  ---
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
    // --- 初期化 ---
    void IdleState::Initialize()
    {
        // 待機中は描画しない
        owner_->SetIsRender(false);

        // 攻撃できる
        owner_->SetIsAbleAttack(true);
    }

    // --- 更新 ---
    void IdleState::Update(const float& elapsedTime)
    {

    }

    // --- 終了化 ---
    void IdleState::Finalize()
    {

    }
}

namespace PoisonHoneyState
{
    void AttackState::Initialize()
    {
        // 速度は各クラスで設定

        // 描画フラグを立てる
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