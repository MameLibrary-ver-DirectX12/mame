#include "FlowerBleu.h"
#include "FlowerManager.h"
#include "PlayerManager.h"

// --- �R���X�g���N�^ ---
FlowerBleu::FlowerBleu()
    : Flower("./Resources/Model/Character/Flower/flower.fbx")
{
    // �}�l�[�W���[�ɓo�^
    FlowerManager::Instance().Register(this);

    // ��ނ�ݒ�
    Flower::SetFlowerType(FlowerType::Blue);
}

// --- ������ ---
void FlowerBleu::Initialize()
{
    // �v���C���[�̑O�����ɐݒu����
    {
        DirectX::XMFLOAT3 playerPos = PlayerManager::Instnace().GetPlayer()->GetTransform()->GetPosition();
        DirectX::XMFLOAT3 playerFront = PlayerManager::Instnace().GetPlayer()->GetTransform()->CalcForward();
        float length = 3.0f;
        DirectX::XMStoreFloat3(&playerPos, DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&playerPos),
            DirectX::XMVectorScale(DirectX::XMVector3Normalize(DirectX::XMLoadFloat3(&playerFront)), length)));
        playerPos.y = 0.0f;

        GetTransform()->SetPosition(playerPos);
    }
}

// --- �I���� ---
void FlowerBleu::Finalize()
{
}

// --- �X�V ---
void FlowerBleu::Update(const float& elapsedTime)
{
}

// --- �`�� ---
void FlowerBleu::Render(ID3D12GraphicsCommandList* commandList, const DirectX::XMMATRIX world)
{
    GetTransform()->SetScaleFactor(100);
    Flower::Render(commandList, GetTransform()->CalcWorldMatrix(1.0f));
}

// --- ImGui�p ---
void FlowerBleu::DrawDebug()
{
    GetTransform()->DrawDebug();
}
