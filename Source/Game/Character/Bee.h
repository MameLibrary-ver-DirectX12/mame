#pragma once
#include "Character.h"
#include "../../AI/StateMachine.h"

class Bee : public Character
{
public:
    Bee();
    ~Bee() override {}

    void Initialize()                       override;   // 初期化 
    void Finalize()                         override;   // 終了化
    void Update(const float& elapsedTime)   override;   // 更新
    void Render(ID3D12GraphicsCommandList* commandList,
        const DirectX::XMMATRIX world = {}) override;   // 描画

    void DrawDebug()                        override;   // ImGui用

public:// --- 取得・設定 ---
    StateMachine<State<Bee>>* GetStateMachine() { return stateMachine_.get(); }


private:
    // ---------- ステートマシン ------------------------------
    std::unique_ptr<StateMachine<State<Bee>>> stateMachine_;

    // -------------------------------------------------------

};

