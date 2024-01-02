#include "UI.h"

// --- �R���X�g���N�^ ---
GameUI::GameUI()
{
    for (int i = 0; i < static_cast<UINT>(PoisonHoneyManager::TYPE::Max); ++i)
    {
        poisonHoneyNum_[i] = std::make_unique<Sprite>("./Resources/Image/numbers.png");
    }

}

// --- ������ ---
void GameUI::Initialize()
{
    for (int i = 0; i < static_cast<UINT>(PoisonHoneyManager::TYPE::Max); ++i)
    {
        poisonHoneyNum_[i]->GetTransform()->SetSize(60, 100);
        poisonHoneyNum_[i]->GetTransform()->SetTexSize(60, 100);
    }
}

// --- �I���� ---
void GameUI::Finalize()
{
}

// --- �X�V ---
void GameUI::Update(const float& elapsedTime)
{
}

// --- �`�� ---
void GameUI::Render(ID3D12GraphicsCommandList* commandList)
{
    for (int i = 0; i < static_cast<UINT>(PoisonHoneyManager::TYPE::Max); ++i)
    {
        poisonHoneyNum_[i]->Render(commandList);
    }
}

// --- ImGui�p ---
void GameUI::DrawDebug()
{
    for (int i = 0; i < static_cast<UINT>(PoisonHoneyManager::TYPE::Max); ++i)
    {
        poisonHoneyNum_[i]->DrawDebug();
    }
}
