#include "Bee.h"
#include "BeeState.h"


// --- �R���X�g���N�^ ---
Bee::Bee()
    : Character("./Resources/Model/Character/bee.fbx")
{
    // --- �X�e�[�g�}�V�� ---
    {
        stateMachine_.reset(new StateMachine<State<Bee>>);

        GetStateMachine()->RegisterState(new BeeState::SearchState(this));
        GetStateMachine()->RegisterState(new BeeState::MoveToFlowerState(this));

        GetStateMachine()->SetState(static_cast<UINT>(STATE::Search));
    }
}

// --- ������ ---
void Bee::Initialize()
{
}

// --- �I���� ---
void Bee::Finalize()
{
}

// --- �X�V ---
void Bee::Update(const float& elapsedTime)
{
}

// --- �`�� ---
void Bee::Render(ID3D12GraphicsCommandList* commandList, const DirectX::XMMATRIX world)
{
}

// --- ImGui�p ---
void Bee::DrawDebug()
{
}
