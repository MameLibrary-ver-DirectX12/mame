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

    int SearchPairIndex();  // �I�Ƀy�A�������Ă�����
    void RemovePair(const int& index);

    void Register(Flower* flower);  // �o�^
    void Clear();                   // �S�폜
    void Remove(Flower* flower);    // �폜

    Flower* GetFlower(const int& index) const { return flowers_.at(index); }

    Flower* GetMostNearFlower(const DirectX::XMFLOAT3& pos);    // �����̈ʒu����ł��߂��Ԃ�Ԃ�
    int GetMostNearFlowerIndex(const DirectX::XMFLOAT3& pos);   // �����̈ʒu����ł��߂��Ԃ̃C���f�b�N�X��Ԃ�


private:
    std::vector<Flower*>    flowers_ = {};
    std::set<Flower*>       removes_ = {};
};

