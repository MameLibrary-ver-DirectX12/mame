#pragma once

#include <memory>
#include "../../AI/StateMachine.h"

#include "../../Resource/Sprite.h"
#include "../../Resource/Model.h"

class ChickenCutIn
{
public:
    ChickenCutIn();
    ~ChickenCutIn();

    void Play(const float& elapsedTime);
    void Render(ID3D12GraphicsCommandList* commandList);

    // --- ステートマシン取得 ---
    StateMachine<State<ChickenCutIn>>* GetStateMachine() { return stateMachine_.get(); }

private:
    void Initialize();    

private:
    // ---------- ステートマシン -------------------------
    std::unique_ptr<StateMachine<State<ChickenCutIn>>> stateMachine_;
    int currentState_ = 0;

    // --------------------------------------------------


    bool isPlay_ = false; // 再生中か
};

