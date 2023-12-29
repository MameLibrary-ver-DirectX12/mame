#pragma once
#include "Character.h"
#include "../../AI/StateMachine.h"
#include "../../Input/Input.h"

class Player : public Character
{
public:// --- �萔 ---
#pragma region �萔
    enum class STATE
    {
        Idle,
        Move,
    };

#pragma endregion �萔

public:
    Player();
    ~Player();


    void Initialize()                                   override;   // ������
    void Finalize()                                     override;   // �I����
    void Update(const float& elapsedTime)               override;   // �X�V
    void Render(ID3D12GraphicsCommandList* commandList,
        const DirectX::XMMATRIX& world = {})             override;  // �`��
    void DrawDebug()                                    override;   // ImGui�p

public:// --- �擾�E�ݒ� ---
    StateMachine<State<Player>>* GetStateMachine() { return stateMachine_.get(); }
    void ChangeState(STATE state) { GetStateMachine()->ChangeState(static_cast<UINT>(state)); }

    // --- ���x ---
    DirectX::XMFLOAT3 GetVelocity() { return velocity_; }
    void SetVelocity(DirectX::XMFLOAT3 velocity) { velocity_ = velocity; }

    // --- �X�L�� ---
    int GetFlowerNum() { return flowerNum_; }
    void SetFlowerNum(int num) { flowerNum_ = num; }
    bool UseFlower() 
    { 
        if (flowerNum_)
        {
            --flowerNum_;
            return true;
        }
        return false;
    }

private:
    // ---------- �X�e�[�g�}�V�� -----------------------------------
    std::unique_ptr<StateMachine<State<Player>>> stateMachine_;

    // ------------------------------------------------------------

    DirectX::XMFLOAT3 velocity_ = {};

    int animationIndex_ = 0;

    // ----- �X�L�� -----
    int flowerNum_ = 0; // �������Ă���Ԃ̐�

};

