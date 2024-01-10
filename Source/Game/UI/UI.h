#pragma once
#include "../../Resource/Sprite.h"



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

    void NumRender(ID3D12GraphicsCommandList* commandList, const int& num, const float& firstPosX, const float& secondPosX, const float& thirdPosX);

private:
    std::unique_ptr<Sprite> numSprite_[3];

    std::unique_ptr<Sprite> flowerSprite_;
    std::unique_ptr<Sprite> poisonHoneySprite_;
};

