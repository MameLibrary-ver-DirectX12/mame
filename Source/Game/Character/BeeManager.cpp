#include "BeeManager.h"
#include "../../Other/Common.h"

// --- ú» ---
void BeeManager::Initialize()
{
    for (Bee*& bee : bees_)
    {
        bee->Initialize();
    }
}

// --- I¹» ---
void BeeManager::Finalize()
{
    for (Bee*& bee : bees_)
    {
        bee->Finalize();
    }
    Clear();
}

// --- XV ---
void BeeManager::Update(const float& elapsedTime)
{
    // XV
    for (Bee*& bee : bees_)
    {
        bee->Update(elapsedTime);
    }

    // Jú
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

// --- `æ ---
void BeeManager::Render(ID3D12GraphicsCommandList* commandList)
{
    for (Bee*& bee : bees_)
    {
        bee->Render(commandList);
    }
}

// --- ImGuip ---
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

// --- o^ ---
void BeeManager::Register(Bee* bee)
{
    bees_.emplace_back(bee);
}

// --- Sí ---
void BeeManager::Clear()
{
    for (Bee*& bee : bees_)
    {
        SafeDeletePtr(bee);
    }
    bees_.clear();
    bees_.shrink_to_fit();
}

// --- í ---
void BeeManager::Remove(Bee* bee)
{
    removes_.insert(bee);
}
