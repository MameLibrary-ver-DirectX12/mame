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

    void Register(Bee* bee);    // �o�^
    void Clear();               // �S�폜
    void Remove(Bee* bee);      // �폜

    Bee* GetBee(const int& index) const { return bees_.at(index); }

    Bee* GetMostNearBee(const DirectX::XMFLOAT3& pos);      // �����̈ʒu����ł��߂��I��Ԃ�
    int GetMostNearBeeIndex(const DirectX::XMFLOAT3& pos);  // �����̈ʒu����ł��߂��I�̃C���f�b�N�X��Ԃ�

private:
    std::vector<Bee*> bees_ = {};
    std::set<Bee*> removes_ = {};
};

