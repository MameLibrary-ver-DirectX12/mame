#include "ChickenCutIn.h"

#include "../../Graphics/Graphics.h"

// --- �R���X�g���N�^ ---
ChickenCutIn::ChickenCutIn()
{
}

// --- �f�X�g���N�^ ---
ChickenCutIn::~ChickenCutIn()
{
}

// --- �Đ� ---
void ChickenCutIn::Play(const float& elapsedTime)
{
    // --- �Đ����鏉�߂̂P�񂾂��������� ---
    if (!isPlay_) Initialize();




    // --- �t���O�𗧂Ă� ---
    isPlay_ = true;
}

// --- �`�� ---
void ChickenCutIn::Render(ID3D12GraphicsCommandList* commandList)
{
    // --- Model ---
    {
        Graphics::Instance().GetShader()->SetLambertShader(commandList);
    }
}

// --- ������ ---
void ChickenCutIn::Initialize()
{
}
