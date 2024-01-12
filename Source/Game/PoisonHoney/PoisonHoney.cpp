#include "PoisonHoney.h"
#include "PoisonHoneyManager.h"

int PoisonHoney::nameNum_ = 0;

// --- �R���X�g���N�^ ---
PoisonHoney::PoisonHoney(const char* fbxFilename, bool triangulate, float samplingRate)
{
    model_ = std::make_unique<Model>(fbxFilename, triangulate, samplingRate);

    // ��{�I�ɕ`�悷��
    SetIsRender(true);

    // �U���t���O�� false �ɂ���
    SetIsAbleAttack(false);
}

// --- �f�X�g���N�^ ---
PoisonHoney::~PoisonHoney()
{
    --nameNum_;
}

// --- �X�V ---
void PoisonHoney::Update(const float& elapsedTime)
{
    GetStateMachine()->Update(elapsedTime);
}

// --- �`�� ---
void PoisonHoney::Render(ID3D12GraphicsCommandList* commandList, const DirectX::XMMATRIX& world)
{
    model_->Render(commandList, world);
}

// --- ImGui�p ---
void PoisonHoney::DrawDebug()
{
    model_->DrawDebug();
}

// --- �X�e�[�g�}�V������ ---
void PoisonHoney::CreateStateMachine()
{
    // �������Ă��Ȃ�������@��������
    if (!isCreateStateMachine_)
    {
        stateMachine_.reset(new StateMachine<State<PoisonHoney>>);
        isCreateStateMachine_ = true;
    }
}
