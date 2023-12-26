#pragma once
#include "../../AI/State.h"
#include "Player.h"

namespace PlayerState
{
    class IdleState : public State<Player>
    {
    public:
        IdleState(Player* player) : State(player, "IdleState") {}
        ~IdleState() {}

        void Initialize()                       override;
        void Update(const float& elapsedTime)   override;
        void Finalize()                         override;
    };

    class MoveState : public State<Player>
    {
    public:
        MoveState(Player* player) : State(player, "MoveState") {}
        ~MoveState() {}

        void Initialize()                       override;
        void Update(const float& elapsedTime)   override;
        void Finalize()                         override;
    };
}

