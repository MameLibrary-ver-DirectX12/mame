#include "TitleState.h"

#include "../../Input/Input.h"

namespace TitleState
{
    // ----- 初期化 -----
    void FadeInState::Initialize()
    {
        // --- 現在のステートを設定 ---
        owner_->SetCurrentState(TitleScene::STATE::FadeIn);

        // --- 変数初期化 ---
        easingTimer_ = 0.0f;
    }

    // ----- 更新 -----
    void FadeInState::Update(const float& elapsedTime)
    {
    }

    // ----- 終了化 -----
    void FadeInState::Finalize()
    {
    }
}