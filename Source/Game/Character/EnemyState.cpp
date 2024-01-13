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
    // --- 初期化 ---
    void SearchState::Initialize()
    {
        // 花か蜂かどっちをターゲットするかをここで決める
        if (owner_->GetSearchType() == static_cast<int>(Enemy::SearchType::FlowerAndBee))
        {
            owner_->SetSearchType((rand() % 2) ? Enemy::SearchType::Flower : Enemy::SearchType::Bee);
        }
    }

    // --- 更新 ---
    void SearchState::Update(const float& elapsedTime)
    {
        int type = owner_->GetSearchType();
        switch(type)
        {
        case static_cast<UINT>(Enemy::SearchType::Point):   // 守っているもの (はちみつ)
            owner_->SetTargetPos({ 0.0f, 0.0f, 15.0f });

            // 次のステートへ ( 2: MoveState )
            owner_->GetStateMachine()->ChangeState(2);
            return;

            break;

        case static_cast<UINT>(Enemy::SearchType::Flower): // 花
        {
            // ペアになれる花を探す
            Flower* flower = FlowerManager::Instance().GetMostNearFlower(owner_->GetTransform()->GetPosition());
            if (flower != nullptr)
            {
                // ペアを組む
                owner_->SetTargetPos(flower->GetTransform()->GetPosition());
                owner_->SetPairIndex(FlowerManager::Instance().GetMostNearFlowerIndex(owner_->GetTransform()->GetPosition()));

                // 次のステートへ ( 2: MoveState )
                owner_->GetStateMachine()->ChangeState(2);
                return;
            }
            // 見つからなかった場合はもう一周する

        }
            break;
        case static_cast<UINT>(Enemy::SearchType::Bee):
            // 蜂
            owner_->SetTargetPos(BeeManager::Instance().GetMostNearBee(owner_->GetTransform()->GetPosition())->GetTransform()->GetPosition());
            owner_->SetPairIndex(BeeManager::Instance().GetMostNearBeeIndex(owner_->GetTransform()->GetPosition()));

            break;
        }


    }

    // --- 終了化 ---
    void SearchState::Finalize()
    {

    }
}

// ----- MoveState -----
namespace EnemyState
{
    // --- 初期化 ---
    void MoveState::Initialize()
    {
        // タイマー設定
        SetTimer(owner_->GetMoveTime());

        // アニメーション設定
        owner_->PlayAnimation(1, true);
    }

    // --- 更新 ---
    void MoveState::Update(const float& elapsedTime)
    {
#pragma region 次のステートへ移行するかの判定

        // 亀、たい焼き、ボックスマン、ボックスマンEX は時間で次のステートへ行く
        if (owner_->GetType() == static_cast<int>(EnemyManager::TYPE::Turtle)   ||
            owner_->GetType() == static_cast<int>(EnemyManager::TYPE::Taiyaki)  ||
            owner_->GetType() == static_cast<int>(EnemyManager::TYPE::BoxMan)   ||
            owner_->GetType() == static_cast<int>(EnemyManager::TYPE::BoxManEx))
        {
            // タイマーが０になったら次のステートへ行く。各enemyで違う ( 3: 〇〇State )
            if (GetTimer() <= 0.0f)
            {
                owner_->GetStateMachine()->ChangeState(3);
                return;
            }

            // タイマーを減らす
            SubtractTime(elapsedTime);
        }

        // 蜂、キノコは場所についたら　次のステートへ行く
        if (owner_->GetType() == static_cast<int>(EnemyManager::TYPE::Bee) ||
            owner_->GetType() == static_cast<int>(EnemyManager::TYPE::Mushroom))
        {
            float targetRadius = 0;

            // ターゲットが蜂の場合
            if (owner_->GetSearchType() == static_cast<int>(Enemy::SearchType::Bee))
            {
                targetRadius = BeeManager::Instance().GetBee(owner_->GetPairIndex())->GetRadius();
            }
            // ターゲットが花の場合
            else if (owner_->GetSearchType() == static_cast<int>(Enemy::SearchType::Flower))
            {
                targetRadius = FlowerManager::Instance().GetFlower(owner_->GetPairIndex())->GetRadius();
            }

            DirectX::XMFLOAT3 targetPos = { owner_->GetTargetPos().x, 0, owner_->GetTargetPos().z };
            DirectX::XMFLOAT3 ownerPos = { owner_->GetTransform()->GetPositionX(), 0, owner_->GetTransform()->GetPositionZ() };

            float length = DirectX::XMVectorGetX(DirectX::XMVector3Length(
                DirectX::XMVectorSubtract(DirectX::XMLoadFloat3(&targetPos), DirectX::XMLoadFloat3(&ownerPos))));

            // 場所についたかの判定
            if (length < targetRadius + owner_->GetRadius())
            {
                // 次のステートに行く 各enemyで違う ( 3: 〇〇State )
                owner_->GetStateMachine()->ChangeState(3);
                return;
            }

        }

#pragma endregion 次のステートへ移行するかの判定

        // 移動処理
        Move(elapsedTime);

        // 回転処理
        Turn(elapsedTime);
    }

    // --- 終了化 ---
    void MoveState::Finalize()
    {

    }

    // --- 移動処理 ---
    void MoveState::Move(const float& elapsedTime)
    {
        // 蜂は動いているため、蜂に向かっていく場合毎フレーム位置を更新したい
        if (owner_->GetSearchType() == static_cast<int>(Enemy::SearchType::Bee))
        {
            owner_->SetTargetPos(BeeManager::Instance().GetBee(owner_->GetPairIndex())->GetTransform()->GetPosition());
        }

        // 移動速度設定
        float speed = owner_->GetSpeed() * elapsedTime;

        // 進行方向を設定
        DirectX::XMFLOAT3 addPos = {};
        DirectX::XMFLOAT3 targetPos = owner_->GetTargetPos();
        DirectX::XMFLOAT3 ownerPos = owner_->GetTransform()->GetPosition();
        DirectX::XMVECTOR directionVec = DirectX::XMVectorSubtract(DirectX::XMLoadFloat3(&targetPos), DirectX::XMLoadFloat3(&ownerPos));
        DirectX::XMStoreFloat3(&addPos, DirectX::XMVectorScale(DirectX::XMVector3Normalize(directionVec), speed));

        // 移動
        owner_->GetTransform()->AddPosition(addPos);
    }

    // --- 回転処理 ---
    void MoveState::Turn(const float& elapsedTime)
    {
        DirectX::XMFLOAT3 targetPos = owner_->GetTargetPos();
        DirectX::XMFLOAT3 ownerPos = owner_->GetTransform()->GetPosition();
        DirectX::XMFLOAT3 direction;
        DirectX::XMStoreFloat3(&direction, DirectX::XMVector3Normalize(
            DirectX::XMVectorSubtract(DirectX::XMLoadFloat3(&targetPos), DirectX::XMLoadFloat3(&ownerPos))));
        DirectX::XMFLOAT3 ownerFront = owner_->GetTransform()->CalcForward();

        // 回転角度を求める (cosθ)
        float dot = (direction.x * ownerFront.x) + (direction.z * ownerFront.z);
        float rot = 1.0f - dot;
        float speed = 2.0f * elapsedTime;

        // 小さすぎる回転はしない。値が飛んでしまうので
        if (acosf(dot) < DirectX::XMConvertToRadians(3)) return;

        // 左右判定
        float cross = (direction.x * ownerFront.z) - (direction.z * ownerFront.x);
        float rotY = (cross > 0.0f) ? acosf(dot) * speed : -acosf(dot) * speed;
        owner_->GetTransform()->AddRotationY(rotY);
    }
}

// ----- CollectState ------
namespace EnemyState
{
    // --- 初期化 ---
    void CollectState::Initialize()
    {
        // アニメーション設定
        owner_->PlayAnimation(0, true);

        // 採取にかかる時間を設定
        maxCollectTime_ = 1.5f;

        // 変数初期化
        collectTimer_ = 0.0f;
    }

    // --- 更新 ---
    void CollectState::Update(const float& elapsedTime)
    {
        collectTimer_ += elapsedTime;

        if (collectTimer_ >= maxCollectTime_)
        {
            // 次のステートへ 4: 帰る
            owner_->GetStateMachine()->ChangeState(4);
        }

    }

    // --- 終了化 ---
    void CollectState::Finalize()
    {

    }
}

// ----- CarryState -----
namespace EnemyState
{
    // --- 初期化 ---
    void CarryState::Initialize()
    {
        // アニメーション設定
        owner_->PlayAnimation(1, true);

        // 戻る位置を設定 ( 敵陣 ) 
        owner_->SetTargetPos({ 0, 0, -86.0f });
    }

    // --- 更新 ---
    void CarryState::Update(const float& elapsedTime)
    {
        // 移動
        Move(elapsedTime);

        // 回転 
        Turn(elapsedTime);
    }

    // --- 終了化 ---
    void CarryState::Finalize()
    {
    }

    // --- 移動 ---
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

    // --- 回転 ---
    void CarryState::Turn(const float& elapsedTime)
    {
        DirectX::XMFLOAT3 targetPos = owner_->GetTargetPos();
        DirectX::XMFLOAT3 ownerPos = owner_->GetTransform()->GetPosition();
        DirectX::XMFLOAT3 direction;
        DirectX::XMStoreFloat3(&direction, DirectX::XMVector3Normalize(
            DirectX::XMVectorSubtract(DirectX::XMLoadFloat3(&targetPos), DirectX::XMLoadFloat3(&ownerPos))));
        DirectX::XMFLOAT3 ownerFront = owner_->GetTransform()->CalcForward();

        // 回転角度を求める (cosθ)
        float dot = (direction.x * ownerFront.x) + (direction.z * ownerFront.z);
        float rot = 1.0f - dot;
        float speed = 2.0f * elapsedTime;

        // 小さすぎる回転はしない。値が飛んでしまうので
        if (acosf(dot) < DirectX::XMConvertToRadians(3)) return;

        // 左右判定
        float cross = (direction.x * ownerFront.z) - (direction.z * ownerFront.x);
        float rotY = (cross > 0.0f) ? acosf(dot) * speed : -acosf(dot) * speed;
        owner_->GetTransform()->AddRotationY(rotY);
    }
}