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
        const DirectX::XMMATRIX& world = {}) override;  // 描画
    void DrawDebug()                        override;   // ImGui用

public:// --- 取得・設定 ---
#pragma region [Get, Set] Function
    StateMachine<State<Bee>>* GetStateMachine() { return stateMachine_.get(); }
    void ChangeState(const STATE& state) { GetStateMachine()->ChangeState(static_cast<UINT>(state)); }

    float GetMoveSpeed() { return moveSpeed_; }
    void SetMoveSpeed(const float& speed) { moveSpeed_ = speed; }

    int GetFlowerIndex() { return flowerIndex_; }
    void SetFlowerIndex(const int& index) { flowerIndex_ = index; }

    float GetRadius() { return radius_; }
    void SetRadius(const float& radius) { radius_ = radius; }

    float GetMaxCollectTime() { return maxCollectTime_; }
    void SetMaxCollectTime(const float& time) { maxCollectTime_ = time; }

    float GetCollectTimer() { return collectTimer_; }
    void SetCollectTimer(const float& time) { collectTimer_ = time; }
    void AddCollectTimer(const float& time) { collectTimer_ += time; }

#pragma endregion [Get, Set] Function

private:
    // ---------- ステートマシン ------------------------------
    std::unique_ptr<StateMachine<State<Bee>>> stateMachine_;

    // -------------------------------------------------------

    float moveSpeed_ = 0.0f; // 移動速度

    int flowerIndex_ = 0; // 花の番号

    float radius_ = 0;  // 蜂の体の大きさ

    float maxCollectTime_ = 0.0f;   // はちみつ採取にかかる時間
    float collectTimer_ = 0.0f;     // はちみつ採取する時用のタイマー
};

