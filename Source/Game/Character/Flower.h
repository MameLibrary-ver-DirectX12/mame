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
    int GetFlowerType() { return static_cast<int>(type_); }
    void SetFlowerType(FlowerType type) { type_ = type; }

private:
    FlowerType type_ = FlowerType::Blue; // �Ԃ̎��
};

