#pragma once
#include "Character.h"

class Flower : public Character
{
public:// --- 定数 ---
    enum class FlowerType
    {
        Blue,
    };

public:
    Flower(const char* fbxFilename);
    ~Flower() override {}

    void Initialize()                       override {} // 初期化 
    void Finalize()                         override {} // 終了化
    void Update(const float& elapsedTime)   override {} // 更新
    void Render(ID3D12GraphicsCommandList* commandList,
        const DirectX::XMMATRIX world = {}) override;   // 描画

    void DrawDebug()                        override {} // ImGui用

public:// --- 取得・設定 ---
#pragma region [Get, Set] Function
    // --- 花の種類 ---
    int GetFlowerType() { return static_cast<int>(type_); }
    void SetFlowerType(const FlowerType& type) { type_ = type; }

    // --- 蜂さんとのペア ---
    bool GetIsPaired() { return isPaired_; }
    void SetIsPaired(const bool& pair) { isPaired_ = pair; }

    // --- はちみつ採取範囲 ---
    float GetRadius() { return radius_; }
    void SetRadius(const float& radius) { radius_ = radius; }

    // --- 採取にかかる時間 ---
    float GetCollectTime() { return collectTime_; }
    void SetCollectTime(float time) { collectTime_ = time; }

#pragma endregion [Get, Set] Function

private:
    FlowerType type_    = FlowerType::Blue; // 花の種類
    bool isPaired_      = false;    // 蜂が自分のことを見つけてるか(一つにつき一つのため)
    float radius_       = 0.0f;     // 半径(蜂が三つを採取できる範囲)
    float collectTime_  = 0.0f;     // 採取できるまでの時間
};

