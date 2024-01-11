#pragma once
#include "BaseScene.h"

#include <memory>

#include "../AI/StateMachine.h"

#include "../Graphics/SkyMap.h"

#include "../Resource/Sprite.h"
#include "../Resource/Model.h"

#include "../Game/Stage/StageNormal.h"
#include "../Game/Character/Player.h"

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
    void UIRender(ID3D12GraphicsCommandList* commandList)   override;
    void DrawDebug()                                    override; // ImGui�p


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

    std::unique_ptr<Stage>  stage_;
    std::unique_ptr<SkyMap> skyMap_;
    std::unique_ptr<Player> player_;
};

