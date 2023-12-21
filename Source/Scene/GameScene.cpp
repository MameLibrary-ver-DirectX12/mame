#include "GameScene.h"

#include "../Graphics/Graphics.h"
#include "../Graphics/Camera.h"
#include "../Input/Input.h"

#include "../Game/Character/PlayerManager.h"

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
    stage_ = std::make_unique<StageNormal>("./Resources/Model/Stage/StageBase.fbx");
    safeZone_ = std::make_unique<StageNormal>("./Resources/Model/Stage/SafeZone.fbx");
}

// ----- ������ -----
void GameScene::Initialize()
{
    // --- �J���������� ---
    Camera::Instance().InitializeGame();

    // --- �v���C���[������ ---
    PlayerManager::Instnace().Initialize();
    PlayerManager::Instnace().GetPlayer()->GetTransform()->SetScaleFactor(0.5f);

    stage_->GetTransform()->SetScaleFactor(10.0f);
    safeZone_->GetTransform()->SetScaleFactor(10.0f);
}

// ----- �I���� -----
void GameScene::Finalize()
{
    // --- �v���C���[�I���� ---
    PlayerManager::Instnace().Finalize();
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

    // --- �J�����X�V ---
    //Camera::Instance().UpdateGame(elapsedTime);

    // --- �v���C���[�X�V ---
    PlayerManager::Instnace().Update(elapsedTime);
}

// ----- �`�� -----
void GameScene::Render(ID3D12GraphicsCommandList* commandList)
{
    Camera::Instance().SetPerSpectiveFovGame();

    // --- Model ---
    {
        Graphics::Instance().GetShader()->SetLambertShader(commandList);

        // --- �X�e�[�W ---
        stage_->Render(commandList, stage_->GetTransform()->CalcWorldMatrix(1.0f));
        safeZone_->Render(commandList, safeZone_->GetTransform()->CalcWorldMatrix(1.0f));

        // --- �v���C���[ ---
        PlayerManager::Instnace().Render(commandList);


    }
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

    ImGui::End();
}