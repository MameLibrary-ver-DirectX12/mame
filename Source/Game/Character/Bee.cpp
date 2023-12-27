#include "Bee.h"

// --- コンストラクタ ---
Bee::Bee()
    : Character("./Resources/Model/Character/bee.fbx")
{
}

// --- 初期化 ---
void Bee::Initialize()
{
}

// --- 終了化 ---
void Bee::Finalize()
{
}

// --- 更新 ---
void Bee::Update(const float& elapsedTime)
{
}

// --- 描画 ---
void Bee::Render(ID3D12GraphicsCommandList* commandList, const DirectX::XMMATRIX world)
{
}

// --- ImGui用 ---
void Bee::DrawDebug()
{
}
