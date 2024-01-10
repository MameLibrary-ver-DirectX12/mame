#include "Honey.h"

// --- �R���X�g���N�^ ---
Honey::Honey()
    : Character("./Resources/Model/Honey.fbx")
{
}

// --- �f�X�g���N�^ ---
Honey::~Honey()
{
}

// --- ������ ---
void Honey::Initialize()
{
    SetColor(0.8f, 0.8f, 0.0f, 1.0f);

    GetTransform()->SetPosition(-15, 8, 40);
    GetTransform()->SetRotation(DirectX::XMConvertToRadians(45), 0, DirectX::XMConvertToRadians(45));
    GetTransform()->SetScaleFactor(4.0f);
}

// --- �I���� ---
void Honey::Finalize()
{
}

// --- �X�V ---
void Honey::Update(const float& elapsedTime)
{
    GetTransform()->AddRotationY(DirectX::XMConvertToRadians(45) * elapsedTime);
}

// --- �`�� ---
void Honey::Render(ID3D12GraphicsCommandList* commandList, const DirectX::XMMATRIX& world)
{
    Character::Render(commandList, GetTransform()->CalcWorldMatrix(1.0f));
}

// --- ImGui�p ---
void Honey::DrawDebug()
{
    if (ImGui::TreeNode("Honey"))
    {
        GetTransform()->DrawDebug();

        ImGui::TreePop();
    }
}
