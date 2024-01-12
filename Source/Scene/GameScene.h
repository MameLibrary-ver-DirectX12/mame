#pragma once
#include "BaseScene.h"

#include "../Graphics/FrameBuffer.h"

#include "../Resource/Sprite.h"
#include "../Game/Stage/StageNormal.h"
#include "../Game/Character/Honey.h"

#include "../Game/CutIn/ChickenCutIn.h"

#include "../Game/UI/UI.h"

class GameScene : public BaseScene
{
public:
    GameScene();
    ~GameScene() override {}

    void CreateResource()                                       override; // ���\�[�X����
    void Initialize()                                           override; // ������
    void Finalize()                                             override; // �I����
    void Update(const float& elapsedTime)                       override; // �X�V����
    void Render(ID3D12GraphicsCommandList* commandList)         override; // �`�揈��
    void UIRender(ID3D12GraphicsCommandList* commandList)       override; // UI�`��
    void ShadowRender(ID3D12GraphicsCommandList* commandList)   override; // �e�������o
    void DrawDebug()                                            override; // ImGui�p

private:
    std::unique_ptr<StageNormal> stage_;

    std::unique_ptr<ChickenCutIn> chickenCutIn_;

    std::unique_ptr<Honey> honey_; // �͂��݂�(������)

    std::unique_ptr<GameUI> ui_;

    std::unique_ptr<FrameBuffer> frameBuffer_;

};

