#pragma once
#include "../../Resource/Sprite.h"

#include "../PoisonHoney/PoisonHoneyManager.h"

class GameUI
{
public:
    GameUI();
    ~GameUI() {}

    void Initialize();
    void Finalize();
    void Update(const float& elapsedTime);
    void Render(ID3D12GraphicsCommandList* commandList);
    void DrawDebug();

private:
    std::unique_ptr<Sprite> poisonHoneyNum_[static_cast<UINT>(PoisonHoneyManager::TYPE::Max)];
};

