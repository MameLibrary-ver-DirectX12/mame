#include "ShopScene.h"

#include "../Graphics/Graphics.h"
#include "../Graphics/Camera.h"

#include "../Game/Shop/Shop.h"

// --- �R���X�g���N�^ ---
ShopScene::ShopScene()
{
}

// --- ���\�[�X���� ---
void ShopScene::CreateResource()
{
    Shop::Instance().CreateResource();
}

// --- ������ ---
void ShopScene::Initialize()
{
    // --- �V���b�v������ ----
    Shop::Instance().Initialize();

    // --- �J���������� ---
    Camera::Instance().Initialize();
}

// --- �I���� ---
void ShopScene::Finalize()
{
    // --- �V���b�v�I���� ---
    Shop::Instance().Finalize();

}

// --- �X�V ---
void ShopScene::Update(const float& elapsedTime)
{
    // --- �V���b�v�X�V ---
    Shop::Instance().Update(elapsedTime);
}

// --- �`�� ---
void ShopScene::Render(ID3D12GraphicsCommandList* commandList)
{
    // --- �J���� ---
    Camera::Instance().SetPerSpectiveFov();

    // --- �V���b�v�`�� ---
    Shop::Instance().Render(commandList);
}

// --- ImGui�p ---
void ShopScene::DrawDebug()
{
    ImGui::Begin("ShopScene");

    Shop::Instance().DrawDebug();

    ImGui::End();
}
