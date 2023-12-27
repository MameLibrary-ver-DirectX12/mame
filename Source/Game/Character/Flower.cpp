#include "Flower.h"

// --- �R���X�g���N�^ ---
Flower::Flower(const char* fbxFilename)
    : Character(fbxFilename)
{
}

// --- �`�� ---
void Flower::Render(ID3D12GraphicsCommandList* commandList, const DirectX::XMMATRIX world)
{
    Character::Render(commandList, GetTransform()->CalcWorldMatrix(0.01f));
}
