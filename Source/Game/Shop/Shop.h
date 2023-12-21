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
public:// --- �萔 ---
    enum class STATE
    {
        FadeIn,
    };

    enum class ChoseModel
    {
        Turret,     // �^���b�g
    };

private:
    Shop();
    ~Shop() {}

public:
    // --- �C���X�^���X�擾 ---
    static Shop& Instance()
    {
        static Shop instance;
        return instance;
    }
    
    void CreateResource();                                  // ���\�[�X����
    void Initialize();                                      // ������
    void Finalize();                                        // �I����
    void Update(const float& elapsedTime);                  // �X�V����
    void Render(ID3D12GraphicsCommandList* commandList);    // �`�揈��
    void DrawDebug();                                       // ImGui�p


public:// --- �擾�E�ݒ� ---
#pragma region [Get Set] Function
    // ----- �X�e�[�g�}�V�� -----
    StateMachine<State<Shop>>* GetStateMachine() const { return stateMachine_.get(); }
    // --- ���݂̃X�e�[�g ---
    int GetCurrentState() { return currentState_; }
    void SetCurrentState(STATE state) { currentState_ = static_cast<UINT>(state); }

    // --- ���ݑI�����Ă��郂�f���ԍ� ---
    int GetCurrentChoseModelNum() { return currentChoseModel_; }
    void SetCurrentChoseModelNum(int num) { currentChoseModel_ = num; }

#pragma endregion [Get Set] Function


private:
    // ---------- �X�e�[�g�}�V�� -------------------------
    std::unique_ptr<StateMachine<State<Shop>>> stateMachine_;
    int currentState_ = 0;

    // --------------------------------------------------

    // ---------- Sprite --------------------------------
    Sprite* whiteSprite_ = nullptr;

    // --------------------------------------------------

    // ---------- Model ---------------------------------
    Model* podium_ = nullptr;   // ��B�i����,�\����Ă���...�j
    Model* turret_ = nullptr;   // �^���b�g

    // --------------------------------------------------

    // ---------- ���\�[�X�����Ǘ��p ----------------------
    bool isCreateResources_ = false;
    // --------------------------------------------------

    // ---------- �ϐ� ----------------------------------
    int currentChoseModel_ = 0; // ���ݑI�����Ă��郂�f���ԍ�

    // --------------------------------------------------
};

