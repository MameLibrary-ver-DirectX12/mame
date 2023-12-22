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

    // --- �X�e�[�g�}�V���擾 ---
    StateMachine<State<ChickenCutIn>>* GetStateMachine() { return stateMachine_.get(); }

private:
    void Initialize();    

private:
    // ---------- �X�e�[�g�}�V�� -------------------------
    std::unique_ptr<StateMachine<State<ChickenCutIn>>> stateMachine_;
    int currentState_ = 0;

    // --------------------------------------------------


    bool isPlay_ = false; // �Đ�����
};

