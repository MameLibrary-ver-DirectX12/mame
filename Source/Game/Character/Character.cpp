#include "Character.h"

// --- �R���X�g���N�^ ---
Character::Character(const char* fbxFilename, bool triangulate, float samplingRate)
{
    // --- ���f������ ---
    model_ = std::make_unique<Model>(fbxFilename, triangulate, samplingRate);
}

// --- �`�� ---
void Character::Render(ID3D12GraphicsCommandList* commandList, const DirectX::XMMATRIX& world)
{
    model_->Render(commandList, world);
}

// --- ImGui�p ---
void Character::DrawDebug()
{
    model_->DrawDebug();
}