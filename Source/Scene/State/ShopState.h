#pragma once
#include "../../AI/State.h"
#include "../ShopScene.h"

namespace ShopState
{
    class FadeInState : public State<ShopScene>
    {
    public:
        FadeInState(ShopScene* shopScene) : State(shopScene, "FadeInState") {}
        ~FadeInState() {}

        void Initialize()                       override;
        void Update(const float& elapsedTime)   override;
        void Finalize()                         override;
    };
}