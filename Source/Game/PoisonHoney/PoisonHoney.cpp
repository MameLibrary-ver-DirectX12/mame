#include "PoisonHoney.h"

int PoisonHoney::nameNum_ = 0;

// --- �R���X�g���N�^ ---
PoisonHoney::PoisonHoney(const char* fbxFilename, bool triangulate, float samplingRate)
{
    model_ = std::make_unique<Model>(fbxFilename, triangulate, samplingRate);
}

// --- �f�X�g���N�^ ---
PoisonHoney::~PoisonHoney()
{
    --nameNum_;
}

// --- �`�� ---
void PoisonHoney::Render(ID3D12GraphicsCommandList* commandList, const DirectX::XMMATRIX& world)
{
    model_->Render(commandList, world);
}

// --- ImGui�p ---
void PoisonHoney::DrawDebug()
{
    model_->DrawDebug();
}
