#include "PoisonHoneyState.h"
#include "../Character/PlayerManager.h"

// ----- MoveState -----
namespace PoisonHoneyState
{
    void MoveState::Initialize()
    {
        // ※Raduisは各クラスの初期化で行う

        SetTimer(1.5f);

        // 変数初期化
        timer_ = 0.0f;        
        speed_ = 0.0f;
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
            rotY += DirectX::XMConvertToRadians(25) * speed_;            

            // 回転値を 0 ~ 360 度 に収める
            if (rotY >= DirectX::XMConvertToRadians(360))   rotY -= DirectX::XMConvertToRadians(360);
            if (rotY <= DirectX::XMConvertToRadians(0))     rotY += DirectX::XMConvertToRadians(360);

            owner_->GetTransform()->SetRotationY(rotY);
        }

        // 移動処理
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

        //// --- 判定 ( プレイヤーが取得したか )  ---
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

        // 攻撃可能フラグを false にする
        owner_->SetIsAbleAttack(false);

        // 角度を設定
        owner_->GetTransform()->SetRotationX(0);
        owner_->GetTransform()->SetRotationZ(0);
        owner_->GetTransform()->SetRotationY(PlayerManager::Instnace().GetPlayer()->GetTransform()->GetRotationY());

        // 位置・方向を設定
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