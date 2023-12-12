#include "SceneManager.h"

#include "../Graphics/Graphics.h"

// ----- �X�V -----
void SceneManager::Update(const float& elapsedTime)
{
    if (nextScene_)
    {
        Graphics::Instance().WaitIdle();

        // --- �Â��V�[�����I�� ---
        Clear();

        // --- �V�����V�[����ݒ� ---
        currentScene_ = nextScene_;
        nextScene_ = nullptr;

        // --- �V�[������������ ( �}���`�X���b�h���������Ă��Ȃ��Ƃ��ɂ�����ʂ� ) ---
        if (!currentScene_->IsReady())
        {
            currentScene_->CreateResource();
            currentScene_->Initialize();
        }
    }

    if (currentScene_)
    {
        currentScene_->Update(elapsedTime);

#ifdef _DEBUG
        currentScene_->DrawDebug();

#endif// _DEBUG
    }
}

// ----- �`�� -----
void SceneManager::Render(ID3D12GraphicsCommandList* commandList)
{
    if (!currentScene_)return;

    currentScene_->Render(commandList);
}

// ----- �V�[���N���A -----
void SceneManager::Clear()
{
    if (!currentScene_)return;

    currentScene_->Finalize();
    delete currentScene_;
    currentScene_ = nullptr;
}

// ----- �V�[���؂�ւ� -----
void SceneManager::ChangeScene(BaseScene* scene)
{
    // --- �V�����V�[����ݒ� ---
    nextScene_ = scene;
}
