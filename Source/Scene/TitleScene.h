#pragma once
#include "BaseScene.h"

#include <memory>

#include "../AI/StateMachine.h"

#include "../Graphics/SkyMap.h"

#include "../Resource/Sprite.h"
#include "../Resource/Model.h"

#include "../Game/Stage/StageNormal.h"
#include "../Game/Character/Player.h"

class TitleScene : public BaseScene
{
public:// 定数
#pragma region 定数
    enum class STATE
    {
        FadeIn,
    };

#pragma endregion 定数

public:
    TitleScene();
    ~TitleScene() override {}

    void CreateResource()                               override; // リソース生成
    void Initialize()                                   override; // 初期化
    void Finalize()                                     override; // 終了化
    void Update(const float& elapsedTime)               override; // 更新処理
    void Render(ID3D12GraphicsCommandList* commandList) override; // 描画処理
    void UIRender(ID3D12GraphicsCommandList* commandList)   override;
    void DrawDebug()                                    override; // ImGui用


public:// 取得・設定
#pragma region [Get, Set] Function
    // ----- ステートマシン -----
    StateMachine<State<TitleScene>>* GetStateMachine() const { return stateMachine_.get(); }

    // --- 現在のステート ---
    int GetCurrentState() { return currentState_; }
    void SetCurrentState(STATE state) { currentState_ = static_cast<UINT>(state); }

#pragma endregion [Get, Set] Function

public:// ステートマシンで触るために public で定義
    // ---------- Sprite ------------------------------
    std::unique_ptr<Sprite> backSprite_;            // 背景
    std::unique_ptr<Sprite> titleLogoSprite_;       // Emma
    std::unique_ptr<Sprite> pressAnyButtonSprite_;  // PressAnyButton
    std::unique_ptr<Sprite> loadGameSprite_;        // LoadGame
    std::unique_ptr<Sprite> quitGameSprite_;        // QuitGame
    // ------------------------------------------------

private:
    // ---------- ステートマシン -----------------------
    std::unique_ptr<StateMachine<State<TitleScene>>> stateMachine_ = nullptr;
    int currentState_ = 0; // 現在のステート
    // ------------------------------------------------

    std::unique_ptr<Stage>  stage_;
    std::unique_ptr<SkyMap> skyMap_;
    std::unique_ptr<Player> player_;
};

