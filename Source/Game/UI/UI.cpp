#include "UI.h"

#include "../PoisonHoney/PoisonHoneyManager.h"

// --- コンストラクタ ---
GameUI::GameUI()
{
    numSprite_ = std::make_unique<Sprite>("./Resources/Image/numbers.png");
}

// --- 初期化 ---
void GameUI::Initialize()
{
    numSprite_->GetTransform()->SetPos(200, 250);
    numSprite_->GetTransform()->SetSize(60, 100);
    numSprite_->GetTransform()->SetTexSize(60, 100);
}

// --- 終了化 ---
void GameUI::Finalize()
{
}

// --- 更新 ---
void GameUI::Update(const float& elapsedTime)
{
}

// --- 描画 ---
void GameUI::Render(ID3D12GraphicsCommandList* commandList)
{
    NumRender(commandList, PoisonHoneyManager::Instance().GetAbleAttckNum(PoisonHoneyManager::TYPE::Normal), 100, 200, 300);
}

// --- ImGui用 ---
void GameUI::DrawDebug()
{
    numSprite_->DrawDebug();
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
        numSprite_->GetTransform()->SetTexPosX(one);
        numSprite_->Render(commandList);
    }
}
