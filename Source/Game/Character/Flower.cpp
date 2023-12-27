#include "Flower.h"

// --- コンストラクタ ---
Flower::Flower(const char* fbxFilename)
    : Character(fbxFilename)
{
}

// --- 描画 ---
void Flower::Render(ID3D12GraphicsCommandList* commandList, const DirectX::XMMATRIX world)
{
    Character::Render(commandList, GetTransform()->CalcWorldMatrix(0.01f));
}
