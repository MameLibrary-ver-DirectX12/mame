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
    Clear();
}

// --- �X�V ---
void PoisonHoneyManager::Update(const float& elapsedTime)
{   
    // �X�V
    for (PoisonHoney*& poisonHoney : poisonHoneies_)
    {
        poisonHoney->Update(elapsedTime);
    }

    float minPosX = -100.0f;
    float maxPosX = 100.0f;
    float minPosZ = -100.0f;
    float maxPosZ = 16.0f;

    for (PoisonHoney*& poisonHoney : poisonHoneies_)
    {
        // �͈͊O����
        if (poisonHoney->GetTransform()->GetPositionX() < minPosX ||
            poisonHoney->GetTransform()->GetPositionZ() > maxPosX ||
            poisonHoney->GetTransform()->GetPositionZ() < minPosZ ||
            poisonHoney->GetTransform()->GetPositionZ() > maxPosZ)
        {
            poisonHoney->SetIsRemove(true);
            poisonHoney->SetIsRender(false);
        }

        // �����o�^���ꂽ����J�E���g����
        if (poisonHoney->GetIsRemove())
        {
            if (poisonHoney->GetRemoveCount() <= Graphics::Instance().GetBufferCount() + 1)
            {
                poisonHoney->AddRemoveCount();
            }
            else
            {
                Remove(poisonHoney);
            }
        }
    }    

    // �J��
    for (PoisonHoney* poisonHoney : removes_)
    {
        auto it = std::find(poisonHoneies_.begin(), poisonHoneies_.end(), poisonHoney);

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
        if (poisonHoney->GetIsRender()) poisonHoney->Render(commandList);
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

// --- �U���\�ȃ|�C�Y���n�j�[�̐� ---
int PoisonHoneyManager::GetAbleAttckNum(TYPE type)
{
    int total = 0;

    for (PoisonHoney*& poisonHoney : poisonHoneies_)
    {
        // �Ⴄ���
        if (poisonHoney->GetType() != static_cast<UINT>(type)) continue;

        // �U���\�����J�E���g����
        if (poisonHoney->GetIsAbleAttack())
        {
            ++total;
        }

    }

    return total;
}

// --- �U�� ---
bool PoisonHoneyManager::Shot(TYPE type)
{
    int num = 0;

    for (PoisonHoney*& poisonHoney : poisonHoneies_)
    {
        // �Ⴄ���
        if (poisonHoney->GetType() != static_cast<UINT>(type)) continue;

        // �U���ł��Ȃ�
        if (!poisonHoney->GetIsAbleAttack()) continue;

        // �U��
        poisonHoney->SetState(PoisonHoney::STATE::Attack);
        return true;
    }

    return false;
}
