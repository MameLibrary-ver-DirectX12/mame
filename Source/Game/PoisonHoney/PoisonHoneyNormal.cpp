#include "PoisonHoneyNormal.h"
#include "PoisonHoneyManager.h"
#include "../Character/PlayerManager.h"
#include "PoisonHoneyState.h"

// --- �R���X�g���N�^ ---
PoisonHoneyNormal::PoisonHoneyNormal()
    : PoisonHoney("./Resources/Model/PoisonHoney/PoisonHoneyNormal.fbx")
{
    // ImGuI���O�ݒ�
    PoisonHoney::SetName("Normal");

    // �����̎�ނ�ݒ肷��
    PoisonHoney::SetType(static_cast<int>(PoisonHoneyManager::TYPE::Normal));

    // �X�e�[�g�}�V��
    PoisonHoney::CreateStateMachine();
    GetStateMachine()->RegisterState(new PoisonHoneyState::MoveState(this));
    GetStateMachine()->RegisterState(new PoisonHoneyState::IdleState(this));
    GetStateMachine()->RegisterState(new PoisonHoneyState::AttackState(this));
    GetStateMachine()->SetState(static_cast<int>(PoisonHoney::STATE::Move));

    // �}�l�[�W���[�ɓo�^
    PoisonHoneyManager::Instance().Register(this);
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
    PoisonHoney::SetDirection(PlayerManager::Instnace().GetPlayer()->GetTransform()->CalcForward());

    // �X�s�[�h�ݒ�
    PoisonHoney::SetSpeed(30.0f);

    // ����p���a�ݒ�
    PoisonHoney::SetRadius(2.0f);
}

// --- �I���� ---
void PoisonHoneyNormal::Finalize()
{
}

// --- �X�V ---
void PoisonHoneyNormal::Update(const float& elapsedTime)
{
    // --- �X�e�[�g�}�V���ł̏��� ---
    PoisonHoney::Update(elapsedTime);

    // ��ʊO�ɏo���ʂ���������
    Remove();
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
        GetTransform()->DrawDebug();

        ImGui::TreePop();
    }
}

// --- ��ʊO�ɏo���e������ ---
void PoisonHoneyNormal::Remove()
{

}
