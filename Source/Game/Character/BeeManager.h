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

    void Register(Bee* bee);    // 登録
    void Clear();               // 全削除
    void Remove(Bee* bee);      // 削除

    Bee* GetBee(const int& index) const { return bees_.at(index); }

    Bee* GetMostNearBee(const DirectX::XMFLOAT3& pos);      // 引数の位置から最も近い蜂を返す
    int GetMostNearBeeIndex(const DirectX::XMFLOAT3& pos);  // 引数の位置から最も近い蜂のインデックスを返す

private:
    std::vector<Bee*> bees_ = {};
    std::set<Bee*> removes_ = {};
};

