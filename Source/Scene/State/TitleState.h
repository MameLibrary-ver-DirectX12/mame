#pragma once

#include "../../AI/State.h"
#include "../TitleScene.h"

namespace TitleState
{
    class FadeInState : public State<TitleScene>
    {
    public:
        FadeInState(TitleScene* titleScene) : State(titleScene, "FadeInState") {}
        ~FadeInState() {}

        void Initialize()                       override;
        void Update(const float& elapsedTime)   override;
        void Finalize()                         override;

    private:
        float easingTimer_ = 0.0f;
    };


}