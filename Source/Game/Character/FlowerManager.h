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

    int SearchPairIndex();  // 蜂にペアを見つけてあげる
    void RemovePair(const int& index);

    void Register(Flower* flower);  // 登録
    void Clear();                   // 全削除
    void Remove(Flower* flower);    // 削除

    Flower* GetFlower(const int& index) const { return flowers_.at(index); }

    Flower* GetMostNearFlower(const DirectX::XMFLOAT3& pos);    // 引数の位置から最も近い花を返す
    int GetMostNearFlowerIndex(const DirectX::XMFLOAT3& pos);   // 引数の位置から最も近い花のインデックスを返す


private:
    std::vector<Flower*>    flowers_ = {};
    std::set<Flower*>       removes_ = {};
};

