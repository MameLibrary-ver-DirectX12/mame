#include "BeeManager.h"
#include "../../Other/Common.h"

// --- ������ ---
void BeeManager::Initialize()
{
    for (Bee*& bee : bees_)
    {
        bee->Initialize();
    }
}

// --- �I���� ---
void BeeManager::Finalize()
{
    for (Bee*& bee : bees_)
    {
        bee->Finalize();
    }
}

// --- �X�V ---
void BeeManager::Update(const float& elapsedTime)
{
    // �X�V
    for (Bee*& bee : bees_)
    {
        bee->Update(elapsedTime);
    }

    // �J��
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

// --- �`�� ---
void BeeManager::Render(ID3D12GraphicsCommandList* commandList)
{
    for (Bee*& bee : bees_)
    {
        bee->Render(commandList);
    }
}

// --- ImGui�p ---
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

// --- �o�^ ---
void BeeManager::Register(Bee* bee)
{
    bees_.emplace_back(bee);
}

// --- �S�폜 ---
void BeeManager::Clear()
{
    for (Bee*& bee : bees_)
    {
        SafeDeletePtr(bee);
    }
    bees_.clear();
    bees_.shrink_to_fit();
}

// --- �폜 ---
void BeeManager::Remove(Bee* bee)
{
    removes_.insert(bee);
}
