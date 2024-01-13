#pragma once
#include "Character.h"
#include "../../AI/StateMachine.h"

class Enemy : public Character
{
public:
    enum class SearchType
    {
        Point,          // 守っているもの (はちみつ)
        Flower,         // 花
        Bee,            // 蜂
        FlowerAndBee,   // 花と蜂
    };

public:
    Enemy(const char* fbxFilename, bool triangulate = true, float samplingRate = 0);
    ~Enemy() {}

    void Initialize()                       override {} // 初期化 
    void Finalize()                         override {} // 終了化
    void Update(const float& elapsedTime)   override;   // 更新
    void Render(ID3D12GraphicsCommandList* commandList,
        const DirectX::XMMATRIX& world = {}) override;   // 描画

    void DrawDebug()                        override;    // ImGui用

public:// --- 取得・設定 ---
#pragma region [Get, Set] Function
    // ---------- ステートマシン -------------------------------------------------------
    StateMachine<State<Enemy>>* GetStateMachine() { return stateMachine_.get(); }
    // --------------------------------------------------------------------------------

    int GetType() { return type_; }
    void SetType(const int& type) { type_ = type; }        

    const int GetSearchType() const { return searchType_; }
    void SetSearchType(const SearchType& type) { searchType_ = static_cast<UINT>(type); }

    const int GetPairIndex() const { return pairIndex_; }
    void SetPairIndex(const int& index) { pairIndex_ = index; }

    DirectX::XMFLOAT3 GetTargetPos() { return targetPos_; }
    void SetTargetPos(const DirectX::XMFLOAT3& pos) { targetPos_ = pos; }

    const float GetMoveTime() const { return moveTime_; }
    void SetMoveTime(const float& time) { moveTime_ = time; }

    const float GetRadius() const { return radius_; }
    void SetRadius(const float& radius) { radius_ = radius; }

    const float GetSpeed() const { return speed_; }
    void SetSpeed(const float& speed) { speed_ = speed; }

#pragma endregion

private:
    // ---------- ステートマシン ------------------------------
    std::unique_ptr<StateMachine<State<Enemy>>> stateMachine_;
    // -------------------------------------------------------
    
    int                 type_       = 0;    // 自分がどの種類の敵なのかを記憶しておく

    // ターゲットを決めるためにどの種類をターゲットにするか用
    int                 searchType_ = 0;

    int                 pairIndex_  = 0;    // 目標物
    DirectX::XMFLOAT3   targetPos_  = {};   // 目標座標

    float               moveTime_   = 0.0f; // 移動時間 (0じゃないときは移動できる)

    float               radius_     = 0.0f; // 半径
    float               speed_      = 0.0f; // 速度

protected:
    // ---------- ImGui用 --------------------
    static int nameNum_;
    // ---------------------------------------
};

