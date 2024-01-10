#include "EnemyManager.h"
#include "../../Other/Common.h"

// --- ú» ---
void EnemyManager::Initialize()
{
    for (Enemy*& enemy : enemies_)
    {
        enemy->Initialize();
    }
}

// --- I¹» ---
void EnemyManager::Finalize()
{
    for (Enemy*& enemy : enemies_)
    {
        enemy->Finalize();
    }
    Clear();
}

// --- XV ---
void EnemyManager::Update(const float& elapsedTIme)
{
    // XV
    for (Enemy*& enemy : enemies_)
    {
        enemy->Update(elapsedTIme);
    }

    // ðú
    for (Enemy* enemy : removes_)
    {
        auto it = std::find(enemies_.begin(), enemies_.end(), enemy);

        if (it != enemies_.end())
        {
            enemies_.erase(it);
        }
    }
    removes_.clear();
}

// --- `æ ---
void EnemyManager::Render(ID3D12GraphicsCommandList* commandList)
{
    for (Enemy*& enemy : enemies_)
    {
        enemy->Render(commandList);
    }
}

// --- ImGuip ---
void EnemyManager::DrawDebug()
{
    if (ImGui::TreeNode("EnemyManager"))
    {
        for (Enemy*& enemy : enemies_)
        {
            enemy->DrawDebug();
        }
        ImGui::TreePop();
    }
}

// --- o^ ---
void EnemyManager::Register(Enemy* enemy)
{
    enemies_.emplace_back(enemy);
}

// --- Sí ---
void EnemyManager::Clear()
{
    for (Enemy*& enemy : enemies_)
    {
        SafeDeletePtr(enemy);
    }
    enemies_.clear();
    enemies_.shrink_to_fit();
}

// --- í ---
void EnemyManager::Remove(Enemy* enemy)
{
    removes_.insert(enemy);
}
