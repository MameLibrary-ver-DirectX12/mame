#include "UI.h"

#include "../PoisonHoney/PoisonHoneyManager.h"
#include "../../Graphics/NoiseTexture.h"
#include "../Character/PlayerManager.h"

// --- コンストラクタ ---
GameUI::GameUI()
{
    for (int i = 0; i < 3; ++i)
    {
        numSprite_[i] = std::make_unique<Sprite>("./Resources/Image/numbers.png");
    }

    flowerSprite_ = std::make_unique<Sprite>("./Resources/Image/flower.png");
    poisonHoneySprite_ = std::make_unique<Sprite>("./Resources/Image/poisonHoney.png");
}

// --- 初期化 ---
void GameUI::Initialize()
{
    for (int i = 0; i < 3; ++i)
    {
        numSprite_[i]->GetTransform()->SetPos(200, 590);
        numSprite_[i]->GetTransform()->SetSize(30, 50);
        numSprite_[i]->GetTransform()->SetTexSize(60, 100);
    }

    flowerSprite_->GetTransform()->SetPos(128, 592);
    flowerSprite_->GetTransform()->SetSize(128);

    poisonHoneySprite_->GetTransform()->SetPos(0, 592);
    poisonHoneySprite_->GetTransform()->SetSize(128);
}

// --- 終了化 ---
void GameUI::Finalize()
{
}

// --- 更新 ---
void GameUI::Update(const float& elapsedTime)
{
    float flowerTimer = PlayerManager::Instnace().GetPlayer()->GetFlowerTimer();
    float flowerMaxTimer = PlayerManager::Instnace().GetPlayer()->GetFlowerMaxTime();    
    
    if (flowerMaxTimer != 0) flowerSprite_->SetChargeValue(flowerTimer / flowerMaxTimer);

}

// --- 描画 ---
void GameUI::Render(ID3D12GraphicsCommandList* commandList)
{
    flowerSprite_->UseChargePixelShader();
    flowerSprite_->Render(commandList, 1);


    poisonHoneySprite_->Render(commandList);
    NumRender(commandList, PoisonHoneyManager::Instance().GetAbleAttckNum(PoisonHoneyManager::TYPE::Normal), 20, 0, 0);
}

// --- ImGui用 ---
void GameUI::DrawDebug()
{
    flowerSprite_->DrawDebug();
    //numSprite_->DrawDebug();
}

void GameUI::NumRender(ID3D12GraphicsCommandList* commandList, const int& num, const float& firstPosX, const float& secondPosX, const float& thirdPosX)
{
    if (num < 0) return;

    float texSize = 60.0f;
    float one       = num       % 10 * texSize;
    float ten       = num / 10  % 10 * texSize;
    float hundred   = num / 100 % 10 * texSize;

    if (num < 10)
    {
        numSprite_[0]->GetTransform()->SetPosX(firstPosX);
        numSprite_[0]->GetTransform()->SetTexPosX(one);
        numSprite_[0]->Render(commandList);
    }
    else if (num < 100)
    {
        numSprite_[0]->GetTransform()->SetPosX(firstPosX);
        numSprite_[0]->GetTransform()->SetTexPosX(one);
        numSprite_[0]->Render(commandList);

        numSprite_[1]->GetTransform()->SetPosX(secondPosX);
        numSprite_[1]->GetTransform()->SetTexPosX(ten);
        numSprite_[1]->Render(commandList);
    }
}
