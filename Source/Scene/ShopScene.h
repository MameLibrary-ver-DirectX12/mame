#pragma once
#include "BaseScene.h"

#include <memory>

#include "../Resource/Sprite.h"

class ShopScene : public BaseScene
{
public:
    enum class STATE
    {
        FadeIn,
    };

public:
    ShopScene();
    ~ShopScene() override {}

    void CreateResource()                               override; // リソース生成
    void Initialize()                                   override; // 初期化
    void Finalize()                                     override; // 終了化
    void Update(const float& elapsedTime)               override; // 更新処理
    void Render(ID3D12GraphicsCommandList* commandList) override; // 描画処理
    void DrawDebug()                                    override; // ImGui用

private:



};

