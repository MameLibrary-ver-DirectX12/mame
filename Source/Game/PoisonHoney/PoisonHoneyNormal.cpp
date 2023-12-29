#include "PoisonHoneyNormal.h"
#include "PoisonHoneyManager.h"
#include "../Character/PlayerManager.h"

// --- �R���X�g���N�^ ---
PoisonHoneyNormal::PoisonHoneyNormal()
    : PoisonHoney("./Resources/Model/PoisonHoneyNormal.fbx")
{
    // ImGuI���O�ݒ�
    PoisonHoney::SetName("Normal");

    // �����̎�ނ�ݒ肷��
    PoisonHoney::SetType(static_cast<int>(PoisonHoneyManager::TYPE::Normal));
}

// --- �f�X�g���N�^ ---
PoisonHoneyNormal::~PoisonHoneyNormal()
{
    // ImGui�㏈���͊��N���X�ōs���Ă���
}

// --- ������ ---
void PoisonHoneyNormal::Initialize()
{
    // �i�s�����擾
    direction_ = PlayerManager::Instnace().GetPlayer()->GetTransform()->CalcForward();

    // �X�s�[�h�ݒ�
    speed_ = 10.0f;
}

// --- �I���� ---
void PoisonHoneyNormal::Finalize()
{
}

// --- �X�V ---
void PoisonHoneyNormal::Update(const float& elapsedTime)
{
    DirectX::XMFLOAT3 moveVec;
    DirectX::XMStoreFloat3(&moveVec, DirectX::XMVectorScale(DirectX::XMLoadFloat3(&direction_), speed_ * elapsedTime));
    
    GetTransform()->AddPosition(moveVec);
}

// --- �`�� ---
void PoisonHoneyNormal::Render(ID3D12GraphicsCommandList* commandList, const DirectX::XMMATRIX& world)
{
    PoisonHoney::Render(commandList, GetTransform()->CalcWorldMatrix(1.0f));
}

// --- ImGui�p ---
void PoisonHoneyNormal::DrawDebug()
{
    if (ImGui::TreeNode(PoisonHoney::GetName()))
    {
        ImGui::DragFloat("speed", &speed_);

        ImGui::TreePop();
    }
}
