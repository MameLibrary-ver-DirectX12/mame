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

public:// --- 取得・設定 ---
    std::unique_ptr<Player>& GetPlayer() { return player_; }

    int GetFlowerNum() { return flowerNum_; }
    void SetFlowerNum(const int& flowerNum) { flowerNum_ = flowerNum; }

private:
    std::unique_ptr<Player> player_ = nullptr;

    int flowerNum_ = 0;     // 持っている花の数
};

