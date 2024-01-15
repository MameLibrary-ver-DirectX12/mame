#pragma once
#include "Enemy.h"
#include <vector>
#include <set>

class EnemyManager
{
public:// --- 定数 ---
    // 敵の種類
    enum class TYPE 
    {
        Bee,        // 蜂
        Turtle,     // 亀
        Bear,       // 熊
        Taiyaki,    // たい焼き
        Mushroom,   // きのこ
        BoxMan,     // ボックスマン
        BoxManEx,   // ボックスマンEX

        Max,
    };

private:
    EnemyManager() {}
    ~EnemyManager() {}

public:
    static EnemyManager& Instance()
    {
        static EnemyManager instance;
        return instance;
    }

    void Initialize();
    void Finalize();
    void Update(const float& elapsedTIme);
    void Render(ID3D12GraphicsCommandList* commandList);
    void DrawDebug();

    void Register(Enemy* enemy);
    void Clear();
    void Remove(Enemy* enemy);

public:
    const int GetSpawnGauge() const { return spawnGauge_; }
    void SetSpawnGauge(const int& gauge) { spawnGauge_ = gauge; }
    void AddSpawnGauge() { ++spawnGauge_; }

    const int GetMaxSpawnGauge() const { return maxSpawnGauge_; }
    void SetMaxSpawnGauge(const int& gauge) { maxSpawnGauge_ = gauge; }

private:
    std::vector<Enemy*> enemies_    = {};
    std::set<Enemy*>    removes_    = {};
    std::set<Enemy*>    generates_  = {};

    int spawnGauge_ = 0;        // 強敵を出現させるゲージ
    int maxSpawnGauge_ = 0;
};

