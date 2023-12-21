#include "AudioManager.h"

// --- リソース読み込み ・音量調整 ---
void AudioManager::LoadAudio()
{
    // --- リソースの読み込み ---
    {

    }

    // --- 音量調整 ---
    {

    }

}

// --- BGM再生 ---
void AudioManager::PlayBGM(const BGM& bgm, const bool isLoop, const bool isIgnoreQueue)
{
    bgm_[static_cast<int>(bgm)]->Play(isLoop, isIgnoreQueue);
}

// --- SE再生 ---
void AudioManager::PlaySE(const SE& se, const bool isLoop, const bool isIgnoreQueue)
{
    se_[static_cast<int>(se)]->Play(isLoop, isIgnoreQueue);
}

// --- BGM停止 ---
void AudioManager::StopBGM(const BGM& bgm)
{
    bgm_[static_cast<int>(bgm)]->Stop();
}

// --- SE停止 ---
void AudioManager::StopSE(const SE& se)
{
    se_[static_cast<int>(se)]->Stop();
}

// --- 全BGM停止 ---
void AudioManager::StopAllBGM()
{
    for (std::unique_ptr<Audio>& bgm : bgm_)
    {
        bgm->Stop();
    }
}

// --- 全SE停止 ---
void AudioManager::StopAllSE()
{
    for (std::unique_ptr<Audio>& se : se_)
    {
        se->Stop();
    }
}

// --- 全BGM・SE停止 ---
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

// --- SEを連続して何回も再生するためにいるやつ ---
void AudioManager::PlaySE(SE_NAME who, SE startIndex, SE endIndex)
{
    // --- 再生数するSEの種類 ---
    int currentIndex = countIndex[static_cast<UINT>(who)];

    // --- 再生するSEの配列先頭 ---
    int StartIndex = static_cast<UINT>(startIndex);

    // --- 今から再生するSEの場所を出す ---
    int playIndex = StartIndex + currentIndex;

    // --- SE再生 ---
    se_[playIndex]->Play(false);

    // --- 自分のSE配列の最後尾 ---
    int EndIndex = static_cast<UINT>(endIndex);

    // --- 自分のSEの数を取得 ---
    int IndexCount = EndIndex - StartIndex;

    // --- 次に再生する番号に設定する ---
    if (currentIndex >= IndexCount)
    {
        // --- 最大まで行ったら最初に戻す ---
        countIndex[static_cast<UINT>(who)] = 0;
    }
    else
    {
        // --- まだ次がある場合は次を設定する ---
        ++currentIndex;
        countIndex[static_cast<UINT>(who)] = currentIndex;
    }
}
