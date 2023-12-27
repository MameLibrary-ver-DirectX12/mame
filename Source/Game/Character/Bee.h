#pragma once
#include "Character.h"
#include "../../AI/StateMachine.h"

class Bee : public Character
{
public:// --- 定数 ---
    enum class STATE
    {
        Search,             // ペアを探す
        MoveToFlower,       // 花に向かって移動 
        Collect,            // 蜜を採取
        MoveToStoragePlace, // 置く場所に移動
    };

public:
    Bee();
    ~Bee() override {}

    void Initialize()                       override;   // 初期化 
    void Finalize()                         override;   // 終了化
    void Update(const float& elapsedTime)   override;   // 更新
    void Render(ID3D12GraphicsCommandList* commandList,
        const DirectX::XMMATRIX world = {}) override;   // 描画

    void DrawDebug()                        override;   // ImGui用

public:// --- 取得・設定 ---
    StateMachine<State<Bee>>* GetStateMachine() { return stateMachine_.get(); }
    void ChangeState(STATE state) { GetStateMachine()->ChangeState(static_cast<UINT>(state)); }

    int GetFlowerIndex() { return flowerIndex_; }
    void SetFlowerIndex(int index) { flowerIndex_ = index; }

private:
    // ---------- ステートマシン ------------------------------
    std::unique_ptr<StateMachine<State<Bee>>> stateMachine_;

    // -------------------------------------------------------

    int flowerIndex_ = 0; // 花の番号

};

