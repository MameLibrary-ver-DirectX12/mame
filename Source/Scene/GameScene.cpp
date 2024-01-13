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

#include "../Game/Character/EnemyManager.h"

#include "../Game/Character/BeeEnemy.h"

#define FRAME_BUFFER 1

// ----- コンストラクタ -----
GameScene::GameScene()
{
}

// ----- リソース生成 -----
void GameScene::CreateResource()
{
    stage_ = std::make_unique<StageNormal>("./Resources/Model/Stage/stage.fbx");

    chickenCutIn_ = std::make_unique<ChickenCutIn>();

    honey_ = std::make_unique<Honey>();

    ui_ = std::make_unique<GameUI>();

#if FRAME_BUFFER
    frameBuffer_ = std::make_unique<FrameBuffer>();
#endif
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

    // --- 敵 初期化 ---
    EnemyManager::Instance().Initialize();

    honey_->Initialize();

    ui_->Initialize();

    stage_->SetColor({ 0.8f,1.0f,1.0f,1.0f });
    stage_->GetTransform()->SetScaleFactor(10.0f);
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

    // --- 敵 終了化 ---
    EnemyManager::Instance().Finalize();

    // --- プレイヤー終了化 ---
    PlayerManager::Instnace().Finalize();
}

// ----- 更新 -----
void GameScene::Update(const float& elapsedTime)
{
    GamePad& gamePad = Input::Instance().GetGamePad();
    
    if (gamePad.GetButtonDown() & GamePad::BTN_LEFT_TRIGGER)
    {
        SceneManager::Instance().ChangeScene(new LoadingScene(new TitleScene));
        return;
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

    // --- 敵 更新 ---
    EnemyManager::Instance().Update(elapsedTime);

    // --- はちみつ 更新 ---
    honey_->Update(elapsedTime);

    // --- UI更新 ---
    ui_->Update(elapsedTime);

}

// ----- 描画 -----
void GameScene::Render(ID3D12GraphicsCommandList* commandList)
{
    Camera::Instance().SetPerSpectiveFovGame();

#if FRAME_BUFFER
    //frameBuffer_->Activate(commandList);
    //frameBuffer_->Deactivate(commandList);
#endif

    // --- Model ---
    {
        Graphics::Instance().GetShader()->SetLambertShader(commandList);

        // --- ステージ ---
        stage_->Render(commandList, stage_->GetTransform()->CalcWorldMatrix(1.0f));
    

        // --- お花 ---
        FlowerManager::Instance().Render(commandList);

        // --- 蜂 ---
        BeeManager::Instance().Render(commandList);

        // --- ポイズンハニー ---
        PoisonHoneyManager::Instance().Render(commandList);

        // --- 敵 ---
        EnemyManager::Instance().Render(commandList);

        // --- プレイヤー ---
        PlayerManager::Instnace().Render(commandList);

        // --- はちみつ ---
        honey_->Render(commandList);

        //chickenCutIn_->Render(commandList);
    }

    
}

void GameScene::UIRender(ID3D12GraphicsCommandList* commandList)
{
    ui_->Render(commandList);
}

void GameScene::ShadowRender(ID3D12GraphicsCommandList* commandList)
{
}

// ----- ImGui用 -----
void GameScene::DrawDebug()
{
    ImGui::Begin("GameScene");

    // --- ステージ ---
    stage_->DrawDebug();
    

    // --- プレイヤー ---
    PlayerManager::Instnace().DrawDebug();

    // --- カメラ ---
    Camera::Instance().DrawDebug();

    // --- お花 ---
    FlowerManager::Instance().DrawDebug();

    // --- 蜂 ---
    BeeManager::Instance().DrawDebug();

    // --- 敵 ---
    EnemyManager::Instance().DrawDebug();

    // --- ポイズンハニー ---
    PoisonHoneyManager::Instance().DrawDebug();

    honey_->DrawDebug();

    ui_->DrawDebug();

    chickenCutIn_->DrawDebug();
    if (ImGui::Button("initialize")) chickenCutIn_->GetStateMachine()->ChangeState(0);

    ImGui::DragFloat3("CreatePos", &createPos_.x);
    if (ImGui::Button("E_Bee"))
    {
        BeeEnemy* bee = new BeeEnemy;
        bee->Initialize();
        bee->GetTransform()->SetPosition(createPos_);
    }

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
