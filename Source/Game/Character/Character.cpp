#include "Character.h"

// --- コンストラクタ ---
Character::Character(const char* fbxFilename, bool triangulate, float samplingRate)
{
    // --- モデル生成 ---
    model_ = std::make_unique<Model>(fbxFilename, triangulate, samplingRate);

    removeCount_ = 0;
    isRemove_ = false;
    isRender_ = true;
}

// --- 描画 ---
void Character::Render(ID3D12GraphicsCommandList* commandList, const DirectX::XMMATRIX& world)
{
    if(isRender_) model_->Render(commandList, world);
}

// --- ImGui用 ---
void Character::DrawDebug()
{
    model_->DrawDebug();
}