#include "PoisonHoney.h"

int PoisonHoney::nameNum_ = 0;

// --- コンストラクタ ---
PoisonHoney::PoisonHoney(const char* fbxFilename, bool triangulate, float samplingRate)
{
    model_ = std::make_unique<Model>(fbxFilename, triangulate, samplingRate);
}

// --- デストラクタ ---
PoisonHoney::~PoisonHoney()
{
    --nameNum_;
}

// --- 描画 ---
void PoisonHoney::Render(ID3D12GraphicsCommandList* commandList, const DirectX::XMMATRIX& world)
{
    model_->Render(commandList, world);
}

// --- ImGui用 ---
void PoisonHoney::DrawDebug()
{
    model_->DrawDebug();
}
