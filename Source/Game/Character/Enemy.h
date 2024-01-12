#pragma once
#include "Character.h"
#include "../../AI/StateMachine.h"

class Enemy : public Character
{
public:
    enum class SearchType
    {
        Point,          // ����Ă������ (�͂��݂�)
        Flower,         // ��
        Bee,            // �I
        FlowerAndBee,   // �ԂƖI
    };

public:
    Enemy(const char* fbxFilename, bool triangulate = true, float samplingRate = 0);
    ~Enemy() {}

    void Initialize()                       override {} // ������ 
    void Finalize()                         override {} // �I����
    void Update(const float& elapsedTime)   override;   // �X�V
    void Render(ID3D12GraphicsCommandList* commandList,
        const DirectX::XMMATRIX& world = {}) override;   // �`��

    void DrawDebug()                        override {} // ImGui�p

public:// --- �擾�E�ݒ� ---
#pragma region [Get, Set] Function
    // ---------- �X�e�[�g�}�V�� -------------------------------------------------------
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

#pragma endregion

private:
    std::unique_ptr<StateMachine<State<Enemy>>> stateMachine_;
    
    int type_ = 0;


    // �^�[�Q�b�g�����߂邽�߂ɂǂ̎�ނ��^�[�Q�b�g�ɂ��邩�p
    int searchType_ = 0;

    int                 pairIndex_ = 0;  // �ڕW��
    DirectX::XMFLOAT3   targetPos_ = {}; // �ڕW���W

    float               moveTime_      = 0.0f; // �ړ����� (0����Ȃ��Ƃ��͈ړ��ł���)
};

