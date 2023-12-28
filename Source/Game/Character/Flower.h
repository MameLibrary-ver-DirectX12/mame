#pragma once
#include "Character.h"

class Flower : public Character
{
public:// --- �萔 ---
    enum class FlowerType
    {
        Blue,
    };

public:
    Flower(const char* fbxFilename);
    ~Flower() override {}

    void Initialize()                       override {} // ������ 
    void Finalize()                         override {} // �I����
    void Update(const float& elapsedTime)   override {} // �X�V
    void Render(ID3D12GraphicsCommandList* commandList,
        const DirectX::XMMATRIX world = {}) override;   // �`��

    void DrawDebug()                        override {} // ImGui�p

public:// --- �擾�E�ݒ� ---
#pragma region [Get, Set] Function
    // --- �Ԃ̎�� ---
    int GetFlowerType() { return static_cast<int>(type_); }
    void SetFlowerType(const FlowerType& type) { type_ = type; }

    // --- �I����Ƃ̃y�A ---
    bool GetIsPaired() { return isPaired_; }
    void SetIsPaired(const bool& pair) { isPaired_ = pair; }

    // --- �͂��݂̎�͈� ---
    float GetRadius() { return radius_; }
    void SetRadius(const float& radius) { radius_ = radius; }

    // --- �̎�ɂ����鎞�� ---
    float GetCollectTime() { return collectTime_; }
    void SetCollectTime(float time) { collectTime_ = time; }

#pragma endregion [Get, Set] Function

private:
    FlowerType type_    = FlowerType::Blue; // �Ԃ̎��
    bool isPaired_      = false;    // �I�������̂��Ƃ������Ă邩(��ɂ���̂���)
    float radius_       = 0.0f;     // ���a(�I���O���̎�ł���͈�)
    float collectTime_  = 0.0f;     // �̎�ł���܂ł̎���
};

