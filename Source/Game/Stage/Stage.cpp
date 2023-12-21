#include "Stage.h"

// --- �R���X�g���N�^ ---
Stage::Stage(const char* fbxFilename, bool triangulate, float samplingRate)
{
    model_ = std::make_unique<Model>(fbxFilename, triangulate, samplingRate);
}

// --- �`�� ---
void Stage::Render(ID3D12GraphicsCommandList* commandList, DirectX::XMMATRIX world)
{
    model_->Render(commandList, GetTransform()->CalcWorldMatrix(0.1f));
}

// --- ImGui�p ---
void Stage::DrawDebug()
{
    model_->DrawDebug();
}
