#pragma once

#include "Player.h"

class PlayerManager
{
private:
    PlayerManager();
    ~PlayerManager() {}

public:
    static PlayerManager& Instnace()
    {
        static PlayerManager instance;
        return instance;
    }

    void Initialize();
    void Finalize();
    void Update(const float& elapsedTime);
    void Render(ID3D12GraphicsCommandList* commandList);
    void DrawDebug();

public:// --- æ“¾Eİ’è ---
    std::unique_ptr<Player>& GetPlayer() { return player_; }

    int GetFlowerNum() { return flowerNum_; }
    void SetFlowerNum(const int& flowerNum) { flowerNum_ = flowerNum; }

private:
    std::unique_ptr<Player> player_ = nullptr;

    int flowerNum_ = 0;     // ‚Á‚Ä‚¢‚é‰Ô‚Ì”
};

