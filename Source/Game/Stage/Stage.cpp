#include "Stage.h"

// --- コンストラクタ ---
Stage::Stage(const char* fbxFilename, bool triangulate, float samplingRate)
{
    model_ = std::make_unique<Model>(fbxFilename, triangulate, samplingRate);
}

// --- 描画 ---
void Stage::Render(ID3D12GraphicsCommandList* commandList, DirectX::XMMATRIX world)
{
    model_->Render(commandList, GetTransform()->CalcWorldMatrix(0.1f));
}

// --- ImGui用 ---
void Stage::DrawDebug()
{
    model_->DrawDebug();
}
