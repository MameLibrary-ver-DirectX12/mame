#include "ChickenState.h"

#include "../../Graphics/Camera.h"
#include "../../Other/Easing.h"

namespace ChickenState
{
    // ----- ������ -----
    void FadeInState::Initialize()
    {
        // --- Sprite������ ---
        owner_->GetBlackSprite0()->GetTransform()->SetPosX(-200);
        owner_->GetBlackSprite1()->GetTransform()->SetPosX(1280);

        owner_->GetWhiteSprite()->GetTransform()->SetColorA(1);

        // --- �ϐ������� ---
        easingTimer_ = 0.0f;
        addPos_ = {};
    }

    // ----- �X�V -----
    void FadeInState::Update(const float& elapsedTime)
    {
        float maxTime = 0.2f;
        if (easingTimer_ < maxTime)
        {
            float alpha = Easing::InSine(easingTimer_, maxTime, 0.5f, 1.0f);
            owner_->GetWhiteSprite()->GetTransform()->SetColorA(alpha);
        }
        else
        {
            owner_->GetWhiteSprite()->GetTransform()->SetColorA(0.4f);
        }

        UpdateSprite();
        UpdateModel();

        easingTimer_ += elapsedTime;
    }

    // ----- �I���� -----
    void FadeInState::Finalize()
    {
    }

    // --- Sprite�X�V ---
    void FadeInState::UpdateSprite()
    {
        float maxTime = 0.2f;
        if (easingTimer_ < maxTime)
        {         
            float pos0 = Easing::InCubic(easingTimer_, maxTime, -280.0f, -2000.0f);
            float pos1 = Easing::InCubic(easingTimer_, maxTime, -440.0f,  2000.0f);


            owner_->GetBlackSprite0()->GetTransform()->SetPosX(pos0);
            owner_->GetBlackSprite1()->GetTransform()->SetPosX(pos1);
        }
        else
        {
            owner_->GetBlackSprite0()->GetTransform()->SetPosX(-280.0f);
            owner_->GetBlackSprite1()->GetTransform()->SetPosX(-440.0f);
        }
    }

    // --- ���f���X�V ---
    void FadeInState::UpdateModel()
    {
        // ---- ���f���̈ʒu�X�V ----
        DirectX::XMFLOAT3 cameraFront = Camera::Instance().GetTransform()->CalcForward();
        DirectX::XMFLOAT3 cameraPos = Camera::Instance().GetTransform()->GetPosition();
        DirectX::XMVECTOR cameraPosVec = DirectX::XMLoadFloat3(&cameraPos);
        DirectX::XMVECTOR cameraFrontVec = DirectX::XMLoadFloat3(&cameraFront);
        cameraFrontVec = DirectX::XMVectorScale(cameraFrontVec, offset_);
        cameraPosVec = DirectX::XMVectorAdd(cameraPosVec, cameraFrontVec);

        float maxTime = 0.5f;
        if (easingTimer_ < maxTime)
        {
            //addPos_.y = Easing::InSine(easingTimer_, maxTime, -2.0f, -6.0f);
            addPos_.y = -10.0f;
        }
        else
        {
            addPos_.y = -2.0f;
        }

        DirectX::XMVECTOR addPos = DirectX::XMLoadFloat3(&addPos_);
        //DirectX::XMVECTOR addPos = DirectX::XMLoadFloat3(&owner_->offset_);
        cameraPosVec = DirectX::XMVectorAdd(cameraPosVec, addPos);

        // --- �ŏI�I�Ȉʒu��ݒ肷�� ---
        DirectX::XMFLOAT3 resultCameraPos = {};
        DirectX::XMStoreFloat3(&resultCameraPos, cameraPosVec);
        owner_->GetModel()->GetTransform()->SetPosition(resultCameraPos);



        // ----- ��]�l -----
        owner_->GetModel()->GetTransform()->SetRotationY(Camera::Instance().GetTransform()->GetRotationY() + DirectX::XMConvertToRadians(180));
    }
}