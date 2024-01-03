#pragma once
#include "Flower.h"
#include <vector>
#include <set>

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

    int SearchPairIndex();  // –I‚ÉƒyƒA‚ğŒ©‚Â‚¯‚Ä‚ ‚°‚é
    void RemovePair(const int& index);

    void Register(Flower* flower);  // “o˜^
    void Clear();                   // ‘Síœ
    void Remove(Flower* flower);    // íœ

    Flower* GetFlower(const int& index) const { return flowers_.at(index); }

private:
    std::vector<Flower*>    flowers_ = {};
    std::set<Flower*>       removes_ = {};
};

