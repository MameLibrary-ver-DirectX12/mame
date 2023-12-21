#pragma once
#include <wrl.h>
#include <memory>
#include <xaudio2.h>

#include "Audio.h"

enum class BGM
{
    Dummy,  // �_�~�[
    Max,    // �ő�
};

enum class SE
{
    Dummy,  // �_�~�[
    Max,    // �ő�
};

enum class SE_NAME
{
    Dummy,  // �_�~�[
    Max,    // �ő�
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

    void LoadAudio();   // ���y�ǂݍ���

    // --- BGM�Đ� ---
    void PlayBGM(
        const BGM& bgm,
        const bool isLoop = true,
        const bool isIgnoreQueue = false
    );

    // --- SE�Đ� ---
    void PlaySE(
        const SE& se,
        const bool isLoop = false,
        const bool isIgnoreQueue = false
    );

    void StopBGM(const BGM& bgm);   // BGM��~
    void StopSE(const SE& se);      // SE��~

    void StopAllBGM();              // �SBGM��~
    void StopAllSE();               // �SSE��~
    void StopAllAudio();            // �S���y��~

    // --- SE��A�����čĐ����邽�߂̊֐� ---
    void PlaySE(SE_NAME who, SE startIndex, SE endIndex);

public:// --- �擾 ---
    std::unique_ptr<Audio>& GetBGM(const BGM& bgm) { return bgm_[static_cast<int>(bgm)]; }
    std::unique_ptr<Audio>& GetSE(const SE& se) { return se_[static_cast<int>(se)]; }

public:
    // XAUDIO2
    Microsoft::WRL::ComPtr<IXAudio2> xAudio2 = nullptr;
    IXAudio2MasteringVoice* masterVoice = nullptr;

private:
    std::unique_ptr<Audio> bgm_[static_cast<int>(BGM::Max)] = {};
    std::unique_ptr<Audio> se_[static_cast<int>(SE::Max)] = {};

    // --- SE��A�����čĐ����邽�߂̕ϐ� ---
    int countIndex[static_cast<UINT>(SE_NAME::Max)] = {};
};

