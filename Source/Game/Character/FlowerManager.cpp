#include "FlowerManager.h"
#include "../../Other/Common.h"

// --- ������ ---
void FlowerManager::Initialize()
{
    for (Flower*& flower : flowers_)
    {
        flower->Initialize();
    }
}

// --- �I���� ---
void FlowerManager::Finalize()
{
    for (Flower*& flower : flowers_)
    {
        flower->Finalize();
    }
    Clear();
}

// --- �X�V ---
void FlowerManager::Update(const float& elapsedTime)
{
    // --- �X�V ---
    for (Flower*& flower : flowers_)
    {
        flower->Update(elapsedTime);
    }

    // --- �J�� ---
    for (Flower* flower : removes_)
    {
        std::vector<Flower*>::iterator it = std::find(flowers_.begin(), flowers_.end(), flower);

        if (it != flowers_.end())
        {
            flowers_.erase(it);
        }

        SafeDeletePtr(flower);
    }
    removes_.clear();
}

// --- �`�� ---
void FlowerManager::Render(ID3D12GraphicsCommandList* commandList)
{
    for (Flower*& flower : flowers_)
    {
        flower->Render(commandList);
    }
}

// --- ImGui�p ---
void FlowerManager::DrawDebug()
{
    if (ImGui::BeginMenu("FlowerManager"))
    {
        for (Flower*& flower : flowers_)
        {
            flower->DrawDebug();
        }

        ImGui::EndMenu();
    }
}

// --- �I�Ƀy�A�������Ă����� ---
int FlowerManager::SearchPairIndex()
{
    // todo : �Ԃ����񂾂Ƃ��I����ɕ񍐂��Ȃ��Ƃ����Ȃ��B
    // �łȂ��Ƒ����o�O��

    int num = -1; // 0����n�߂邽�߂ɍŏ���-1�ɂ��Ă���

    for (Flower*& flower : flowers_)
    {
        ++num; // �ԍ��𐔂���

        // --- ���łɃy�A������Ƃ��͖߂� ---
        if (flower->GetIsPaired()) continue;

        // --- �y�A�ɂȂ��� ---
        flower->SetIsPaired(true);

        return num; // vector�̃C���f�b�N�X��Ԃ�
    }

    return -1; // ���s
}

// --- �y�A���� ---
void FlowerManager::RemovePair(const int& index)
{
    int num = -1;

    for (Flower*& flower : flowers_)
    {
        ++num; // �ԍ��𐔂���

        if (num != index) continue;
     
        // �y�A����
        flower->SetIsPaired(false);

        return;
    }
}

// --- �o�^ ---
void FlowerManager::Register(Flower* flower)
{
    flowers_.emplace_back(flower);
}

// --- �S�폜 ---
void FlowerManager::Clear()
{
    for (Flower*& flower : flowers_)
    {
        SafeDeletePtr(flower);
    }
    flowers_.clear();
    flowers_.shrink_to_fit();
}

// --- �폜 ---
void FlowerManager::Remove(Flower* flower)
{
    removes_.insert(flower);
}
