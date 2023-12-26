#pragma once

#include "../../AI/State.h"
#include "ChickenCutIn.h"

namespace ChickenState
{
    class FadeInState : public State<ChickenCutIn>
    {
    public:
        FadeInState(ChickenCutIn* chickenCutIn) : State(chickenCutIn, "FadeInState") {}
        ~FadeInState() override {}

        void Initialize()                       override;
        void Update(const float& elapsedTime)   override;
        void Finalize()                         override;

    private:
        void UpdateSprite();
        void UpdateModel();

    private:
        float easingTimer_ = 0.0f;

        float offset_ = 10.0f;

        DirectX::XMFLOAT3 addPos_ = {};
    };
}