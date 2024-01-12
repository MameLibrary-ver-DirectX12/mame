#include "PoisonHoney.h"
#include "PoisonHoneyManager.h"

int PoisonHoney::nameNum_ = 0;

// --- コンストラクタ ---
PoisonHoney::PoisonHoney(const char* fbxFilename, bool triangulate, float samplingRate)
{
    model_ = std::make_unique<Model>(fbxFilename, triangulate, samplingRate);

    // 基本的に描画する
    SetIsRender(true);

    // 攻撃フラグを false にする
    SetIsAbleAttack(false);
}

// --- デストラクタ ---
PoisonHoney::~PoisonHoney()
{
    --nameNum_;
}

// --- 更新 ---
void PoisonHoney::Update(const float& elapsedTime)
{
    GetStateMachine()->Update(elapsedTime);
}

// --- 描画 ---
void PoisonHoney::Render(ID3D12GraphicsCommandList* commandList, const DirectX::XMMATRIX& world)
{
    model_->Render(commandList, world);
}

// --- ImGui用 ---
void PoisonHoney::DrawDebug()
{
    model_->DrawDebug();
}

// --- ステートマシン生成 ---
void PoisonHoney::CreateStateMachine()
{
    // 生成していなかったら　生成する
    if (!isCreateStateMachine_)
    {
        stateMachine_.reset(new StateMachine<State<PoisonHoney>>);
        isCreateStateMachine_ = true;
    }
}
