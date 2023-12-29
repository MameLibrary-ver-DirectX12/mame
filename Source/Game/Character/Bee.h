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
        const DirectX::XMMATRIX& world = {}) override;  // �`��
    void DrawDebug()                        override;   // ImGui�p

public:// --- �擾�E�ݒ� ---
#pragma region [Get, Set] Function
    StateMachine<State<Bee>>* GetStateMachine() { return stateMachine_.get(); }
    void ChangeState(const STATE& state) { GetStateMachine()->ChangeState(static_cast<UINT>(state)); }

    float GetMoveSpeed() { return moveSpeed_; }
    void SetMoveSpeed(const float& speed) { moveSpeed_ = speed; }

    int GetFlowerIndex() { return flowerIndex_; }
    void SetFlowerIndex(const int& index) { flowerIndex_ = index; }

    float GetRadius() { return radius_; }
    void SetRadius(const float& radius) { radius_ = radius; }

    float GetMaxCollectTime() { return maxCollectTime_; }
    void SetMaxCollectTime(const float& time) { maxCollectTime_ = time; }

    float GetCollectTimer() { return collectTimer_; }
    void SetCollectTimer(const float& time) { collectTimer_ = time; }
    void AddCollectTimer(const float& time) { collectTimer_ += time; }

#pragma endregion [Get, Set] Function

private:
    // ---------- �X�e�[�g�}�V�� ------------------------------
    std::unique_ptr<StateMachine<State<Bee>>> stateMachine_;

    // -------------------------------------------------------

    float moveSpeed_ = 0.0f; // �ړ����x

    int flowerIndex_ = 0; // �Ԃ̔ԍ�

    float radius_ = 0;  // �I�̑̂̑傫��

    float maxCollectTime_ = 0.0f;   // �͂��݂̎�ɂ����鎞��
    float collectTimer_ = 0.0f;     // �͂��݂̎悷�鎞�p�̃^�C�}�[
};

