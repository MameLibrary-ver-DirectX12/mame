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

// ----- �R���X�g���N�^ -----
GameScene::GameScene()
{
}

// ----- ���\�[�X���� -----
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

    // --- �G ������ ---
    EnemyManager::Instance().Initialize();

    honey_->Initialize();

    ui_->Initialize();

    stage_->SetColor({ 0.8f,1.0f,1.0f,1.0f });
    stage_->GetTransform()->SetScaleFactor(10.0f);
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

    // --- �G �I���� ---
    EnemyManager::Instance().Finalize();

    // --- �v���C���[�I���� ---
    PlayerManager::Instnace().Finalize();
}

// ----- �X�V -----
void GameScene::Update(const float& elapsedTime)
{
    GamePad& gamePad = Input::Instance().GetGamePad();
    
    if (gamePad.GetButtonDown() & GamePad::BTN_LEFT_TRIGGER)
    {
        SceneManager::Instance().ChangeScene(new LoadingScene(new TitleScene));
        return;
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

    // --- �G �X�V ---
    EnemyManager::Instance().Update(elapsedTime);

    // --- �͂��݂� �X�V ---
    honey_->Update(elapsedTime);

    // --- UI�X�V ---
    ui_->Update(elapsedTime);

}

// ----- �`�� -----
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

        // --- �X�e�[�W ---
        stage_->Render(commandList, stage_->GetTransform()->CalcWorldMatrix(1.0f));
    

        // --- ���� ---
        FlowerManager::Instance().Render(commandList);

        // --- �I ---
        BeeManager::Instance().Render(commandList);

        // --- �|�C�Y���n�j�[ ---
        PoisonHoneyManager::Instance().Render(commandList);

        // --- �G ---
        EnemyManager::Instance().Render(commandList);

        // --- �v���C���[ ---
        PlayerManager::Instnace().Render(commandList);

        // --- �͂��݂� ---
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

// ----- ImGui�p -----
void GameScene::DrawDebug()
{
    ImGui::Begin("GameScene");

    // --- �X�e�[�W ---
    stage_->DrawDebug();
    

    // --- �v���C���[ ---
    PlayerManager::Instnace().DrawDebug();

    // --- �J���� ---
    Camera::Instance().DrawDebug();

    // --- ���� ---
    FlowerManager::Instance().DrawDebug();

    // --- �I ---
    BeeManager::Instance().DrawDebug();

    // --- �G ---
    EnemyManager::Instance().DrawDebug();

    // --- �|�C�Y���n�j�[ ---
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
