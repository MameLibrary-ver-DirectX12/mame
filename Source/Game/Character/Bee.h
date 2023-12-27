#pragma once
#include "Character.h"
#include "../../AI/StateMachine.h"

class Bee : public Character
{
public:// --- �萔 ---
    enum class STATE
    {
        Search,             // �y�A��T��
        MoveToFlower,       // �ԂɌ������Ĉړ� 
        Collect,            // �����̎�
        MoveToStoragePlace, // �u���ꏊ�Ɉړ�
    };

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
    void ChangeState(STATE state) { GetStateMachine()->ChangeState(static_cast<UINT>(state)); }

    int GetFlowerIndex() { return flowerIndex_; }
    void SetFlowerIndex(int index) { flowerIndex_ = index; }

private:
    // ---------- �X�e�[�g�}�V�� ------------------------------
    std::unique_ptr<StateMachine<State<Bee>>> stateMachine_;

    // -------------------------------------------------------

    int flowerIndex_ = 0; // �Ԃ̔ԍ�

};

