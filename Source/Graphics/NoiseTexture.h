#pragma once

#include <d3d12.h>
#include <wrl.h>

#include "../Graphics/Descriptor.h"

class NoiseTexture
{
private:
    NoiseTexture();
    ~NoiseTexture() {}

public:
    // --- インスタンス取得 ---
    static NoiseTexture& Instance()
    {
        static NoiseTexture instance;
        return instance;
    }

    void PSSetShaderResourceView(ID3D12GraphicsCommandList* commandList, const int& noiseTextureType);

private:
    static const int MAX_NOISE_TEXTURE_NUM = 2; // 最大枚数
    Microsoft::WRL::ComPtr<ID3D12Resource> noiseTexture_[MAX_NOISE_TEXTURE_NUM];

    Descriptor* shaderResourceViewDescriptor_[MAX_NOISE_TEXTURE_NUM] = {};
};

