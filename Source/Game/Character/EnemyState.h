#pragma once
#include "../../AI/State.h"
#include "Enemy.h"

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
    };

    class CollectState : public State<Enemy>
    {
    public:
        CollectState(Enemy* enemy) : State(enemy, "CollectState") {}
        ~CollectState() {}

        void Initialize()                       override;
        void Update(const float& elapsedTime)   override;
        void Finalize()                         override;
    };
}