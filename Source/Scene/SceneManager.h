#pragma once
#include "BaseScene.h"

class SceneManager
{
private:
    SceneManager() {}
    ~SceneManager() {}

public:
    // --- �C���X�^���X�擾 ---
    static SceneManager& Instance()
    {
        static SceneManager instance;
        return instance;
    }

    void Update(const float& elapsedTime);                      // �X�V
    void Render(ID3D12GraphicsCommandList* commandList);        // �`��
    void UIRender(ID3D12GraphicsCommandList* commnadList);      // UI�`��
    void ShadowRender(ID3D12GraphicsCommandList* commandList);  // �e���o

    
    void Clear();                                           // �V�[���N���A

    void ChangeScene(BaseScene* scene);                         // �V�[���؂�ւ�

    BaseScene* GetCurrentScene() const { return currentScene_; }

    void ResetCountFrame() { countFrame_ = 0; }

private:
    BaseScene* currentScene_    = nullptr;
    BaseScene* nextScene_       = nullptr;

    int countFrame_ = 0; // ���������΍��p�ϐ�
};

