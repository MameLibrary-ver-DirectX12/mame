#pragma once
#include "Character.h"
#include "../../AI/StateMachine.h"

class Bee : public Character
{
public:
    Bee();
    ~Bee() override {}

    void Initialize()                       override;   // ������ 
    void Finalize()                         override;   // �I����
    void Update(const float& elapsedTime)   override;   // �X�V
    void Render(ID3D12GraphicsCommandList* commandList,
        const DirectX::XMMATRIX world = {}) override;   // �`��

    void DrawDebug()                        override;   // ImGui�p

public:// --- �擾�E�ݒ� ---
    StateMachine<State<Bee>>* GetStateMachine() { return stateMachine_.get(); }


private:
    // ---------- �X�e�[�g�}�V�� ------------------------------
    std::unique_ptr<StateMachine<State<Bee>>> stateMachine_;

    // -------------------------------------------------------

};

