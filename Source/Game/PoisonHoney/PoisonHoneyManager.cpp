#include "PoisonHoneyManager.h"
#include "../../Other/Common.h"

// --- ������ ---
void PoisonHoneyManager::Initialize()
{
    for (PoisonHoney*& poisonHoney : poisonHoneies_)
    {
        poisonHoney->Initialize();
    }
}

// --- �I���� ---
void PoisonHoneyManager::Finalize()
{
    for (PoisonHoney*& poisonHoney : poisonHoneies_)
    {
        poisonHoney->Finalize();
    }
}

// --- �X�V ---
void PoisonHoneyManager::Update(const float& elapsedTime)
{
    // �X�V
    for (PoisonHoney*& poisonHoney : poisonHoneies_)
    {
        poisonHoney->Update(elapsedTime);
    }

    // �J��
    for (PoisonHoney* poisonHoney : removes_)
    {
        std::vector<PoisonHoney*>::iterator it = std::find(poisonHoneies_.begin(), poisonHoneies_.end(), poisonHoney);

        if (it != poisonHoneies_.end())
        {
            poisonHoneies_.erase(it);
        }

        SafeDeletePtr(poisonHoney);
    }
    removes_.clear();
}

// --- �`�� ---
void PoisonHoneyManager::Render(ID3D12GraphicsCommandList* commandList)
{
    for (PoisonHoney*& poisonHoney : poisonHoneies_)
    {
        poisonHoney->Render(commandList);
    }
}

// --- ImGui�p ---
void PoisonHoneyManager::DrawDebug()
{
    for (PoisonHoney*& poisonHoney : poisonHoneies_)
    {
        poisonHoney->DrawDebug();
    }
}

// --- �o�^ ---
void PoisonHoneyManager::Register(PoisonHoney* poisonHoney)
{
    poisonHoneies_.emplace_back(poisonHoney);

    // ���������J�E���g���� (��ނ��Ƃ�)
    ++possessionNum_[poisonHoney->GetType()];
}

// --- �S�폜 ---
void PoisonHoneyManager::Clear()
{
    for (PoisonHoney*& poisonHoney : poisonHoneies_)
    {
        SafeDeletePtr(poisonHoney);
    }
    poisonHoneies_.clear();
    poisonHoneies_.shrink_to_fit();

    // �����������Z�b�g
    for (int typeIndex = 0; typeIndex < static_cast<int>(TYPE::Max); ++typeIndex)
    {
        possessionNum_[typeIndex] = 0;
    }
}

// --- �폜 ---
void PoisonHoneyManager::Remove(PoisonHoney* poisonHoney)
{
    // �����������炷
    --possessionNum_[poisonHoney->GetType()];

    // �������X�g�ɓo�^
    removes_.insert(poisonHoney);
}
