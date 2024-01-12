#include "Bee.h"
#include "BeeState.h"
#include "BeeManager.h"

// --- �R���X�g���N�^ ---
Bee::Bee()
    : Character("./Resources/Model/Character/bee.fbx")
{
    // �}�l�[�W���[�ɓo�^
    BeeManager::Instance().Register(this);

    // --- �X�e�[�g�}�V�� ---
    {
        stateMachine_.reset(new StateMachine<State<Bee>>);

        GetStateMachine()->RegisterState(new BeeState::SearchState(this));
        GetStateMachine()->RegisterState(new BeeState::MoveToFlowerState(this));
        GetStateMachine()->RegisterState(new BeeState::CollectState(this));
        GetStateMachine()->RegisterState(new BeeState::MoveToStoragePlaceState(this));

        GetStateMachine()->SetState(static_cast<UINT>(STATE::Search));
    }
}

// --- ������ ---
void Bee::Initialize()
{
    // �ړ����x
    SetMoveSpeed(3.0f);

    // �F��ݒ�
    Character::SetColor(1, 1, 0, 1);

    GetTransform()->SetPosition(0, 2, 0);
    GetTransform()->SetScaleFactor(1.5f);

    // �̂̑傫��
    SetRadius(4.5f);
}

// --- �I���� ---
void Bee::Finalize()
{
}

// --- �X�V ---
void Bee::Update(const float& elapsedTime)
{
    GetStateMachine()->Update(elapsedTime);

    //time += elapsedTime;
    //if (time > 3.0f) BeeManager::Instance().Remove(this);

}

// --- �`�� ---
void Bee::Render(ID3D12GraphicsCommandList* commandList, const DirectX::XMMATRIX& world)
{
    //if(time < 2.0f)
        Character::Render(commandList, GetTransform()->CalcWorldMatrix(1.0f));
}

// --- ImGui�p ---
void Bee::DrawDebug()
{
    GetTransform()->DrawDebug();
}
