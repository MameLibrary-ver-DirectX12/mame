#include "ChickenCutIn.h"

#include "../../Graphics/Graphics.h"

// --- コンストラクタ ---
ChickenCutIn::ChickenCutIn()
{
}

// --- デストラクタ ---
ChickenCutIn::~ChickenCutIn()
{
}

// --- 再生 ---
void ChickenCutIn::Play(const float& elapsedTime)
{
    // --- 再生する初めの１回だけ処理する ---
    if (!isPlay_) Initialize();




    // --- フラグを立てる ---
    isPlay_ = true;
}

// --- 描画 ---
void ChickenCutIn::Render(ID3D12GraphicsCommandList* commandList)
{
    // --- Model ---
    {
        Graphics::Instance().GetShader()->SetLambertShader(commandList);
    }
}

// --- 初期化 ---
void ChickenCutIn::Initialize()
{
}
