#pragma once
#include "BaseScene.h"

#include <thread>
#include <memory>

class LoadingScene : public BaseScene
{
public:
    LoadingScene(BaseScene* nextScene);
    ~LoadingScene() override {}

    void CreateResource()                               override; // ���\�[�X����
    void Initialize()                                   override; // ������
    void Finalize()                                     override; // �I����
    void Update(const float& elapsedTime)               override; // �X�V����
    void Render(ID3D12GraphicsCommandList* commandList) override; // �`�揈��
    void DrawDebug()                                    override; // ImGui�p

private:
    // ---------- �X���b�h�֌W ----------
    static void LoadingThread(LoadingScene* scene);
    BaseScene* nextScene_       = nullptr;
    std::thread* thread_    = nullptr;

};

