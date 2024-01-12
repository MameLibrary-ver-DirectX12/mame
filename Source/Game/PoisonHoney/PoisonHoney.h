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

    virtual void Initialize() {}                                    // ������
    virtual void Finalize() {}                                      // �I����
    virtual void Update(const float& elapsedTime);                  // �X�V
    virtual void Render(ID3D12GraphicsCommandList* commandList,
        const DirectX::XMMATRIX& world = {});                       // �`��
    virtual void DrawDebug();                                       // ImGui�p

    void CreateStateMachine();

public:// --- �擾�E�ݒ� ---
#pragma region [Get, Set] Function
    // --- Transform ---
    Transform* GetTransform() { return model_->GetTransform(); }
    DirectX::XMFLOAT4 GetColor() { return model_->GetColor(); }
    void SetColor(const DirectX::XMFLOAT4& color) { model_->SetColor(color); }
    void SetColor(const float& r, const float& g, const float& b, const float& a) { model_->SetColor({ r, g, b, a }); }

    // --- �X�e�[�g�}�V�� ---
    StateMachine<State<PoisonHoney>>* GetStateMachine() { return stateMachine_.get(); }
    void SetState(STATE state) { GetStateMachine()->ChangeState(static_cast<int>(state)); }

    // --- �`��t���O ---
    bool GetIsRender() { return isRender_; }
    void SetIsRender(const bool& isRender) { isRender_ = isRender; }

    // --- �U���ł��邩�t���O ---
    bool GetIsAbleAttack() { return isAbleAttack_; }
    void SetIsAbleAttack(const bool& isAbleAttack) { isAbleAttack_ = isAbleAttack; }

    // --- ��� ---
    int GetType() { return type_; }
    void SetType(int type) { type_ = type; }

    // --- ���x ---
    float GetSpeed() { return speed_; }
    void SetSpeed(const float& speed) { speed_ = speed; }

    // --- �i�s���� ---
    DirectX::XMFLOAT3 GetDirection() { return direction_; }
    void SetDirection(const DirectX::XMFLOAT3& direction) { direction_ = direction; }

    // --- ���a ---
    float GetRadius() { return radius_; }
    void SetRadius(const float& radius) { radius_ = radius; }

    bool GetIsRemove() { return isRemove_; }
    void SetIsRemove(const bool& remove) { isRemove_ = remove; }
    int GetRemoveCount() { return removeCount_; }
    void SetRemoveCount(const int& count) { removeCount_ = count; }
    void AddRemoveCount() { ++removeCount_; }

    // --- ImGui�p ---
    const char* const GetName() const { return model_->GetName(); }
    void SetName(const std::string& name) { model_->SetName(name + std::to_string(nameNum_++)); }

#pragma endregion [Get, Set] Function

private:
    std::unique_ptr<Model> model_;
    static int nameNum_;

    // ---------- �X�e�[�g�}�V�� ----------------------------------------
    std::unique_ptr<StateMachine<State<PoisonHoney>>> stateMachine_;
    bool isCreateStateMachine_ = false; // �������ꂽ��
    // -----------------------------------------------------------------

    bool                isRender_       = true;     // �`�悷�邩
    bool                isAbleAttack_   = false;    // �U���ł��邩

    int type_ = 0; // ���

    float               speed_      = 0.0f; // ���x
    DirectX::XMFLOAT3   direction_  = {};   // �i�s����

    float               radius_     = 0.0f; // ����p���a

    bool                isRemove_       = false;    // ��������
    int                 removeCount_    = 0;        // �҂t���[�����J�E���g����
};

