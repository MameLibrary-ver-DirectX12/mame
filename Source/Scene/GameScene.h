#pragma once
#include "BaseScene.h"

#include "../Resource/Sprite.h"
#include "../Game/Stage/StageNormal.h"

#include "../Game/CutIn/ChickenCutIn.h"

class GameScene : public BaseScene
{
public:
    GameScene();
    ~GameScene() override {}

    void CreateResource()                               override; // リソース生成
    void Initialize()                                   override; // 初期化
    void Finalize()                                     override; // 終了化
    void Update(const float& elapsedTime)               override; // 更新処理
    void Render(ID3D12GraphicsCommandList* commandList) override; // 描画処理
    void DrawDebug()                                    override; // ImGui用

private:
    std::unique_ptr<StageNormal> stage_;
    std::unique_ptr<StageNormal> safeZone_;

    std::unique_ptr<ChickenCutIn> chickenCutIn_;

};

