#include "Shop.h"

#include "../../Graphics/Graphics.h"
#include "../../Other/Common.h"

// --- コンストラクタ ---
Shop::Shop()
{
}

// --- リソース生成 ---
void Shop::CreateResource()
{
    // --- リソース生成されていたら生成しない ---
    if (isCreateResources_) return;

    // --- Sprite ---
    {
        whiteSprite_ = new Sprite("./Resources/Image/white.png");
        shopBaseSprite_ = new Sprite("./Resources/Image/Shop/ShopBase.png");
    }

    // --- Model ---
    {
        podium_ = new Model("./Resources/Model/pointBlack.fbx");
        turret_ = new Model("./Resources/Model/cube.fbx");
    }

    // --- リソース生成した ---
    isCreateResources_ = true;
}

// --- 初期化 ---
void Shop::Initialize()
{
    // --- podium 台 ---
    podium_->GetTransform()->SetPosition(30, 0, -40);
    podium_->GetTransform()->SetScaleFactor(20);

    // --- turret タレット ---
    turret_->GetTransform()->SetPosition(30, 25, -40);
    turret_->GetTransform()->SetScaleFactor(7);

    // --- Sprite ---
#pragma region Sprite Initialize
    {
        shopBaseSprite_->GetTransform()->SetPos(650, 50);
        shopBaseSprite_->GetTransform()->SetSize(540, 560);
    }
#pragma endregion Sprite Initialize

}

// --- 終了化 ---
void Shop::Finalize()
{
    
    // --- Sprite ---
    {
        SafeDeletePtr(whiteSprite_);
        SafeDeletePtr(shopBaseSprite_);
    }

    // --- Model ---
    {
        SafeDeletePtr(podium_);
        SafeDeletePtr(turret_);
    }

    // --- リソースを破棄した ---
    isCreateResources_ = false;
}

// --- 更新 ---
void Shop::Update(const float& elapsedTime)
{
    turret_->GetTransform()->AddRotationY(DirectX::XMConvertToRadians(45) * elapsedTime);

}

// --- 描画 ---
void Shop::Render(ID3D12GraphicsCommandList* commandList)
{
    // --- Sprite ---
    {
        whiteSprite_->GetTransform()->SetSize(0);
        whiteSprite_->Render(commandList);
        shopBaseSprite_->Render(commandList);
    }

    // --- Model ---
    {
        Graphics::Instance().GetShader()->SetLambertShader(commandList);

        podium_->Render(commandList, podium_->GetTransform()->CalcWorldMatrix(1.0f));
        turret_->Render(commandList, turret_->GetTransform()->CalcWorldMatrix(1.0f));

    }


}

// --- ImGUi用 ---
void Shop::DrawDebug()
{
    ImGui::Begin("Shop");

    whiteSprite_->DrawDebug();
    shopBaseSprite_->DrawDebug();

    //podium_->DrawDebug();
    turret_->DrawDebug();

    ImGui::End();
}
