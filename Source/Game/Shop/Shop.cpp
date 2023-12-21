#include "Shop.h"

#include "../../Graphics/Graphics.h"
#include "../../Other/Common.h"

// --- �R���X�g���N�^ ---
Shop::Shop()
{
}

// --- ���\�[�X���� ---
void Shop::CreateResource()
{
    // --- ���\�[�X��������Ă����琶�����Ȃ� ---
    if (isCreateResources_) return;

    // --- Sprite ---
    {
        whiteSprite_ = new Sprite("./Resources/Image/white.png");
    }

    // --- Model ---
    {
        podium_ = new Model("./Resources/Model/pointBlack.fbx");
        turret_ = new Model("./Resources/Model/cube.fbx");
    }

    // --- ���\�[�X�������� ---
    isCreateResources_ = true;
}

// --- ������ ---
void Shop::Initialize()
{
    // --- podium �� ---
    podium_->GetTransform()->SetPosition(30, 0, -40);
    podium_->GetTransform()->SetScaleFactor(20);

    // --- turret �^���b�g ---
    turret_->GetTransform()->SetPosition(30, 25, -40);
    turret_->GetTransform()->SetScaleFactor(7);

}

// --- �I���� ---
void Shop::Finalize()
{
    
    // --- Sprite ---
    {
        SafeDeletePtr(whiteSprite_);
    }

    // --- Model ---
    {
        SafeDeletePtr(podium_);
        SafeDeletePtr(turret_);
    }

    // --- ���\�[�X��j������ ---
    isCreateResources_ = false;
}

// --- �X�V ---
void Shop::Update(const float& elapsedTime)
{
    turret_->GetTransform()->AddRotationY(DirectX::XMConvertToRadians(45) * elapsedTime);

}

// --- �`�� ---
void Shop::Render(ID3D12GraphicsCommandList* commandList)
{
    // --- Sprite ---
    {
        whiteSprite_->GetTransform()->SetSize(0);
        whiteSprite_->Render(commandList);
    }

    // --- Model ---
    {
        Graphics::Instance().GetShader()->SetLambertShader(commandList);

        podium_->Render(commandList, podium_->GetTransform()->CalcWorldMatrix(1.0f));
        turret_->Render(commandList, turret_->GetTransform()->CalcWorldMatrix(1.0f));

    }


}

// --- ImGUi�p ---
void Shop::DrawDebug()
{
    ImGui::Begin("Shop");

    whiteSprite_->DrawDebug();

    //podium_->DrawDebug();
    turret_->DrawDebug();

    ImGui::End();
}
