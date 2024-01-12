#include "PoisonHoneyManager.h"
#include "../../Other/Common.h"

// --- 初期化 ---
void PoisonHoneyManager::Initialize()
{
    for (PoisonHoney*& poisonHoney : poisonHoneies_)
    {
        poisonHoney->Initialize();
    }
}

// --- 終了化 ---
void PoisonHoneyManager::Finalize()
{
    for (PoisonHoney*& poisonHoney : poisonHoneies_)
    {
        poisonHoney->Finalize();
    }
    Clear();
}

// --- 更新 ---
void PoisonHoneyManager::Update(const float& elapsedTime)
{   
    // 更新
    for (PoisonHoney*& poisonHoney : poisonHoneies_)
    {
        poisonHoney->Update(elapsedTime);
    }

    float minPosX = -100.0f;
    float maxPosX = 100.0f;
    float minPosZ = -100.0f;
    float maxPosZ = 16.0f;

    for (PoisonHoney*& poisonHoney : poisonHoneies_)
    {
        // 範囲外判定
        if (poisonHoney->GetTransform()->GetPositionX() < minPosX ||
            poisonHoney->GetTransform()->GetPositionZ() > maxPosX ||
            poisonHoney->GetTransform()->GetPositionZ() < minPosZ ||
            poisonHoney->GetTransform()->GetPositionZ() > maxPosZ)
        {
            poisonHoney->SetIsRemove(true);
            poisonHoney->SetIsRender(false);
        }

        // 消去登録されたやつをカウントする
        if (poisonHoney->GetIsRemove())
        {
            if (poisonHoney->GetRemoveCount() <= Graphics::Instance().GetBufferCount() + 1)
            {
                poisonHoney->AddRemoveCount();
            }
            else
            {
                Remove(poisonHoney);
            }
        }
    }    

    // 開放
    for (PoisonHoney* poisonHoney : removes_)
    {
        auto it = std::find(poisonHoneies_.begin(), poisonHoneies_.end(), poisonHoney);

        if (it != poisonHoneies_.end())
        {
            poisonHoneies_.erase(it);
        }
        SafeDeletePtr(poisonHoney);
    }
    removes_.clear();
}

// --- 描画 ---
void PoisonHoneyManager::Render(ID3D12GraphicsCommandList* commandList)
{
    for (PoisonHoney*& poisonHoney : poisonHoneies_)
    {
        if (poisonHoney->GetIsRender()) poisonHoney->Render(commandList);
    }
}

// --- ImGui用 ---
void PoisonHoneyManager::DrawDebug()
{
    for (PoisonHoney*& poisonHoney : poisonHoneies_)
    {
        poisonHoney->DrawDebug();
    }
}

// --- 登録 ---
void PoisonHoneyManager::Register(PoisonHoney* poisonHoney)
{
    poisonHoneies_.emplace_back(poisonHoney);

    // 所持数をカウントする (種類ごとに)
    ++possessionNum_[poisonHoney->GetType()];
}

// --- 全削除 ---
void PoisonHoneyManager::Clear()
{
    for (PoisonHoney*& poisonHoney : poisonHoneies_)
    {
        SafeDeletePtr(poisonHoney);
    }
    poisonHoneies_.clear();
    poisonHoneies_.shrink_to_fit();    

    // 所持数をリセット
    for (int typeIndex = 0; typeIndex < static_cast<int>(TYPE::Max); ++typeIndex)
    {
        possessionNum_[typeIndex] = 0;
    }
}

// --- 削除 ---
void PoisonHoneyManager::Remove(PoisonHoney* poisonHoney)
{
    // 所持数を減らす
    --possessionNum_[poisonHoney->GetType()];

    // 消去リストに登録
    removes_.insert(poisonHoney);
}

// --- 攻撃可能なポイズンハニーの数 ---
int PoisonHoneyManager::GetAbleAttckNum(TYPE type)
{
    int total = 0;

    for (PoisonHoney*& poisonHoney : poisonHoneies_)
    {
        // 違う種類
        if (poisonHoney->GetType() != static_cast<UINT>(type)) continue;

        // 攻撃可能数をカウントする
        if (poisonHoney->GetIsAbleAttack())
        {
            ++total;
        }

    }

    return total;
}

// --- 攻撃 ---
bool PoisonHoneyManager::Shot(TYPE type)
{
    int num = 0;

    for (PoisonHoney*& poisonHoney : poisonHoneies_)
    {
        // 違う種類
        if (poisonHoney->GetType() != static_cast<UINT>(type)) continue;

        // 攻撃できない
        if (!poisonHoney->GetIsAbleAttack()) continue;

        // 攻撃
        poisonHoney->SetState(PoisonHoney::STATE::Attack);
        return true;
    }

    return false;
}
