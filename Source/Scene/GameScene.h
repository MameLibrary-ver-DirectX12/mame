#pragma once
#include "BaseScene.h"

#include "../Resource/Sprite.h"
#include "../Game/Stage/StageNormal.h"

#include "../Game/CutIn/ChickenCutIn.h"

class GameScene : public BaseScene
{
public:
    GameScene();
    ~GameScene() override {}

    void CreateResource()                               override; // ���\�[�X����
    void Initialize()                                   override; // ������
    void Finalize()                                     override; // �I����
    void Update(const float& elapsedTime)               override; // �X�V����
    void Render(ID3D12GraphicsCommandList* commandList) override; // �`�揈��
    void DrawDebug()                                    override; // ImGui�p

private:
    std::unique_ptr<StageNormal> stage_;
    std::unique_ptr<StageNormal> safeZone_;

    std::unique_ptr<ChickenCutIn> chickenCutIn_;

};

