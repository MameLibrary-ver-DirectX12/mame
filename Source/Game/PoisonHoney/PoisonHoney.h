#pragma once
#include "../../Resource/Model.h"
#include "../../AI/StateMachine.h"
#include <string>

class PoisonHoney
{
public:
    enum class STATE
    {
        Move,
        Idle,
        Attack,
    };

public:
    PoisonHoney(const char* fbxFilename, bool triangulate = false, float samplingRate = 0);
    virtual ~PoisonHoney();

    virtual void Initialize() {}                                    // 初期化
    virtual void Finalize() {}                                      // 終了化
    virtual void Update(const float& elapsedTime);                  // 更新
    virtual void Render(ID3D12GraphicsCommandList* commandList,
        const DirectX::XMMATRIX& world = {});                       // 描画
    virtual void DrawDebug();                                       // ImGui用

    void CreateStateMachine();

public:// --- 取得・設定 ---
#pragma region [Get, Set] Function
    // --- Transform ---
    Transform* GetTransform() { return model_->GetTransform(); }
    DirectX::XMFLOAT4 GetColor() { return model_->GetColor(); }
    void SetColor(const DirectX::XMFLOAT4& color) { model_->SetColor(color); }
    void SetColor(const float& r, const float& g, const float& b, const float& a) { model_->SetColor({ r, g, b, a }); }

    // --- ステートマシン ---
    StateMachine<State<PoisonHoney>>* GetStateMachine() { return stateMachine_.get(); }
    void SetState(STATE state) { GetStateMachine()->ChangeState(static_cast<int>(state)); }

    // --- 描画フラグ ---
    bool GetIsRender() { return isRender_; }
    void SetIsRender(const bool& isRender) { isRender_ = isRender; }

    // --- 攻撃できるかフラグ ---
    bool GetIsAbleAttack() { return isAbleAttack_; }
    void SetIsAbleAttack(const bool& isAbleAttack) { isAbleAttack_ = isAbleAttack; }

    // --- 種類 ---
    int GetType() { return type_; }
    void SetType(int type) { type_ = type; }

    // --- 速度 ---
    float GetSpeed() { return speed_; }
    void SetSpeed(const float& speed) { speed_ = speed; }

    // --- 進行方向 ---
    DirectX::XMFLOAT3 GetDirection() { return direction_; }
    void SetDirection(const DirectX::XMFLOAT3& direction) { direction_ = direction; }

    // --- 半径 ---
    float GetRadius() { return radius_; }
    void SetRadius(const float& radius) { radius_ = radius; }

    bool GetIsRemove() { return isRemove_; }
    void SetIsRemove(const bool& remove) { isRemove_ = remove; }
    int GetRemoveCount() { return removeCount_; }
    void SetRemoveCount(const int& count) { removeCount_ = count; }
    void AddRemoveCount() { ++removeCount_; }

    // --- ImGui用 ---
    const char* const GetName() const { return model_->GetName(); }
    void SetName(const std::string& name) { model_->SetName(name + std::to_string(nameNum_++)); }

#pragma endregion [Get, Set] Function

private:
    std::unique_ptr<Model> model_;
    static int nameNum_;

    // ---------- ステートマシン ----------------------------------------
    std::unique_ptr<StateMachine<State<PoisonHoney>>> stateMachine_;
    bool isCreateStateMachine_ = false; // 生成されたか
    // -----------------------------------------------------------------

    bool                isRender_       = true;     // 描画するか
    bool                isAbleAttack_   = false;    // 攻撃できるか

    int type_ = 0; // 種類

    float               speed_      = 0.0f; // 速度
    DirectX::XMFLOAT3   direction_  = {};   // 進行方向

    float               radius_     = 0.0f; // 判定用半径

    bool                isRemove_       = false;    // 消去する
    int                 removeCount_    = 0;        // 待つフレームをカウントする
};

