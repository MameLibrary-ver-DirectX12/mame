#include "TitleScene.h"

#include <wrl.h>

#include "../Input/Input.h"
#include "../Graphics/Graphics.h"
#include "../Graphics/Camera.h"
#include "../Graphics/Shader.h"
#include "../Other/Misc.h"

#include "GameScene.h"
#include "LoadingScene.h"
#include "SceneManager.h"

#include "../Resource/ModelResourceManager.h"

// ----- コンストラクタ -----
TitleScene::TitleScene()
{
}

// ----- リソース生成 -----
void TitleScene::CreateResource()
{
    backSprite_ = std::make_unique<Sprite>("./Resources/Image/Title/White.png", "Back");
    titleLogoSprite_ = std::make_unique<Sprite>("./Resources/Image/Title/Emma.png", "Emma");
    pressAnyButtonSprite_ = std::make_unique<Sprite>("./Resources/Image/Title/PressAnyButton.png", "PressAnyButton");
    loadGameSprite_ = std::make_unique<Sprite>("./Resources/Image/Title/LoadGame.png", "LoadGame");
    quitGameSprite_ = std::make_unique<Sprite>("./Resources/Image/Title/QuitGame.png", "QuitGame");

    stage_ = std::make_unique<StageNormal>("./Resources/Model/Stage/stage.fbx");
    skyMap_ = std::make_unique<SkyMap>();
    player_ = std::make_unique<Player>();
}

// ----- 初期化 -----
void TitleScene::Initialize()
{
    backSprite_->GetTransform()->SetColorBlack();

    pressAnyButtonSprite_->GetTransform()->SetPos(480, 450);
    pressAnyButtonSprite_->GetTransform()->SetSize(300, 50);
    pressAnyButtonSprite_->GetTransform()->SetColorA(1.0f);

    loadGameSprite_->GetTransform()->SetPos(475, 420);
    loadGameSprite_->GetTransform()->SetSize(300, 50);
    loadGameSprite_->GetTransform()->SetColorA(1.0f);

    quitGameSprite_->GetTransform()->SetPos(475, 480);
    quitGameSprite_->GetTransform()->SetSize(300, 50);
    quitGameSprite_->GetTransform()->SetColorA(1.0f);    

    // --- カメラ初期化 ---
    Camera::Instance().Initialize();
    
    player_->Initialize();
}

// ----- 終了化 -----
void TitleScene::Finalize()
{
}

// ----- 更新 -----
void TitleScene::Update(const float& elapsedTime)
{
    GamePad& gamePad = Input::Instance().GetGamePad();

    backSprite_->Update(elapsedTime);
    titleLogoSprite_->Update(elapsedTime);
    pressAnyButtonSprite_->Update(elapsedTime);
    loadGameSprite_->Update(elapsedTime);
    quitGameSprite_->Update(elapsedTime);

    player_->Update(elapsedTime);

    if (gamePad.GetButtonDown() & GamePad::BTN_A)
    {        
        SceneManager::Instance().ChangeScene(new LoadingScene(new GameScene));
        return;
    }
}

// ----- 描画 -----
void TitleScene::Render(ID3D12GraphicsCommandList* commandList)
{
    Camera& camera = Camera::Instance();

    camera.SetPerSpectiveFov();


    // --- Model ---
    {
        skyMap_->Render(commandList);

        Graphics::Instance().GetShader()->SetLambertShader(commandList);

        stage_->Render(commandList);

        player_->Render(commandList);
    }

    //backSprite_->Render(commandList);
    //titleLogoSprite_->Render(commandList);
    //pressAnyButtonSprite_->Render(commandList);
    //loadGameSprite_->Render(commandList);
    //quitGameSprite_->Render(commandList);
}

// ----- ImGui用 -----
void TitleScene::DrawDebug()
{
    ImGui::Begin("TitleScene");
    
    stage_->DrawDebug();

    Camera::Instance().DrawDebug();

    player_->DrawDebug();

    if (ImGui::BeginMenu("Sprite"))
    {
        backSprite_->DrawDebug();
        titleLogoSprite_->DrawDebug();
        pressAnyButtonSprite_->DrawDebug();
        loadGameSprite_->DrawDebug();
        quitGameSprite_->DrawDebug();

        ImGui::EndMenu();
    }

    ImGui::End();
}