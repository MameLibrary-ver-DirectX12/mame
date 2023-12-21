#include "StageNormal.h"

int StageNormal::imguiNum_ = 0;

// --- コンストラクタ ---
StageNormal::StageNormal(const char* fbxFilename)
    : Stage(fbxFilename)
{
    Stage::SetName("StageNormal" + std::to_string(imguiNum_++));
}

// --- 初期化 ---
void StageNormal::Initialize()
{
}

// --- 終了化 ---
void StageNormal::Finalize()
{
}

// --- 更新 ---
void StageNormal::Update(const float& elapsedTime)
{
}

// --- 描画 ---
void StageNormal::Render(ID3D12GraphicsCommandList* commandList, DirectX::XMMATRIX world)
{
    Stage::Render(commandList, world);
}

// --- ImGui用 ---
void StageNormal::DrawDebug()
{
    if (ImGui::TreeNode(Stage::GetName().c_str()))
    {
        Stage::DrawDebug();
        ImGui::TreePop();
    }
}
