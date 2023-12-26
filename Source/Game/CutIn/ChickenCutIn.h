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
    void DrawDebug();

    // --- ステートマシン取得 ---
    StateMachine<State<ChickenCutIn>>* GetStateMachine() { return stateMachine_.get(); }

    Model* GetModel() { return model_.get(); }
    
    Sprite* GetBlackSprite0() { return blackSprite_[0].get(); }
    Sprite* GetBlackSprite1() { return blackSprite_[1].get(); }
    Sprite* GetWhiteSprite() { return whiteSprite_.get(); }


    DirectX::XMFLOAT3 offset_ = {};
private:
    void Initialize();    

private:
    // ---------- ステートマシン -------------------------
    std::unique_ptr<StateMachine<State<ChickenCutIn>>> stateMachine_;
    int currentState_ = 0;

    // --------------------------------------------------


    bool isPlay_ = false; // 再生中か

    std::unique_ptr<Model> model_;
    std::unique_ptr<Sprite> blackSprite_[2];
    std::unique_ptr<Sprite> whiteSprite_;
};

