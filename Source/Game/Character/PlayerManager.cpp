#include "PlayerManager.h"

// --- コンストラクタ ---
PlayerManager::PlayerManager()
{
    player_ = std::make_unique<Player>();
}

// --- 初期化 ---
void PlayerManager::Initialize()
{
    player_->Initialize();
}

// --- 終了化 ---
void PlayerManager::Finalize()
{
    player_->Finalize();
}

// --- 更新 ---
void PlayerManager::Update(const float& elapsedTime)
{
    player_->Update(elapsedTime);
}

// --- 描画 ---
void PlayerManager::Render(ID3D12GraphicsCommandList* commandList)
{
    player_->Render(commandList);
}

// --- ImGui用 ---
void PlayerManager::DrawDebug()
{
    player_->DrawDebug();
}
