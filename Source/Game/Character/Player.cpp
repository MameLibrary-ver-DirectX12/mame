#include "Player.h"
#include "PlayerState.h"
#include "../PoisonHoney/PoisonHoneyManager.h"

#include "FlowerBleu.h"
#include "Bee.h"

// --- �R���X�g���N�^ ---
Player::Player()
    //: Character("./Resources/Model/Character/golem.fbx", true)
    //: Character("./Resources/Model/Character/nico.fbx", true)
    //: Character("./Resources/Model/Character/sword.fbx", true)
    //: Character("./Resources/Model/Character/Player.fbx", true)
    : Character("./Resources/Model/cube.fbx", true)
{
    SetName("Player");

    // --- �X�e�[�g�}�V�� ---
    {
        stateMachine_.reset(new StateMachine<State<Player>>);

        GetStateMachine()->RegisterState(new PlayerState::IdleState(this));
        GetStateMachine()->RegisterState(new PlayerState::MoveState(this));

        GetStateMachine()->SetState(static_cast<UINT>(STATE::Move));
    }
    
}

// --- �f�X�g���N�^ ---
Player::~Player()
{
}

// --- ������ ---
void Player::Initialize()
{
    animationIndex_ = 1;
    //Character::PlayAnimation(animationIndex_, true);

    GetTransform()->SetPositionY(1.0f);

    // --- ���a�ݒ� ---
    SetRadius(4.0f);

    // �����͉E�����Őݒ肷��
    GetTransform()->SetRotationY(DirectX::XMConvertToRadians(180));
}

// --- �I���� ---
void Player::Finalize()
{
}

// --- �X�V ---
void Player::Update(const float& elapsedTime)
{
    Character::UpdateAnimation(elapsedTime);

    GetStateMachine()->Update(elapsedTime);

    // --- �ʒu�␳ ---
    DirectX::XMFLOAT3 pos = GetTransform()->GetPosition();
    if (pos.x >=  43.0f) pos.x =  43.0f;
    if (pos.x <= -43.0f) pos.x = -43.0f;
    if (pos.z >=  14.0f) pos.z =  14.0f;
    if (pos.z <= -84.0f) pos.z = -84.0f;
    GetTransform()->SetPosition(pos);

     // �U��
    GamePad& gamePad = Input::Instance().GetGamePad();
    if (gamePad.GetButtonDown() & GamePad::BTN_RIGHT_SHOULDER ||
        gamePad.GetButtonDown() & GamePad::BTN_RIGHT_TRIGGER)
    {
        PoisonHoneyManager::Instance().Shot(PoisonHoneyManager::TYPE::Normal);
    }

    // ���Ԑݒu
    if (gamePad.GetButtonDown() & GamePad::BTN_X &&
        isAbleUseFlower_)
    {

        FlowerBleu* flowerBleu = new FlowerBleu;
        flowerBleu->Initialize();

        flowerTimer_ = 4.0f;
        flowerMaxTimer_ = 4.0f;
        isAbleUseFlower_ = false;
    }
    if (!isAbleUseFlower_)
    {
        flowerTimer_ -= elapsedTime;

        if (flowerTimer_ <= 0.0f)
        {
            flowerTimer_ = 0.0f;
            isAbleUseFlower_ = true;
        }
    }


    if (gamePad.GetButtonDown() & GamePad::BTN_Y)
    {
        Bee* bee = new Bee;
        bee->Initialize();
    }

}

// --- �`�� ---
void Player::Render(ID3D12GraphicsCommandList* commandList, const DirectX::XMMATRIX& world)
{
    GetTransform()->SetScaleFactor(1.0f);
    GetTransform()->AddRotationY(DirectX::XMConvertToRadians(90.0f));
    //Character::Render(commandList, GetTransform()->CalcWorldMatrix(0.1f));
    Character::Render(commandList, GetTransform()->CalcWorldMatrix(1.0f));
    GetTransform()->AddRotationY(DirectX::XMConvertToRadians(-90.0f));
}

// --- ImGui ---
void Player::DrawDebug()
{
    if (ImGui::BeginMenu(GetName()))
    {
        ImGui::SliderInt("animation", &animationIndex_, 0, 12);
        //ImGui::SliderInt("animation", &animationIndex_, 0, 23);
        if (ImGui::Button("Change")) { PlayAnimation(animationIndex_, true); }

        Character::DrawDebug();

        ImGui::EndMenu();
    }
}
