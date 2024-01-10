#include "Player.h"
#include "PlayerState.h"
#include "../PoisonHoney/PoisonHoneyManager.h"

#include "FlowerBleu.h"
#include "Bee.h"

// --- コンストラクタ ---
Player::Player()
    //: Character("./Resources/Model/Character/golem.fbx", true)
    //: Character("./Resources/Model/Character/nico.fbx", true)
    //: Character("./Resources/Model/Character/sword.fbx", true)
    //: Character("./Resources/Model/Character/Player.fbx", true)
    : Character("./Resources/Model/cube.fbx", true)
{
    SetName("Player");

    // --- ステートマシン ---
    {
        stateMachine_.reset(new StateMachine<State<Player>>);

        GetStateMachine()->RegisterState(new PlayerState::IdleState(this));
        GetStateMachine()->RegisterState(new PlayerState::MoveState(this));

        GetStateMachine()->SetState(static_cast<UINT>(STATE::Move));
    }
    
}

// --- デストラクタ ---
Player::~Player()
{
}

// --- 初期化 ---
void Player::Initialize()
{
    animationIndex_ = 1;
    //Character::PlayAnimation(animationIndex_, true);

    GetTransform()->SetPositionY(1.0f);

    // --- 半径設定 ---
    SetRadius(4.0f);

    // 初期は右向きで設定する
    GetTransform()->SetRotationY(DirectX::XMConvertToRadians(180));
}

// --- 終了化 ---
void Player::Finalize()
{
}

// --- 更新 ---
void Player::Update(const float& elapsedTime)
{
    Character::UpdateAnimation(elapsedTime);

    GetStateMachine()->Update(elapsedTime);

    // --- 位置補正 ---
    DirectX::XMFLOAT3 pos = GetTransform()->GetPosition();
    if (pos.x >=  43.0f) pos.x =  43.0f;
    if (pos.x <= -43.0f) pos.x = -43.0f;
    if (pos.z >=  14.0f) pos.z =  14.0f;
    if (pos.z <= -84.0f) pos.z = -84.0f;
    GetTransform()->SetPosition(pos);

     // 攻撃
    GamePad& gamePad = Input::Instance().GetGamePad();
    if (gamePad.GetButtonDown() & GamePad::BTN_RIGHT_SHOULDER ||
        gamePad.GetButtonDown() & GamePad::BTN_RIGHT_TRIGGER)
    {
        PoisonHoneyManager::Instance().Shot(PoisonHoneyManager::TYPE::Normal);
    }

    // お花設置
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

// --- 描画 ---
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
