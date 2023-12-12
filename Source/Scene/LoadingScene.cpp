#include "LoadingScene.h"

#include <windows.h>

#include "SceneManager.h"

// ----- コンストラクタ -----
LoadingScene::LoadingScene(BaseScene* nextScene)
    : nextScene_(nextScene)
{
}

// ----- リソース生成 -----
void LoadingScene::CreateResource()
{
}

// ----- 初期化 -----
void LoadingScene::Initialize()
{
    // --- スレッド開始 ---
    thread_ = new std::thread(LoadingThread, this);
}

// ----- 終了化 -----
void LoadingScene::Finalize()
{
    // スレッド終了化
    thread_->join();
    if (thread_ != nullptr)
    {
        delete thread_;
        thread_ = nullptr;
    }
}

// ----- 更新 -----
void LoadingScene::Update(const float& elapsedTime)
{
    // 次のシーンが準備できたら
    if (nextScene_->IsReady())
    {
        SceneManager::Instance().ChangeScene(nextScene_);
        return;
    }
}

// ----- 描画 -----
void LoadingScene::Render(ID3D12GraphicsCommandList* commandList)
{
}

// ----- ImGui用 -----
void LoadingScene::DrawDebug()
{
}

// ----- スレッド -----
void LoadingScene::LoadingThread(LoadingScene* scene)
{
    // COM関連の初期化でスレッド毎に呼ぶ必要がある
    std::ignore = CoInitialize(nullptr); // std::ignoreで返り値警告解消

    // 次のシーンの初期化を行う
    scene->nextScene_->CreateResource();
    scene->nextScene_->Initialize();

    // スレッドが終わる前にCOM関連の終了化
    CoUninitialize();

    // 次のシーンの準備完了設定
    scene->nextScene_->SetReady();
}
