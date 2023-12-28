#include "BeeManager.h"
#include "../../Other/Common.h"

// --- 初期化 ---
void BeeManager::Initialize()
{
    for (Bee*& bee : bees_)
    {
        bee->Initialize();
    }
}

// --- 終了化 ---
void BeeManager::Finalize()
{
    for (Bee*& bee : bees_)
    {
        bee->Finalize();
    }
}

// --- 更新 ---
void BeeManager::Update(const float& elapsedTime)
{
    // 更新
    for (Bee*& bee : bees_)
    {
        bee->Update(elapsedTime);
    }

    // 開放
    for (Bee* bee : removes_)
    {
        std::vector<Bee*>::iterator it = std::find(bees_.begin(), bees_.end(), bee);

        if (it != bees_.end())
        {
            bees_.erase(it);
        }

        SafeDeletePtr(bee);
    }
    removes_.clear();
}

// --- 描画 ---
void BeeManager::Render(ID3D12GraphicsCommandList* commandList)
{
    for (Bee*& bee : bees_)
    {
        bee->Render(commandList);
    }
}

// --- ImGui用 ---
void BeeManager::DrawDebug()
{
    if (ImGui::BeginMenu("BeeManager"))
    {
        for (Bee*& bee : bees_)
        {
            bee->DrawDebug();
        }

        ImGui::EndMenu();
    }
}

// --- 登録 ---
void BeeManager::Register(Bee* bee)
{
    bees_.emplace_back(bee);
}

// --- 全削除 ---
void BeeManager::Clear()
{
    for (Bee*& bee : bees_)
    {
        SafeDeletePtr(bee);
    }
    bees_.clear();
    bees_.shrink_to_fit();
}

// --- 削除 ---
void BeeManager::Remove(Bee* bee)
{
    removes_.insert(bee);
}
