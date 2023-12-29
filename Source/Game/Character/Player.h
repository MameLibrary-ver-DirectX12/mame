#pragma once
#include "Character.h"
#include "../../AI/StateMachine.h"
#include "../../Input/Input.h"

class Player : public Character
{
public:// --- 定数 ---
#pragma region 定数
    enum class STATE
    {
        Idle,
        Move,
    };

#pragma endregion 定数

public:
    Player();
    ~Player();


    void Initialize()                                   override;   // 初期化
    void Finalize()                                     override;   // 終了化
    void Update(const float& elapsedTime)               override;   // 更新
    void Render(ID3D12GraphicsCommandList* commandList,
        const DirectX::XMMATRIX& world = {})             override;  // 描画
    void DrawDebug()                                    override;   // ImGui用

public:// --- 取得・設定 ---
    StateMachine<State<Player>>* GetStateMachine() { return stateMachine_.get(); }
    void ChangeState(STATE state) { GetStateMachine()->ChangeState(static_cast<UINT>(state)); }

    // --- 速度 ---
    DirectX::XMFLOAT3 GetVelocity() { return velocity_; }
    void SetVelocity(DirectX::XMFLOAT3 velocity) { velocity_ = velocity; }

    // --- スキル ---
    int GetFlowerNum() { return flowerNum_; }
    void SetFlowerNum(int num) { flowerNum_ = num; }
    bool UseFlower() 
    { 
        if (flowerNum_)
        {
            --flowerNum_;
            return true;
        }
        return false;
    }

private:
    // ---------- ステートマシン -----------------------------------
    std::unique_ptr<StateMachine<State<Player>>> stateMachine_;

    // ------------------------------------------------------------

    DirectX::XMFLOAT3 velocity_ = {};

    int animationIndex_ = 0;

    // ----- スキル -----
    int flowerNum_ = 0; // 所持している花の数

};

