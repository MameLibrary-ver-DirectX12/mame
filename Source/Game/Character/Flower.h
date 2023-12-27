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
    int GetFlowerType() { return static_cast<int>(type_); }
    void SetFlowerType(FlowerType type) { type_ = type; }

    bool GetIsPaired() { return isPaired_; }
    void SetIsPaired(bool pair) { isPaired_ = pair; }

private:
    FlowerType type_ = FlowerType::Blue; // 花の種類
    bool isPaired_ = false; // 蜂が自分のことを見つけてるか(一つにつき一つのため)
};

