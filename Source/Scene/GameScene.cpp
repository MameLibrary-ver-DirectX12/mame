#include "GameScene.h"

#include "../Graphics/Graphics.h"
#include "../Graphics/Camera.h"
#include "../Input/Input.h"

#include "../Game/Character/PlayerManager.h"

#include "TitleScene.h"
#include "LoadingScene.h"
#include "SceneManager.h"

#include "../Game/Character/FlowerManager.h"
#include "../Game/Character/FlowerBleu.h"
#include "../Game/Character/Bee.h"
#include "../Game/Character/BeeManager.h"

#include "../Game/PoisonHoney/PoisonHoneyManager.h"
#include "../Game/PoisonHoney/PoisonHoneyNormal.h"

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

    ui_ = std::make_unique<GameUI>();

    //frameBuffer_ = std::make_unique<FrameBuffer>();
}

// ----- 初期化 -----
void GameScene::Initialize()
{
    // --- カメラ 初期化 ---
    Camera::Instance().InitializeGame();

    // --- プレイヤー 初期化 ---
    PlayerManager::Instnace().Initialize();
    PlayerManager::Instnace().GetPlayer()->GetTransform()->SetScaleFactor(0.5f);

    // --- お花 初期化 ---
    FlowerManager::Instance().Initialize();

    // --- ポイズンハニー 初期化 ---
    PoisonHoneyManager::Instance().Initialize();

    ui_->Initialize();

    stage_->GetTransform()->SetScaleFactor(10.0f);
    safeZone_->GetTransform()->SetScaleFactor(10.0f);
}

// ----- 終了化 -----
void GameScene::Finalize()
{
    // --- ポイズンハニー 終了化 ---
    PoisonHoneyManager::Instance().Finalize();

    // --- お花 終了化 ---
    FlowerManager::Instance().Finalize();

    // --- 蜂 終了化 ---
    BeeManager::Instance().Finalize();

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

    // --- ポイズンハニー 更新 ---
    PoisonHoneyManager::Instance().Update(elapsedTime);

    // --- お花 更新 ---
    FlowerManager::Instance().Update(elapsedTime);

    // --- 蜂 更新 ---
    BeeManager::Instance().Update(elapsedTime);

    // --- UI更新 ---
    ui_->Update(elapsedTime);
}

// ----- 描画 -----
void GameScene::Render(ID3D12GraphicsCommandList* commandList)
{
    Camera::Instance().SetPerSpectiveFovGame();

    //frameBuffer_->Activate(commandList);
    //frameBuffer_->Deactivate(commandList);

    // --- Model ---
    {
        Graphics::Instance().GetShader()->SetLambertShader(commandList);

        // --- ステージ ---
        stage_->Render(commandList, stage_->GetTransform()->CalcWorldMatrix(1.0f));
        safeZone_->Render(commandList, safeZone_->GetTransform()->CalcWorldMatrix(1.0f));

        // --- お花 ---
        FlowerManager::Instance().Render(commandList);

        // --- 蜂 ---
        BeeManager::Instance().Render(commandList);

        // --- ポイズンハニー ---
        PoisonHoneyManager::Instance().Render(commandList);

        // --- プレイヤー ---
        PlayerManager::Instnace().Render(commandList);

        //chickenCutIn_->Render(commandList);
    }

    ui_->Render(commandList);
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

    // --- お花 ---
    FlowerManager::Instance().DrawDebug();

    // --- 蜂 ---
    BeeManager::Instance().DrawDebug();

    // --- ポイズンハニー ---
    PoisonHoneyManager::Instance().DrawDebug();

    ui_->DrawDebug();

    chickenCutIn_->DrawDebug();
    if (ImGui::Button("initialize")) chickenCutIn_->GetStateMachine()->ChangeState(0);

    if (ImGui::Button("flower"))
    {
        FlowerBleu* flowerBleu = new FlowerBleu;
        flowerBleu->Initialize();
    }

    if (ImGui::Button("bee"))
    {
        Bee* bee = new Bee;
        bee->Initialize();
    }

    if (ImGui::Button("poison"))
    {
        PoisonHoneyNormal* poison = new PoisonHoneyNormal;
        poison->Initialize();
    }


    ImGui::End();
}
