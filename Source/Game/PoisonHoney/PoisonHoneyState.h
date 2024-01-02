#pragma once
#include "PoisonHoney.h"
#include "../../AI/State.h"

namespace PoisonHoneyState
{
    class MoveState : public State<PoisonHoney>
    {
    public:
        MoveState(PoisonHoney* poisonHoney) : State(poisonHoney, "MoveState") {}
        ~MoveState() {}

        void Initialize()                       override;
        void Update(const float& elapsedTime)   override;
        void Finalize()                         override;

    private:
        float timer_ = 0.0f;
    };

    class IdleState : public State<PoisonHoney>
    {
    public:
        IdleState(PoisonHoney* poisonHoney) : State(poisonHoney, "IdleState") {}
        ~IdleState() {}

        void Initialize()                       override;
        void Update(const float& elapsedTime)   override;
        void Finalize()                         override;
    };

    class AttackState : public State<PoisonHoney>
    {
    public:
        AttackState(PoisonHoney* poisonHoney) : State(poisonHoney, "AttackState") {}
        ~AttackState() {}

        void Initialize()                       override;
        void Update(const float& elapsedTime)   override;
        void Finalize()                         override;
    };
}
