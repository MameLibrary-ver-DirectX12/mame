#include "Honey.h"

// --- コンストラクタ ---
Honey::Honey()
    : Character("./Resources/Model/Honey.fbx")
{
}

// --- デストラクタ ---
Honey::~Honey()
{
}

// --- 初期化 ---
void Honey::Initialize()
{
    SetColor(0.8f, 0.8f, 0.0f, 1.0f);

    GetTransform()->SetPosition(-15, 8, 40);
    GetTransform()->SetRotation(DirectX::XMConvertToRadians(45), 0, DirectX::XMConvertToRadians(45));
    GetTransform()->SetScaleFactor(4.0f);
}

// --- 終了化 ---
void Honey::Finalize()
{
}

// --- 更新 ---
void Honey::Update(const float& elapsedTime)
{
    GetTransform()->AddRotationY(DirectX::XMConvertToRadians(45) * elapsedTime);
}

// --- 描画 ---
void Honey::Render(ID3D12GraphicsCommandList* commandList, const DirectX::XMMATRIX& world)
{
    Character::Render(commandList, GetTransform()->CalcWorldMatrix(1.0f));
}

// --- ImGui用 ---
void Honey::DrawDebug()
{
    if (ImGui::TreeNode("Honey"))
    {
        GetTransform()->DrawDebug();

        ImGui::TreePop();
    }
}
