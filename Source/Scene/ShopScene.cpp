#include "ShopScene.h"

#include "../Graphics/Graphics.h"
#include "../Graphics/Camera.h"

#include "../Game/Shop/Shop.h"

// --- コンストラクタ ---
ShopScene::ShopScene()
{
}

// --- リソース生成 ---
void ShopScene::CreateResource()
{
    Shop::Instance().CreateResource();
}

// --- 初期化 ---
void ShopScene::Initialize()
{
    // --- ショップ初期化 ----
    Shop::Instance().Initialize();

    // --- カメラ初期化 ---
    Camera::Instance().Initialize();
}

// --- 終了化 ---
void ShopScene::Finalize()
{
    // --- ショップ終了化 ---
    Shop::Instance().Finalize();

}

// --- 更新 ---
void ShopScene::Update(const float& elapsedTime)
{
    // --- ショップ更新 ---
    Shop::Instance().Update(elapsedTime);
}

// --- 描画 ---
void ShopScene::Render(ID3D12GraphicsCommandList* commandList)
{
    // --- カメラ ---
    Camera::Instance().SetPerSpectiveFov();

    // --- ショップ描画 ---
    Shop::Instance().Render(commandList);
}

// --- ImGui用 ---
void ShopScene::DrawDebug()
{
    ImGui::Begin("ShopScene");

    Shop::Instance().DrawDebug();

    ImGui::End();
}
