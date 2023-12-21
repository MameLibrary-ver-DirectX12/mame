#pragma once
#include <wrl.h>
#include <memory>
#include <xaudio2.h>

#include "Audio.h"

enum class BGM
{
    Dummy,  // ダミー
    Max,    // 最大
};

enum class SE
{
    Dummy,  // ダミー
    Max,    // 最大
};

enum class SE_NAME
{
    Dummy,  // ダミー
    Max,    // 最大
};

class AudioManager
{
private:
    AudioManager() = default;
    ~AudioManager() = default;

public:
    static AudioManager& Instance()
    {
        static AudioManager instance;
        return instance;
    }

    void LoadAudio();   // 音楽読み込み

    // --- BGM再生 ---
    void PlayBGM(
        const BGM& bgm,
        const bool isLoop = true,
        const bool isIgnoreQueue = false
    );

    // --- SE再生 ---
    void PlaySE(
        const SE& se,
        const bool isLoop = false,
        const bool isIgnoreQueue = false
    );

    void StopBGM(const BGM& bgm);   // BGM停止
    void StopSE(const SE& se);      // SE停止

    void StopAllBGM();              // 全BGM停止
    void StopAllSE();               // 全SE停止
    void StopAllAudio();            // 全音楽停止

    // --- SEを連続して再生するための関数 ---
    void PlaySE(SE_NAME who, SE startIndex, SE endIndex);

public:// --- 取得 ---
    std::unique_ptr<Audio>& GetBGM(const BGM& bgm) { return bgm_[static_cast<int>(bgm)]; }
    std::unique_ptr<Audio>& GetSE(const SE& se) { return se_[static_cast<int>(se)]; }

public:
    // XAUDIO2
    Microsoft::WRL::ComPtr<IXAudio2> xAudio2 = nullptr;
    IXAudio2MasteringVoice* masterVoice = nullptr;

private:
    std::unique_ptr<Audio> bgm_[static_cast<int>(BGM::Max)] = {};
    std::unique_ptr<Audio> se_[static_cast<int>(SE::Max)] = {};

    // --- SEを連続して再生するための変数 ---
    int countIndex[static_cast<UINT>(SE_NAME::Max)] = {};
};

