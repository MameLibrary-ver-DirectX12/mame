#include "TitleState.h"

#include "../../Input/Input.h"

namespace TitleState
{
    // ----- ������ -----
    void FadeInState::Initialize()
    {
        // --- ���݂̃X�e�[�g��ݒ� ---
        owner_->SetCurrentState(TitleScene::STATE::FadeIn);

        // --- �ϐ������� ---
        easingTimer_ = 0.0f;
    }

    // ----- �X�V -----
    void FadeInState::Update(const float& elapsedTime)
    {
    }

    // ----- �I���� -----
    void FadeInState::Finalize()
    {
    }
}