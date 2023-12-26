#include "ChickenCutIn.h"

#include "ChickenState.h"

#include "../../Graphics/Graphics.h"

// --- �R���X�g���N�^ ---
ChickenCutIn::ChickenCutIn()
{
    model_ = std::make_unique<Model>("./Resources/Model/Character/golem.fbx");
    //model_ = std::make_unique<Model>("./Resources/Model/Character/sword.fbx");

    // --- Sprite ---
    blackSprite_[0] = std::make_unique<Sprite>("./Resources/Image/CutIn/black.png");
    blackSprite_[1] = std::make_unique<Sprite>("./Resources/Image/CutIn/black.png");
    whiteSprite_ = std::make_unique<Sprite>("./Resources/Image/white.png");

    // --- �X�e�[�g�}�V�� ---
    {
        stateMachine_.reset(new StateMachine<State<ChickenCutIn>>);

        GetStateMachine()->RegisterState(new ChickenState::FadeInState(this));
    }
}

// --- �f�X�g���N�^ ---
ChickenCutIn::~ChickenCutIn()
{
}

// --- �Đ� ---
void ChickenCutIn::Play(const float& elapsedTime)
{
    // --- �Đ����鏉�߂̂P�񂾂��������� ---
    if (!isPlay_) Initialize();

    GetStateMachine()->Update(elapsedTime);
    

    // --- �t���O�𗧂Ă� ---
    isPlay_ = true;
}

// --- �`�� ---
void ChickenCutIn::Render(ID3D12GraphicsCommandList* commandList)
{


    // --- Sprite ---
    {
        blackSprite_[0]->Render(commandList);
        blackSprite_[1]->Render(commandList);
        whiteSprite_->Render(commandList);
    }
    // --- Model ---
    {
        Graphics::Instance().GetShader()->SetLambertShader(commandList);

        model_->Render(commandList, model_->GetTransform()->CalcWorldMatrix(0.01f));
    }
}

void ChickenCutIn::DrawDebug()
{
    if (ImGui::TreeNode("ChickenCutIn"))
    {
        ImGui::DragFloat3("offset_", &offset_.x);

        
        blackSprite_[0]->DrawDebug();
        blackSprite_[1]->DrawDebug();
        whiteSprite_->DrawDebug();

        ImGui::TreePop();
    }
}

// --- ������ ---
void ChickenCutIn::Initialize()
{
    GetStateMachine()->SetState(0);

    blackSprite_[0]->GetTransform()->SetPos(2000, 0);
    blackSprite_[1]->GetTransform()->SetPos(-2000, 570);
}
