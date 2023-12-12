#include "GameScene.h"

#include "../Graphics/Graphics.h"
#include "../Input/Input.h"

#include "TitleScene.h"
#include "LoadingScene.h"
#include "SceneManager.h"

// ----- コンストラクタ -----
GameScene::GameScene()
{
}

// ----- リソース生成 -----
void GameScene::CreateResource()
{
}

// ----- 初期化 -----
void GameScene::Initialize()
{
}

// ----- 終了化 -----
void GameScene::Finalize()
{
}

// ----- 更新 -----
void GameScene::Update(const float& elapsedTime)
{
    GamePad& gamePad = Input::Instance().GetGamePad();
    
    if (gamePad.GetButtonDown() & GamePad::BTN_A)
    {
        SceneManager::Instance().ChangeScene(new LoadingScene(new TitleScene));
        return;
    }
}

// ----- 描画 -----
void GameScene::Render(ID3D12GraphicsCommandList* commandList)
{
}

// ----- ImGui用 -----
void GameScene::DrawDebug()
{
}
