#include "PlayerManager.h"

// --- �R���X�g���N�^ ---
PlayerManager::PlayerManager()
{
    player_ = std::make_unique<Player>();
}

// --- ������ ---
void PlayerManager::Initialize()
{
    player_->Initialize();
}

// --- �I���� ---
void PlayerManager::Finalize()
{
    player_->Finalize();
}

// --- �X�V ---
void PlayerManager::Update(const float& elapsedTime)
{
    player_->Update(elapsedTime);
}

// --- �`�� ---
void PlayerManager::Render(ID3D12GraphicsCommandList* commandList)
{
    player_->Render(commandList);
}

// --- ImGui�p ---
void PlayerManager::DrawDebug()
{
    player_->DrawDebug();
}
