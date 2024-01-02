#include "UI.h"

// --- コンストラクタ ---
GameUI::GameUI()
{
    for (int i = 0; i < static_cast<UINT>(PoisonHoneyManager::TYPE::Max); ++i)
    {
        poisonHoneyNum_[i] = std::make_unique<Sprite>("./Resources/Image/numbers.png");
    }

}

// --- 初期化 ---
void GameUI::Initialize()
{
    for (int i = 0; i < static_cast<UINT>(PoisonHoneyManager::TYPE::Max); ++i)
    {
        poisonHoneyNum_[i]->GetTransform()->SetSize(60, 100);
        poisonHoneyNum_[i]->GetTransform()->SetTexSize(60, 100);
    }
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
    for (int i = 0; i < static_cast<UINT>(PoisonHoneyManager::TYPE::Max); ++i)
    {
        poisonHoneyNum_[i]->Render(commandList);
    }
}

// --- ImGui用 ---
void GameUI::DrawDebug()
{
    for (int i = 0; i < static_cast<UINT>(PoisonHoneyManager::TYPE::Max); ++i)
    {
        poisonHoneyNum_[i]->DrawDebug();
    }
}
