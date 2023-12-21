#include "AudioManager.h"

// --- ���\�[�X�ǂݍ��� �E���ʒ��� ---
void AudioManager::LoadAudio()
{
    // --- ���\�[�X�̓ǂݍ��� ---
    {

    }

    // --- ���ʒ��� ---
    {

    }

}

// --- BGM�Đ� ---
void AudioManager::PlayBGM(const BGM& bgm, const bool isLoop, const bool isIgnoreQueue)
{
    bgm_[static_cast<int>(bgm)]->Play(isLoop, isIgnoreQueue);
}

// --- SE�Đ� ---
void AudioManager::PlaySE(const SE& se, const bool isLoop, const bool isIgnoreQueue)
{
    se_[static_cast<int>(se)]->Play(isLoop, isIgnoreQueue);
}

// --- BGM��~ ---
void AudioManager::StopBGM(const BGM& bgm)
{
    bgm_[static_cast<int>(bgm)]->Stop();
}

// --- SE��~ ---
void AudioManager::StopSE(const SE& se)
{
    se_[static_cast<int>(se)]->Stop();
}

// --- �SBGM��~ ---
void AudioManager::StopAllBGM()
{
    for (std::unique_ptr<Audio>& bgm : bgm_)
    {
        bgm->Stop();
    }
}

// --- �SSE��~ ---
void AudioManager::StopAllSE()
{
    for (std::unique_ptr<Audio>& se : se_)
    {
        se->Stop();
    }
}

// --- �SBGM�ESE��~ ---
void AudioManager::StopAllAudio()
{
    for (std::unique_ptr<Audio>& bgm : bgm_)
    {
        bgm->Stop();
    }
    for (std::unique_ptr<Audio>& se : se_)
    {
        se->Stop();
    }
}

// --- SE��A�����ĉ�����Đ����邽�߂ɂ����� ---
void AudioManager::PlaySE(SE_NAME who, SE startIndex, SE endIndex)
{
    // --- �Đ�������SE�̎�� ---
    int currentIndex = countIndex[static_cast<UINT>(who)];

    // --- �Đ�����SE�̔z��擪 ---
    int StartIndex = static_cast<UINT>(startIndex);

    // --- ������Đ�����SE�̏ꏊ���o�� ---
    int playIndex = StartIndex + currentIndex;

    // --- SE�Đ� ---
    se_[playIndex]->Play(false);

    // --- ������SE�z��̍Ō�� ---
    int EndIndex = static_cast<UINT>(endIndex);

    // --- ������SE�̐����擾 ---
    int IndexCount = EndIndex - StartIndex;

    // --- ���ɍĐ�����ԍ��ɐݒ肷�� ---
    if (currentIndex >= IndexCount)
    {
        // --- �ő�܂ōs������ŏ��ɖ߂� ---
        countIndex[static_cast<UINT>(who)] = 0;
    }
    else
    {
        // --- �܂���������ꍇ�͎���ݒ肷�� ---
        ++currentIndex;
        countIndex[static_cast<UINT>(who)] = currentIndex;
    }
}
