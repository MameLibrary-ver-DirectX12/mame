#include "GameScene.h"

#include "../Graphics/Graphics.h"
#include "../Graphics/Camera.h"
#include "../Input/Input.h"

#include "../Game/Character/PlayerManager.h"

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
    stage_ = std::make_unique<StageNormal>("./Resources/Model/Stage/StageBase.fbx");
    safeZone_ = std::make_unique<StageNormal>("./Resources/Model/Stage/SafeZone.fbx");

    chickenCutIn_ = std::make_unique<ChickenCutIn>();
}

// ----- 初期化 -----
void GameScene::Initialize()
{
    // --- カメラ初期化 ---
    Camera::Instance().InitializeGame();

    // --- プレイヤー初期化 ---
    PlayerManager::Instnace().Initialize();
    PlayerManager::Instnace().GetPlayer()->GetTransform()->SetScaleFactor(0.5f);

    stage_->GetTransform()->SetScaleFactor(10.0f);
    safeZone_->GetTransform()->SetScaleFactor(10.0f);
}

// ----- 終了化 -----
void GameScene::Finalize()
{
    // --- プレイヤー終了化 ---
    PlayerManager::Instnace().Finalize();
}

// ----- 更新 -----
void GameScene::Update(const float& elapsedTime)
{
    GamePad& gamePad = Input::Instance().GetGamePad();
    
    if (gamePad.GetButtonDown() & GamePad::BTN_A)
    {
        //SceneManager::Instance().ChangeScene(new LoadingScene(new TitleScene));
        //return;
    }

    chickenCutIn_->Play(elapsedTime);

    // --- カメラ更新 ---
    Camera::Instance().UpdateGame(elapsedTime);

    // --- プレイヤー更新 ---
    PlayerManager::Instnace().Update(elapsedTime);
}

// ----- 描画 -----
void GameScene::Render(ID3D12GraphicsCommandList* commandList)
{
    Camera::Instance().SetPerSpectiveFovGame();

    // --- Model ---
    {
        Graphics::Instance().GetShader()->SetLambertShader(commandList);

        // --- ステージ ---
        stage_->Render(commandList, stage_->GetTransform()->CalcWorldMatrix(1.0f));
        safeZone_->Render(commandList, safeZone_->GetTransform()->CalcWorldMatrix(1.0f));

        // --- プレイヤー ---
        PlayerManager::Instnace().Render(commandList);

        //chickenCutIn_->Render(commandList);
    }
}

// ----- ImGui用 -----
void GameScene::DrawDebug()
{
    ImGui::Begin("GameScene");

    // --- ステージ ---
    stage_->DrawDebug();
    safeZone_->DrawDebug();

    // --- プレイヤー ---
    PlayerManager::Instnace().DrawDebug();

    // --- カメラ ---
    Camera::Instance().DrawDebug();

    chickenCutIn_->DrawDebug();
    if (ImGui::Button("initialize")) chickenCutIn_->GetStateMachine()->ChangeState(0);

    ImGui::End();
}
