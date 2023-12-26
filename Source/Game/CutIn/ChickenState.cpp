#include "ChickenState.h"

#include "../../Graphics/Camera.h"
#include "../../Other/Easing.h"

namespace ChickenState
{
    // ----- 初期化 -----
    void FadeInState::Initialize()
    {
        // --- Sprite初期化 ---
        owner_->GetBlackSprite0()->GetTransform()->SetPosX(-200);
        owner_->GetBlackSprite1()->GetTransform()->SetPosX(1280);

        owner_->GetWhiteSprite()->GetTransform()->SetColorA(1);

        // --- 変数初期化 ---
        easingTimer_ = 0.0f;
        addPos_ = {};
    }

    // ----- 更新 -----
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

    // ----- 終了化 -----
    void FadeInState::Finalize()
    {
    }

    // --- Sprite更新 ---
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

    // --- モデル更新 ---
    void FadeInState::UpdateModel()
    {
        // ---- モデルの位置更新 ----
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

        // --- 最終的な位置を設定する ---
        DirectX::XMFLOAT3 resultCameraPos = {};
        DirectX::XMStoreFloat3(&resultCameraPos, cameraPosVec);
        owner_->GetModel()->GetTransform()->SetPosition(resultCameraPos);



        // ----- 回転値 -----
        owner_->GetModel()->GetTransform()->SetRotationY(Camera::Instance().GetTransform()->GetRotationY() + DirectX::XMConvertToRadians(180));
    }
}