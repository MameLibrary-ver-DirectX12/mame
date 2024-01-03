#include "FlowerManager.h"
#include "../../Other/Common.h"

// --- 初期化 ---
void FlowerManager::Initialize()
{
    for (Flower*& flower : flowers_)
    {
        flower->Initialize();
    }
}

// --- 終了化 ---
void FlowerManager::Finalize()
{
    for (Flower*& flower : flowers_)
    {
        flower->Finalize();
    }
    Clear();
}

// --- 更新 ---
void FlowerManager::Update(const float& elapsedTime)
{
    // --- 更新 ---
    for (Flower*& flower : flowers_)
    {
        flower->Update(elapsedTime);
    }

    // --- 開放 ---
    for (Flower* flower : removes_)
    {
        std::vector<Flower*>::iterator it = std::find(flowers_.begin(), flowers_.end(), flower);

        if (it != flowers_.end())
        {
            flowers_.erase(it);
        }

        SafeDeletePtr(flower);
    }
    removes_.clear();
}

// --- 描画 ---
void FlowerManager::Render(ID3D12GraphicsCommandList* commandList)
{
    for (Flower*& flower : flowers_)
    {
        flower->Render(commandList);
    }
}

// --- ImGui用 ---
void FlowerManager::DrawDebug()
{
    if (ImGui::BeginMenu("FlowerManager"))
    {
        for (Flower*& flower : flowers_)
        {
            flower->DrawDebug();
        }

        ImGui::EndMenu();
    }
}

// --- 蜂にペアを見つけてあげる ---
int FlowerManager::SearchPairIndex()
{
    // todo : 花が死んだとき蜂さんに報告しないといけない。
    // でないと多分バグる

    int num = -1; // 0から始めるために最初は-1にしておく

    for (Flower*& flower : flowers_)
    {
        ++num; // 番号を数える

        // --- すでにペアがいるときは戻る ---
        if (flower->GetIsPaired()) continue;

        // --- ペアになった ---
        flower->SetIsPaired(true);

        return num; // vectorのインデックスを返す
    }

    return -1; // 失敗
}

// --- ペア解除 ---
void FlowerManager::RemovePair(const int& index)
{
    int num = -1;

    for (Flower*& flower : flowers_)
    {
        ++num; // 番号を数える

        if (num != index) continue;
     
        // ペア解除
        flower->SetIsPaired(false);

        return;
    }
}

// --- 登録 ---
void FlowerManager::Register(Flower* flower)
{
    flowers_.emplace_back(flower);
}

// --- 全削除 ---
void FlowerManager::Clear()
{
    for (Flower*& flower : flowers_)
    {
        SafeDeletePtr(flower);
    }
    flowers_.clear();
    flowers_.shrink_to_fit();
}

// --- 削除 ---
void FlowerManager::Remove(Flower* flower)
{
    removes_.insert(flower);
}
