#include "NoiseTexture.h"

#include "Graphics.h"

// --- コンストラクタ ---
NoiseTexture::NoiseTexture()
{
    HRESULT result = S_OK;

    Graphics& graphics = Graphics::Instance();

    result = graphics.LoadTexture("./Resources/Image/Noise/mask.png", noiseTexture_[0].GetAddressOf());

    for (int index = 0; index < MAX_NOISE_TEXTURE_NUM; ++index)
    {
        // --- ディスクリプタの取得 ---
        shaderResourceViewDescriptor_[index] = graphics.GetSrvCbvUavDescriptorHeap()->PopDescriptor();

        // --- シェーダーリソースビューの生成 ---
        D3D12_RESOURCE_DESC resourceDesc = noiseTexture_[index]->GetDesc();

        // --- シェーダーリソースビューの設定 ---
        D3D12_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc = {};
        shaderResourceViewDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
        shaderResourceViewDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
        shaderResourceViewDesc.Format = resourceDesc.Format;
        shaderResourceViewDesc.Texture2D.MipLevels = resourceDesc.MipLevels;
        shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;

        // --- シェーダーリソースビューを生成 ---
        graphics.GetDevice()->CreateShaderResourceView(
            noiseTexture_[index].Get(),
            &shaderResourceViewDesc,
            shaderResourceViewDescriptor_[index]->GetCpuHandle()
        );
    }
}

// --- GPU側に送る ---
void NoiseTexture::PSSetShaderResourceView(ID3D12GraphicsCommandList* commandList, const int& noiseTextureType)
{
    commandList->SetGraphicsRootDescriptorTable(2, shaderResourceViewDescriptor_[noiseTextureType]->GetGpuHandle());
}
