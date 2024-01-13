#pragma once
#include "../../AI/State.h"
#include "Enemy.h"

// --- EnemyState について ---
// 共通で使えるところが多いため基底クラスで作ってみんなに使ってもらう
// 0  : IdleState
// 1  : SearchState
// 2  : MoveState
// 3~ : 各自 別ステート
#pragma region ステート管理表
// - TYPE - : 0    : 1      : 2    : 3       : 4
// Bee      : Idle : Search : Move : Collect : 
// Tutle    : Idle : Search : Move :         :
// Taiyaki  : Idle : Search : Move :         :
// Mushroom : Idle : Search : Move :         :
#pragma endregion ステート管理表

namespace EnemyState
{
    class IdleState : public State<Enemy>
    {
    public:
        IdleState(Enemy* enemy) : State(enemy, "IdleState") {}
        ~IdleState() {}

        void Initialize()                       override;
        void Update(const float& elapsedTime)   override;
        void Finalize()                         override;
    };

    class SearchState : public State<Enemy>
    {
    public:
        SearchState(Enemy* enemy) : State(enemy, "SearchState") {}
        ~SearchState() {}

        void Initialize()                       override;
        void Update(const float& elapsedTime)   override;
        void Finalize()                         override;
    };

    class MoveState : public State<Enemy>
    {
    public:
        MoveState(Enemy* enemy) : State(enemy, "MoveState") {}
        ~MoveState() {}

        void Initialize()                       override;
        void Update(const float& elapsedTime)   override;
        void Finalize()                         override;

    private:
        void Move(const float& elapsedTime);
        void Turn(const float& elapsedTime);

    };

    class CollectState : public State<Enemy>
    {
    public:
        CollectState(Enemy* enemy) : State(enemy, "CollectState") {}
        ~CollectState() {}

        void Initialize()                       override;
        void Update(const float& elapsedTime)   override;
        void Finalize()                         override;

    private:
        float collectTimer_ = 0.0f;
        float maxCollectTime_ = 0.0f;
    };

    class CarryState : public State<Enemy>
    {
    public:
        CarryState(Enemy* enemy) : State(enemy, "CarryState") {}
        ~CarryState() {}

        void Initialize()                       override;
        void Update(const float& elapsedTime)   override;
        void Finalize()                         override;

    private:
        void Move(const float& elapsedTime);
        void Turn(const float& elapsedTime);
    };
}