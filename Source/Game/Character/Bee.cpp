#include "Bee.h"
#include "BeeState.h"
#include "BeeManager.h"

// --- コンストラクタ ---
Bee::Bee()
    : Character("./Resources/Model/Character/bee.fbx")
{
    // マネージャーに登録
    BeeManager::Instance().Register(this);

    // --- ステートマシン ---
    {
        stateMachine_.reset(new StateMachine<State<Bee>>);

        GetStateMachine()->RegisterState(new BeeState::SearchState(this));
        GetStateMachine()->RegisterState(new BeeState::MoveToFlowerState(this));
        GetStateMachine()->RegisterState(new BeeState::CollectState(this));
        GetStateMachine()->RegisterState(new BeeState::MoveToStoragePlaceState(this));

        GetStateMachine()->SetState(static_cast<UINT>(STATE::Search));
    }
}

// --- 初期化 ---
void Bee::Initialize()
{
    // 移動速度
    SetMoveSpeed(3.0f);

    // 色を設定
    Character::SetColor(1, 1, 0, 1);

    GetTransform()->SetPosition(0, 2, 0);
    GetTransform()->SetScaleFactor(1.5f);

    // 体の大きさ
    SetRadius(4.5f);
}

// --- 終了化 ---
void Bee::Finalize()
{
}

// --- 更新 ---
void Bee::Update(const float& elapsedTime)
{
    GetStateMachine()->Update(elapsedTime);

    //time += elapsedTime;
    //if (time > 3.0f) BeeManager::Instance().Remove(this);

}

// --- 描画 ---
void Bee::Render(ID3D12GraphicsCommandList* commandList, const DirectX::XMMATRIX& world)
{
    //if(time < 2.0f)
        Character::Render(commandList, GetTransform()->CalcWorldMatrix(1.0f));
}

// --- ImGui用 ---
void Bee::DrawDebug()
{
    GetTransform()->DrawDebug();
}
