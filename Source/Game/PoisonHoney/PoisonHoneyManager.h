#pragma once
#include "PoisonHoney.h"
#include <vector>
#include <set>

class PoisonHoneyManager
{
public:// --- 定数 ---
    enum class TYPE // 種類
    {
        Normal,
        Max,
    };

private:
    PoisonHoneyManager() {}
    ~PoisonHoneyManager() {}

public:
    static PoisonHoneyManager& Instance()
    {
        static PoisonHoneyManager instance;
        return instance;
    }

    void Initialize();                                      // 初期化
    void Finalize();                                        // 終了化
    void Update(const float& elapsedTime);                  // 更新
    void Render(ID3D12GraphicsCommandList* commandList);    // 描画
    void DrawDebug();                                       // ImGui用

    void Register(PoisonHoney* poisonHoney);                // 登録
    void Clear();                                           // 全削除
    void Remove(PoisonHoney* poisonHoney);                  // 削除

private:
    int possessionNum_[static_cast<int>(TYPE::Max)] = {};   // 各ポイズンハニーの所持数

    std::vector<PoisonHoney*>   poisonHoneies_  = {};
    std::set<PoisonHoney*>      removes_        = {};
};

