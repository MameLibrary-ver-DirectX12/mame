#include "GameScene.h"

#include "../Graphics/Graphics.h"
#include "../Input/Input.h"

#include "TitleScene.h"
#include "LoadingScene.h"
#include "SceneManager.h"

// ----- �R���X�g���N�^ -----
GameScene::GameScene()
{
}

// ----- ���\�[�X���� -----
void GameScene::CreateResource()
{
}

// ----- ������ -----
void GameScene::Initialize()
{
}

// ----- �I���� -----
void GameScene::Finalize()
{
}

// ----- �X�V -----
void GameScene::Update(const float& elapsedTime)
{
    GamePad& gamePad = Input::Instance().GetGamePad();
    
    if (gamePad.GetButtonDown() & GamePad::BTN_A)
    {
        SceneManager::Instance().ChangeScene(new LoadingScene(new TitleScene));
        return;
    }
}

// ----- �`�� -----
void GameScene::Render(ID3D12GraphicsCommandList* commandList)
{
}

// ----- ImGui�p -----
void GameScene::DrawDebug()
{
}
