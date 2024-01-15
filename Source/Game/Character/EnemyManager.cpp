#include "EnemyManager.h"
#include "../../Other/Common.h"

// --- ‰Šú‰» ---
void EnemyManager::Initialize()
{
    for (Enemy*& enemy : enemies_)
    {
        enemy->Initialize();
    }

    SetSpawnGauge(0);
    SetMaxSpawnGauge(3);
}

// --- I—¹‰» ---
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
    // ¶¬
    for (Enemy* enemy : generates_)
    {
        enemies_.emplace_back(enemy);
    }
    generates_.clear();

    for (Enemy*& enemy : enemies_)
    {
        // XV
        enemy->Update(elapsedTIme);

        // Á‹“o˜^‚³‚ê‚½‚â‚Â‚ğƒJƒEƒ“ƒg‚·‚é
        if (enemy->GetIsRemove())
        {
            if (enemy->GetRemoveCount() <= Graphics::Instance().GetBufferCount() + 1)
            {
                enemy->AddRemoveCount();
            }
            else
            {
                Remove(enemy);
            }
        }
    }

    // ‰ğ•ú
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

// --- •`‰æ ---
void EnemyManager::Render(ID3D12GraphicsCommandList* commandList)
{
    for (Enemy*& enemy : enemies_)
    {
        enemy->Render(commandList);
    }
}

// --- ImGui—p ---
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

// --- “o˜^ ---
void EnemyManager::Register(Enemy* enemy)
{
    generates_.insert(enemy);
}

// --- ‘Síœ ---
void EnemyManager::Clear()
{
    for (Enemy*& enemy : enemies_)
    {
        SafeDeletePtr(enemy);
    }
    enemies_.clear();
    enemies_.shrink_to_fit();
}

// --- íœ ---
void EnemyManager::Remove(Enemy* enemy)
{
    removes_.insert(enemy);
}
