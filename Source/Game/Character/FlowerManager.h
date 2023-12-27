#pragma once
#include "Flower.h"

class FlowerManager
{
private:
    FlowerManager() {}
    ~FlowerManager() {}

public:
    static FlowerManager& Instance()
    {
        static FlowerManager instance;
        return instance;
    }

    void Initialize();
    void Finalize();
    void Update(const float& elapsedTime);
    void Render(ID3D12GraphicsCommandList* commandList);
    void DrawDebug();

private:

};

