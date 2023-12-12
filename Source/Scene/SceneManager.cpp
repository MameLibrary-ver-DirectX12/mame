#include "SceneManager.h"

#include "../Graphics/Graphics.h"

// ----- 更新 -----
void SceneManager::Update(const float& elapsedTime)
{
    if (nextScene_)
    {
        Graphics::Instance().WaitIdle();

        // --- 古いシーンを終了 ---
        Clear();

        // --- 新しいシーンを設定 ---
        currentScene_ = nextScene_;
        nextScene_ = nullptr;

        // --- シーン初期化処理 ( マルチスレッド処理をしていないときにここを通る ) ---
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

// ----- 描画 -----
void SceneManager::Render(ID3D12GraphicsCommandList* commandList)
{
    if (!currentScene_)return;

    currentScene_->Render(commandList);
}

// ----- シーンクリア -----
void SceneManager::Clear()
{
    if (!currentScene_)return;

    currentScene_->Finalize();
    delete currentScene_;
    currentScene_ = nullptr;
}

// ----- シーン切り替え -----
void SceneManager::ChangeScene(BaseScene* scene)
{
    // --- 新しいシーンを設定 ---
    nextScene_ = scene;
}
