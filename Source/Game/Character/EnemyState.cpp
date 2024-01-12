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
        switch(owner_->GetSearchType())
        {
        case static_cast<UINT>(Enemy::SearchType::Point):
            // 守っているもの (はちみつ)
            owner_->SetTargetPos({ 0.0f, 0.0f, 15.0f });

            break;

        case static_cast<UINT>(Enemy::SearchType::Flower):
            // 花
            owner_->SetTargetPos(FlowerManager::Instance().GetMostNearFlower(owner_->GetTransform()->GetPosition())->GetTransform()->GetPosition());
            owner_->SetPairIndex(FlowerManager::Instance().GetMostNearFlowerIndex(owner_->GetTransform()->GetPosition()));

            break;

        case static_cast<UINT>(Enemy::SearchType::Bee):
            // 蜂
            owner_->SetTargetPos(BeeManager::Instance().GetMostNearBee(owner_->GetTransform()->GetPosition())->GetTransform()->GetPosition());
            owner_->SetPairIndex(BeeManager::Instance().GetMostNearBeeIndex(owner_->GetTransform()->GetPosition()));

            break;
        }

        // 次のステートへ ( 2: MoveState )
        owner_->GetStateMachine()->ChangeState(2);
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
        SetTimer(owner_->GetMoveTime());
    }

    // --- 更新 ---
    void MoveState::Update(const float& elapsedTime)
    {
        // 亀、たい焼き、ボックスマン、ボックスマンEX は時間で次のステートへ行く
        if (owner_->GetType() ==
            static_cast<int>(EnemyManager::TYPE::Turtle)  ||
            static_cast<int>(EnemyManager::TYPE::Taiyaki) ||
            static_cast<int>(EnemyManager::TYPE::BoxMan)  ||
            static_cast<int>(EnemyManager::TYPE::BoxManEx))
        {
            // タイマーが０になったら次のステートへ行く。各enemyで違う ( 3: 〇〇State )
            if (GetTimer() <= 0.0f) owner_->GetStateMachine()->ChangeState(3);

            SubtractTime(elapsedTime);
        }

        // 蜂、キノコは場所についたら　次のステートへ行く
        if (owner_->GetType() ==
            static_cast<int>(EnemyManager::TYPE::Bee) ||
            static_cast<int>(EnemyManager::TYPE::Mushroom))
        {
            
        }

    }

    // --- 終了化 ---
    void MoveState::Finalize()
    {

    }
}

// ----- CollectState ------
namespace EnemyState
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