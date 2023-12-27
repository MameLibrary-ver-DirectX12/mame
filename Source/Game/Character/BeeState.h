#pragma once
#include "../../AI/State.h"
#include "Bee.h"

namespace BeeState
{
    class SearchState : public State<Bee>
    {
        SearchState(Bee* bee) : State(bee, "SearchState") {}
        ~SearchState() {}

        void Initialize()                       override;
        void Update(const float& elapsedTime)   override;
        void Finalize()                         override;
    };
}