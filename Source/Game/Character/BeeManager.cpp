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
    Clear();
}

// --- 更新 ---
void BeeManager::Update(const float& elapsedTime)
{
    for (Bee*& bee : bees_)
    {
        // 更新
        bee->Update(elapsedTime);

        // 消去登録されたやつをカウントする
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

    // 開放
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

// --- 引数の位置から最も近い蜂を返す ---
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

// --- 引数の位置から最も近い蜂のインデックスを返す ---
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
