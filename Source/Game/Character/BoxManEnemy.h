#pragma once
#include "Enemy.h"

class BoxManEnemy : public Enemy
{
public:
    BoxManEnemy();
    ~BoxManEnemy();

    void Initialize()                       override; // 初期化
    void Finalize()                         override; // 終了化  
    void Update(const float& elapsedTime)   override; // 更新
    void Render(ID3D12GraphicsCommandList* commandList,
        const DirectX::XMMATRIX& world = {})override; // 描画
    void DrawDebug()                        override; // ImGui用
};

