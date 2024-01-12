#pragma once
#include "BaseScene.h"

#include "../Graphics/FrameBuffer.h"

#include "../Resource/Sprite.h"
#include "../Game/Stage/StageNormal.h"
#include "../Game/Character/Honey.h"

#include "../Game/CutIn/ChickenCutIn.h"

#include "../Game/UI/UI.h"

class GameScene : public BaseScene
{
public:
    GameScene();
    ~GameScene() override {}

    void CreateResource()                                       override; // リソース生成
    void Initialize()                                           override; // 初期化
    void Finalize()                                             override; // 終了化
    void Update(const float& elapsedTime)                       override; // 更新処理
    void Render(ID3D12GraphicsCommandList* commandList)         override; // 描画処理
    void UIRender(ID3D12GraphicsCommandList* commandList)       override; // UI描画
    void ShadowRender(ID3D12GraphicsCommandList* commandList)   override; // 影成分抽出
    void DrawDebug()                                            override; // ImGui用

private:
    std::unique_ptr<StageNormal> stage_;

    std::unique_ptr<ChickenCutIn> chickenCutIn_;

    std::unique_ptr<Honey> honey_; // はちみつ(守るもの)

    std::unique_ptr<GameUI> ui_;

    std::unique_ptr<FrameBuffer> frameBuffer_;

};

