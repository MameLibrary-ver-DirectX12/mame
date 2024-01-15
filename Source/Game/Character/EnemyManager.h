#pragma once
#include "Enemy.h"
#include <vector>
#include <set>

class EnemyManager
{
public:// --- �萔 ---
    // �G�̎��
    enum class TYPE 
    {
        Bee,        // �I
        Turtle,     // �T
        Bear,       // �F
        Taiyaki,    // �����Ă�
        Mushroom,   // ���̂�
        BoxMan,     // �{�b�N�X�}��
        BoxManEx,   // �{�b�N�X�}��EX

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

    int spawnGauge_ = 0;        // ���G���o��������Q�[�W
    int maxSpawnGauge_ = 0;
};

