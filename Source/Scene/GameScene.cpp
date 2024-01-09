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

// ----- �R���X�g���N�^ -----
GameScene::GameScene()
{
}

// ----- ���\�[�X���� -----
void GameScene::CreateResource()
{
    stage_ = std::make_unique<StageNormal>("./Resources/Model/Stage/StageBase.fbx");
    safeZone_ = std::make_unique<StageNormal>("./Resources/Model/Stage/SafeZone.fbx");

    chickenCutIn_ = std::make_unique<ChickenCutIn>();

    ui_ = std::make_unique<GameUI>();

    //frameBuffer_ = std::make_unique<FrameBuffer>();
}

// ----- ������ -----
void GameScene::Initialize()
{
    // --- �J���� ������ ---
    Camera::Instance().InitializeGame();

    // --- �v���C���[ ������ ---
    PlayerManager::Instnace().Initialize();
    PlayerManager::Instnace().GetPlayer()->GetTransform()->SetScaleFactor(0.5f);

    // --- ���� ������ ---
    FlowerManager::Instance().Initialize();

    // --- �|�C�Y���n�j�[ ������ ---
    PoisonHoneyManager::Instance().Initialize();

    ui_->Initialize();

    stage_->GetTransform()->SetScaleFactor(10.0f);
    safeZone_->GetTransform()->SetScaleFactor(10.0f);
}

// ----- �I���� -----
void GameScene::Finalize()
{
    // --- �|�C�Y���n�j�[ �I���� ---
    PoisonHoneyManager::Instance().Finalize();

    // --- ���� �I���� ---
    FlowerManager::Instance().Finalize();

    // --- �I �I���� ---
    BeeManager::Instance().Finalize();

    // --- �v���C���[�I���� ---
    PlayerManager::Instnace().Finalize();
}

// ----- �X�V -----
void GameScene::Update(const float& elapsedTime)
{
    GamePad& gamePad = Input::Instance().GetGamePad();
    
    if (gamePad.GetButtonDown() & GamePad::BTN_A)
    {
        //SceneManager::Instance().ChangeScene(new LoadingScene(new TitleScene));
        //return;
    }

    chickenCutIn_->Play(elapsedTime);

    // --- �J�����X�V ---
    Camera::Instance().UpdateGame(elapsedTime);

    // --- �v���C���[�X�V ---
    PlayerManager::Instnace().Update(elapsedTime);

    // --- �|�C�Y���n�j�[ �X�V ---
    PoisonHoneyManager::Instance().Update(elapsedTime);

    // --- ���� �X�V ---
    FlowerManager::Instance().Update(elapsedTime);

    // --- �I �X�V ---
    BeeManager::Instance().Update(elapsedTime);

    // --- UI�X�V ---
    ui_->Update(elapsedTime);
}

// ----- �`�� -----
void GameScene::Render(ID3D12GraphicsCommandList* commandList)
{
    Camera::Instance().SetPerSpectiveFovGame();

    //frameBuffer_->Activate(commandList);
    //frameBuffer_->Deactivate(commandList);

    // --- Model ---
    {
        Graphics::Instance().GetShader()->SetLambertShader(commandList);

        // --- �X�e�[�W ---
        stage_->Render(commandList, stage_->GetTransform()->CalcWorldMatrix(1.0f));
        safeZone_->Render(commandList, safeZone_->GetTransform()->CalcWorldMatrix(1.0f));

        // --- ���� ---
        FlowerManager::Instance().Render(commandList);

        // --- �I ---
        BeeManager::Instance().Render(commandList);

        // --- �|�C�Y���n�j�[ ---
        PoisonHoneyManager::Instance().Render(commandList);

        // --- �v���C���[ ---
        PlayerManager::Instnace().Render(commandList);

        //chickenCutIn_->Render(commandList);
    }

    ui_->Render(commandList);
}

// ----- ImGui�p -----
void GameScene::DrawDebug()
{
    ImGui::Begin("GameScene");

    // --- �X�e�[�W ---
    stage_->DrawDebug();
    safeZone_->DrawDebug();

    // --- �v���C���[ ---
    PlayerManager::Instnace().DrawDebug();

    // --- �J���� ---
    Camera::Instance().DrawDebug();

    // --- ���� ---
    FlowerManager::Instance().DrawDebug();

    // --- �I ---
    BeeManager::Instance().DrawDebug();

    // --- �|�C�Y���n�j�[ ---
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
