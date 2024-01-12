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
    Clear();
}

// --- �X�V ---
void BeeManager::Update(const float& elapsedTime)
{
    for (Bee*& bee : bees_)
    {
        // �X�V
        bee->Update(elapsedTime);

        // �����o�^���ꂽ����J�E���g����
        if (bee->GetIsRemove())
        {
            if (bee->GetRemoveCount() <= Graphics::Instance().GetBufferCount() + 1)
            {
                bee->AddRemoveCount();
            }
            else
            {
                Remove(bee);
            }
        }
    }

    // �J��
    for (Bee* bee : removes_)
    {
        auto it = std::find(bees_.begin(), bees_.end(), bee);

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

// --- �����̈ʒu����ł��߂��I��Ԃ� ---
Bee* BeeManager::GetMostNearBee(const DirectX::XMFLOAT3& pos)
{
    DirectX::XMFLOAT3 position = pos;
    float mostNearLength = FLT_MAX;
    float mostNearBeeIndex = 0;
    int num = -1;
    for (Bee*& bee : bees_)
    {
        ++num;

        DirectX::XMFLOAT3 beePos = bee->GetTransform()->GetPosition();
        beePos.y = 0;
        position.y = 0;

        float len = DirectX::XMVectorGetX(DirectX::XMVector3Length(DirectX::XMVectorSubtract(DirectX::XMLoadFloat3(&beePos), DirectX::XMLoadFloat3(&position))));

        if (mostNearLength > len)
        {
            mostNearLength = len;
            mostNearBeeIndex = num;
        }
    }

    return bees_.at(mostNearBeeIndex);
}

// --- �����̈ʒu����ł��߂��I�̃C���f�b�N�X��Ԃ� ---
int BeeManager::GetMostNearBeeIndex(const DirectX::XMFLOAT3& pos)
{
    DirectX::XMFLOAT3 position = pos;
    float mostNearLength = FLT_MAX;
    float mostNearBeeIndex = 0;
    int num = -1;
    for (Bee*& bee : bees_)
    {
        ++num;

        DirectX::XMFLOAT3 beePos = bee->GetTransform()->GetPosition();
        beePos.y = 0;
        position.y = 0;

        float len = DirectX::XMVectorGetX(DirectX::XMVector3Length(DirectX::XMVectorSubtract(DirectX::XMLoadFloat3(&beePos), DirectX::XMLoadFloat3(&position))));

        if (mostNearLength > len)
        {
            mostNearLength = len;
            mostNearBeeIndex = num;
        }
    }

    return mostNearBeeIndex;
}
