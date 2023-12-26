#include "Enemy.h"

// --- コンストラクタ ---
Enemy::Enemy(const char* fbxFilename, bool triangulate, float samplingRate)
    : Character(fbxFilename, triangulate, samplingRate)
{
}

// --- 描画 ---
void Enemy::Render(ID3D12GraphicsCommandList* commandList, const DirectX::XMMATRIX world)
{
    Character::Render(commandList, GetTransform()->CalcWorldMatrix(0.01f));
}
