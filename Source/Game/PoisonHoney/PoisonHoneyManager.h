#pragma once
#include "PoisonHoney.h"
#include <vector>
#include <set>

class PoisonHoneyManager
{
public:// --- �萔 ---
    enum class TYPE // ���
    {
        Normal,
        Max,
    };

private:
    PoisonHoneyManager() {}
    ~PoisonHoneyManager() {}

public:
    static PoisonHoneyManager& Instance()
    {
        static PoisonHoneyManager instance;
        return instance;
    }

    void Initialize();                                      // ������
    void Finalize();                                        // �I����
    void Update(const float& elapsedTime);                  // �X�V
    void Render(ID3D12GraphicsCommandList* commandList);    // �`��
    void DrawDebug();                                       // ImGui�p

    void Register(PoisonHoney* poisonHoney);                // �o�^
    void Clear();                                           // �S�폜
    void Remove(PoisonHoney* poisonHoney);                  // �폜

private:
    int possessionNum_[static_cast<int>(TYPE::Max)] = {};   // �e�|�C�Y���n�j�[�̏�����

    std::vector<PoisonHoney*>   poisonHoneies_  = {};
    std::set<PoisonHoney*>      removes_        = {};
};

