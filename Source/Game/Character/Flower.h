#pragma once
#include "Character.h"

class Flower : public Character
{
public:// --- ’è” ---
    enum class FlowerType
    {
        Blue,
    };

public:
    Flower(const char* fbxFilename);
    ~Flower() override {}

    void Initialize()                       override {} // ‰Šú‰» 
    void Finalize()                         override {} // I—¹‰»
    void Update(const float& elapsedTime)   override {} // XV
    void Render(ID3D12GraphicsCommandList* commandList,
        const DirectX::XMMATRIX& world = {}) override;  // •`‰æ

    void DrawDebug()                        override {} // ImGui—p

public:// --- æ“¾Eİ’è ---
#pragma region [Get, Set] Function
    // --- ‰Ô‚Ìí—Ş ---
    int GetFlowerType() { return static_cast<int>(type_); }
    void SetFlowerType(const FlowerType& type) { type_ = type; }

    // --- –I‚³‚ñ‚Æ‚ÌƒyƒA ---
    bool GetIsPaired() { return isPaired_; }
    void SetIsPaired(const bool& pair) { isPaired_ = pair; }

    bool GetIsEenmyPaired() { return isEnemyPaired_; }
    void SetIsEnemyPaired(const bool& pair) { isEnemyPaired_ = pair; }

    // --- ‚Í‚¿‚İ‚ÂÌæ”ÍˆÍ ---
    float GetRadius() { return radius_; }
    void SetRadius(const float& radius) { radius_ = radius; }

    // --- Ìæ‚É‚©‚©‚éŠÔ ---
    float GetCollectTime() { return collectTime_; }
    void SetCollectTime(float time) { collectTime_ = time; }

#pragma endregion [Get, Set] Function

private:
    FlowerType type_    = FlowerType::Blue; // ‰Ô‚Ìí—Ş
    bool isPaired_      = false;    // –I‚ª©•ª‚Ì‚±‚Æ‚ğŒ©‚Â‚¯‚Ä‚é‚©(ˆê‚Â‚É‚Â‚«ˆê‚Â‚Ì‚½‚ß)
    bool isEnemyPaired_ = false;    // “G‚Ì–I‚ª©•ª‚Ì‚±‚Æ‚ğŒ©‚Â‚¯‚Ä‚é‚©(ˆê‚Â‚É‚Â‚«ˆê‚Â‚Ì‚½‚ß)
    float radius_       = 0.0f;     // ”¼Œa(–I‚ªO‚Â‚ğÌæ‚Å‚«‚é”ÍˆÍ)
    float collectTime_  = 0.0f;     // Ìæ‚Å‚«‚é‚Ü‚Å‚ÌŠÔ
};

