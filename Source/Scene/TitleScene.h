#pragma once
#include "BaseScene.h"

#include <memory>

#include "../AI/StateMachine.h"

#include "../Resource/Sprite.h"
#include "../Resource/Model.h"


class TitleScene : public BaseScene
{
public:// �萔
#pragma region �萔
    enum class STATE
    {
        FadeIn,
    };

#pragma endregion �萔

public:
    TitleScene();
    ~TitleScene() override {}

    void CreateResource()                               override; // ���\�[�X����
    void Initialize()                                   override; // ������
    void Finalize()                                     override; // �I����
    void Update(const float& elapsedTime)               override; // �X�V����
    void Render(ID3D12GraphicsCommandList* commandList) override; // �`�揈��
    void DrawDebug()                                    override; // ImGui�p

private:
    void CreateRootSignature();     // ���[�g�V�O�l�`������
    void CreateGraphicsPipeline();  // �O���t�B�b�N�X�p�C�v���C���쐬

public:// �擾�E�ݒ�
#pragma region [Get, Set] Function
    // ----- �X�e�[�g�}�V�� -----
    StateMachine<State<TitleScene>>* GetStateMachine() const { return stateMachine_.get(); }

    // --- ���݂̃X�e�[�g ---
    int GetCurrentState() { return currentState_; }
    void SetCurrentState(STATE state) { currentState_ = static_cast<UINT>(state); }

#pragma endregion [Get, Set] Function

public:// �X�e�[�g�}�V���ŐG�邽�߂� public �Œ�`
    // ---------- Sprite ------------------------------
    std::unique_ptr<Sprite> backSprite_;            // �w�i
    std::unique_ptr<Sprite> titleLogoSprite_;       // Emma
    std::unique_ptr<Sprite> pressAnyButtonSprite_;  // PressAnyButton
    std::unique_ptr<Sprite> loadGameSprite_;        // LoadGame
    std::unique_ptr<Sprite> quitGameSprite_;        // QuitGame
    // ------------------------------------------------

private:
    // ---------- �X�e�[�g�}�V�� -----------------------
    std::unique_ptr<StateMachine<State<TitleScene>>> stateMachine_ = nullptr;
    int currentState_ = 0; // ���݂̃X�e�[�g
    // ------------------------------------------------



    std::shared_ptr<Model> model_;

    // ---------- �\���Ɋւ������ --------------------
    D3D12_GRAPHICS_PIPELINE_STATE_DESC graphicsPipeline_ = {};
    
    Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState_ = {};
    Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature_ = {};

    // -----------------------------------------------
};

