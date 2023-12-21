#include "StageNormal.h"

int StageNormal::imguiNum_ = 0;

// --- �R���X�g���N�^ ---
StageNormal::StageNormal(const char* fbxFilename)
    : Stage(fbxFilename)
{
    Stage::SetName("StageNormal" + std::to_string(imguiNum_++));
}

// --- ������ ---
void StageNormal::Initialize()
{
}

// --- �I���� ---
void StageNormal::Finalize()
{
}

// --- �X�V ---
void StageNormal::Update(const float& elapsedTime)
{
}

// --- �`�� ---
void StageNormal::Render(ID3D12GraphicsCommandList* commandList, DirectX::XMMATRIX world)
{
    Stage::Render(commandList, world);
}

// --- ImGui�p ---
void StageNormal::DrawDebug()
{
    if (ImGui::TreeNode(Stage::GetName().c_str()))
    {
        Stage::DrawDebug();
        ImGui::TreePop();
    }
}
