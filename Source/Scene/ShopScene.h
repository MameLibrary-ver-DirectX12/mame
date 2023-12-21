#pragma once
#include "BaseScene.h"

#include <memory>

#include "../Resource/Sprite.h"

class ShopScene : public BaseScene
{
public:
    enum class STATE
    {
        FadeIn,
    };

public:
    ShopScene();
    ~ShopScene() override {}

    void CreateResource()                               override; // ���\�[�X����
    void Initialize()                                   override; // ������
    void Finalize()                                     override; // �I����
    void Update(const float& elapsedTime)               override; // �X�V����
    void Render(ID3D12GraphicsCommandList* commandList) override; // �`�揈��
    void DrawDebug()                                    override; // ImGui�p

private:



};

