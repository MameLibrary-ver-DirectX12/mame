#include "Player.h"

// --- コンストラクタ ---
Player::Player()
    //: Character("./Resources/Model/Character/golem.fbx", true)
    //: Character("./Resources/Model/Character/nico.fbx", true)
    //: Character("./Resources/Model/Character/sword.fbx", true)
    : Character("./Resources/Model/Character/playerEx.fbx", true)
    //: Character("./Resources/Model/Character/Player.fbx", true)
{
    SetName("Player");

    arm_ = std::make_unique<Character>("./Resources/Model/Character/sword.fbx", true);
}

// --- デストラクタ ---
Player::~Player()
{
}

// --- 初期化 ---
void Player::Initialize()
{
    animationIndex_ = 1;
    Character::PlayAnimation(animationIndex_, true);
}

// --- 終了化 ---
void Player::Finalize()
{
}

// --- 更新 ---
void Player::Update(const float& elapsedTime)
{
    Character::UpdateAnimation(elapsedTime);
}

// --- 描画 ---
void Player::Render(ID3D12GraphicsCommandList* commandList, const DirectX::XMMATRIX world)
{
    Character::Render(commandList, GetTransform()->CalcWorldMatrix(0.1f));
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

        ImGui::Begin("ARM");
        arm_->DrawDebug();
        ImGui::End();

        ImGui::EndMenu();
    }
}
