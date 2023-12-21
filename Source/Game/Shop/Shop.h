#pragma once
#include <d3d12.h>
#include <memory>

#include "../../../External/ImGui/imgui.h"
#include "../../../External/ImGui/imgui_impl_win32.h"
#include "../../../External/ImGui/imgui_impl_dx12.h"

#include "../../AI/StateMachine.h"

#include "../../Resource/Sprite.h"
#include "../../Resource/Model.h"

class Shop
{
public:// --- 定数 ---
    enum class STATE
    {
        FadeIn,
    };

    enum class ChoseModel
    {
        Turret,     // タレット
    };

private:
    Shop();
    ~Shop() {}

public:
    // --- インスタンス取得 ---
    static Shop& Instance()
    {
        static Shop instance;
        return instance;
    }
    
    void CreateResource();                                  // リソース生成
    void Initialize();                                      // 初期化
    void Finalize();                                        // 終了化
    void Update(const float& elapsedTime);                  // 更新処理
    void Render(ID3D12GraphicsCommandList* commandList);    // 描画処理
    void DrawDebug();                                       // ImGui用


public:// --- 取得・設定 ---
#pragma region [Get Set] Function
    // ----- ステートマシン -----
    StateMachine<State<Shop>>* GetStateMachine() const { return stateMachine_.get(); }
    // --- 現在のステート ---
    int GetCurrentState() { return currentState_; }
    void SetCurrentState(STATE state) { currentState_ = static_cast<UINT>(state); }

    // --- 現在選択しているモデル番号 ---
    int GetCurrentChoseModelNum() { return currentChoseModel_; }
    void SetCurrentChoseModelNum(int num) { currentChoseModel_ = num; }

#pragma endregion [Get Set] Function


private:
    // ---------- ステートマシン -------------------------
    std::unique_ptr<StateMachine<State<Shop>>> stateMachine_;
    int currentState_ = 0;

    // --------------------------------------------------

    // ---------- Sprite --------------------------------
    Sprite* whiteSprite_ = nullptr;

    // --------------------------------------------------

    // ---------- Model ---------------------------------
    Model* podium_ = nullptr;   // 台。（演台,表彰台てきな...）
    Model* turret_ = nullptr;   // タレット

    // --------------------------------------------------

    // ---------- リソース生成管理用 ----------------------
    bool isCreateResources_ = false;
    // --------------------------------------------------

    // ---------- 変数 ----------------------------------
    int currentChoseModel_ = 0; // 現在選択しているモデル番号

    // --------------------------------------------------
};

