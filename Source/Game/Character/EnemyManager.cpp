#include "EnemyManager.h"
#include "../../Other/Common.h"

// --- 初期化 ---
void EnemyManager::Initialize()
{
    for (Enemy*& enemy : enemies_)
    {
        enemy->Initialize();
    }
}

// --- 終了化 ---
void EnemyManager::Finalize()
{
    for (Enemy*& enemy : enemies_)
    {
        enemy->Finalize();
    }
    Clear();
}

// --- 更新 ---
void EnemyManager::Update(const float& elapsedTIme)
{
    for (Enemy*& enemy : enemies_)
    {
        // 更新
        enemy->Update(elapsedTIme);

        // 消去登録されたやつをカウントする
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

    // 解放
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

// --- 描画 ---
void EnemyManager::Render(ID3D12GraphicsCommandList* commandList)
{
    for (Enemy*& enemy : enemies_)
    {
        enemy->Render(commandList);
    }
}

// --- ImGui用 ---
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

// --- 登録 ---
void EnemyManager::Register(Enemy* enemy)
{
    enemies_.emplace_back(enemy);
}

// --- 全削除 ---
void EnemyManager::Clear()
{
    for (Enemy*& enemy : enemies_)
    {
        SafeDeletePtr(enemy);
    }
    enemies_.clear();
    enemies_.shrink_to_fit();
}

// --- 削除 ---
void EnemyManager::Remove(Enemy* enemy)
{
    removes_.insert(enemy);
}
