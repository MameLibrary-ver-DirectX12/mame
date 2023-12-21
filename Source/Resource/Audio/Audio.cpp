#include "Audio.h"

#include <windows.h>
#include <winerror.h>

HRESULT FindChunk(HANDLE hFile, DWORD fourcc, DWORD& chunkSize, DWORD& chunkDataPosition)
{
    HRESULT hr = S_OK;

    if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, 0, NULL, FILE_BEGIN))
    {
        return HRESULT_FROM_WIN32(GetLastError());
    }

    DWORD chunkType;
    DWORD chunkDataSize;
    DWORD riffDataSize = 0;
    DWORD fileType;
    DWORD bytesRead = 0;
    DWORD offset = 0;

    while (hr == S_OK)
    {
        DWORD numberOfBytesRead;
        if (0 == ReadFile(hFile, &chunkType, sizeof(DWORD), &numberOfBytesRead, NULL))
        {
            hr = HRESULT_FROM_WIN32(GetLastError());
        }

        if (0 == ReadFile(hFile, &chunkDataSize, sizeof(DWORD), &numberOfBytesRead, NULL))
        {
            hr = HRESULT_FROM_WIN32(GetLastError());
        }

        switch (chunkType)
        {
        case 'FFIR'/*RIFF*/:
            riffDataSize = chunkDataSize;
            chunkDataSize = 4;
            if (0 == ReadFile(hFile, &fileType, sizeof(DWORD), &numberOfBytesRead, NULL))
            {
                hr = HRESULT_FROM_WIN32(GetLastError());
            }
            break;

        default:
            if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, chunkDataSize, NULL, FILE_CURRENT))
            {
                return HRESULT_FROM_WIN32(GetLastError());
            }
        }

        offset += sizeof(DWORD) * 2;

        if (chunkType == fourcc)
        {
            chunkSize = chunkDataSize;
            chunkDataPosition = offset;
            return S_OK;
        }

        offset += chunkDataSize;

        if (bytesRead >= riffDataSize)
        {
            return S_FALSE;
        }
    }

    return S_OK;
}

HRESULT ReadChunkData(HANDLE hFile, LPVOID buffer, DWORD bufferSize, DWORD bufferOffset)
{
    HRESULT hr = S_OK;
    if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, bufferOffset, NULL, FILE_BEGIN))
    {
        return HRESULT_FROM_WIN32(GetLastError());
    }
    DWORD numberOfBytesRead;
    if (0 == ReadFile(hFile, buffer, bufferSize, &numberOfBytesRead, NULL))
    {
        hr = HRESULT_FROM_WIN32(GetLastError());
    }
    return hr;
}

// --- コンストラクタ ---
Audio::Audio(IXAudio2* xaudio2, const wchar_t* filename)
{
    HRESULT result;

    // Open the file
    HANDLE hFile = CreateFileW(filename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
    if (INVALID_HANDLE_VALUE == hFile)
    {
        result = HRESULT_FROM_WIN32(GetLastError());
        _ASSERT_EXPR(SUCCEEDED(result), HRTrace(result));
    }

    if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, 0, NULL, FILE_BEGIN))
    {
        result = HRESULT_FROM_WIN32(GetLastError());
        _ASSERT_EXPR(SUCCEEDED(result), HRTrace(result));
    }

    DWORD chunkSize;
    DWORD chunkPosition;
    // check the file type, should be 'WAVE' or 'XWMA'
    FindChunk(hFile, 'FFIR'/*RIFF*/, chunkSize, chunkPosition);
    DWORD fileType;
    ReadChunkData(hFile, &fileType, sizeof(DWORD), chunkPosition);
    _ASSERT_EXPR(fileType == 'EVAW'/*WAVE*/, L"Onlt support 'WAVE'");

    FindChunk(hFile, ' tmf'/*FMT*/, chunkSize, chunkPosition);
    ReadChunkData(hFile, &wfx_, chunkSize, chunkPosition);

    // file out the audio data buffer with the contents of the fourccDATA chunk
    FindChunk(hFile, 'atad'/*DATA*/, chunkSize, chunkPosition);
    BYTE* data = new BYTE[chunkSize];
    ReadChunkData(hFile, data, chunkSize, chunkPosition);

    buffer_.AudioBytes = chunkSize;  // size of the audio buffer in bytes
    buffer_.pAudioData = data;   // buffer containing audio data;
    buffer_.Flags = XAUDIO2_END_OF_STREAM;   // tell the source voice not to expect any data after this buffer

    result = xaudio2->CreateSourceVoice(&sourceVoice_, (WAVEFORMATEX*)&wfx_);
    _ASSERT_EXPR(SUCCEEDED(result), HRTrace(result));
}

// --- デストラクタ ---
Audio::~Audio()
{
    sourceVoice_->DestroyVoice();
    delete[] buffer_.pAudioData;
}

// --- 再生 ---
void Audio::Play(const int loopCount)
{
    HRESULT result;

    XAUDIO2_VOICE_STATE voiceState = {};
    sourceVoice_->GetState(&voiceState);

    if (voiceState.BuffersQueued)
    {
        return;
    }

    buffer_.LoopCount = loopCount;
    result = sourceVoice_->SubmitSourceBuffer(&buffer_);
    _ASSERT_EXPR(SUCCEEDED(result), HRTrace(result));

    result = sourceVoice_->Start(0);
    _ASSERT_EXPR(SUCCEEDED(result), HRTrace(result));
}

// --- 再生 ---
void Audio::Play(const bool isLoop, const bool isIgnoreQueue)
{
    HRESULT result;

    XAUDIO2_VOICE_STATE voiceState = {};
    sourceVoice_->GetState(&voiceState);

    if (!isIgnoreQueue && voiceState.BuffersQueued) return;

    const int loopCount = isLoop ? XAUDIO2_LOOP_INFINITE : 0;

    buffer_.LoopCount = loopCount;
    result = sourceVoice_->SubmitSourceBuffer(&buffer_);
    _ASSERT_EXPR(SUCCEEDED(result), HRTrace(result));

    result = sourceVoice_->Start(0);
    _ASSERT_EXPR(SUCCEEDED(result), HRTrace(result));
}

// --- 停止 ---
void Audio::Stop(bool playTails, size_t afterSamplesPlayed)
{
    XAUDIO2_VOICE_STATE voiceState = {};
    sourceVoice_->GetState(&voiceState);
    if (!voiceState.BuffersQueued)
    {
        return;
    }

    if (voiceState.SamplesPlayed < afterSamplesPlayed)
    {
        return;
    }

    HRESULT result;
    result = sourceVoice_->Stop(playTails ? XAUDIO2_PLAY_TAILS : 0);
    _ASSERT_EXPR(SUCCEEDED(result), HRTrace(result));

    result = sourceVoice_->FlushSourceBuffers();
    _ASSERT_EXPR(SUCCEEDED(result), HRTrace(result));
}

// --- 音量調整 ---
void Audio::Volume(float volume)
{
    HRESULT result = sourceVoice_->SetVolume(volume);
    _ASSERT_EXPR(SUCCEEDED(result), HRTrace(result));
}

// --- キューイング ---
bool Audio::Queuing()
{
    XAUDIO2_VOICE_STATE voiceState = {};
    sourceVoice_->GetState(&voiceState);
    return voiceState.BuffersQueued;
}
