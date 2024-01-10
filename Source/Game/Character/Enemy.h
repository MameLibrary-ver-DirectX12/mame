#pragma once
#include "Character.h"

class Enemy : public Character
{
public:
    Enemy(const char* fbxFilename, bool triangulate = true, float samplingRate = 0);
    ~Enemy() {}

    void Initialize()                       override {} // 初期化 
    void Finalize()                         override {} // 終了化
    void Update(const float& elapsedTime)   override {} // 更新
    void Render(ID3D12GraphicsCommandList* commandList,
        const DirectX::XMMATRIX& world = {}) override;   // 描画

    void DrawDebug()                        override {} // ImGui用

public:// --- 取得・設定 ---
    int GetType() { return type_; }
    void SetType(const int& type) { type_ = type; }        

private:
    int type_ = 0;
};

