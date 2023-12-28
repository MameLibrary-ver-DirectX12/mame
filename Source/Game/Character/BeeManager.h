#pragma once
#include "Bee.h"
#include <vector>
#include <set>

class BeeManager
{
private:
    BeeManager() {}
    ~BeeManager() {}

public:
    static BeeManager& Instance()
    {
        static BeeManager instance;
        return instance;
    }

    void Initialize();
    void Finalize();
    void Update(const float& elapsedTime);
    void Render(ID3D12GraphicsCommandList* commandList);
    void DrawDebug();

    void Register(Bee* bee);    // “o˜^
    void Clear();               // ‘Síœ
    void Remove(Bee* bee);      // íœ

private:
    std::vector<Bee*> bees_ = {};
    std::set<Bee*> removes_ = {};
};

