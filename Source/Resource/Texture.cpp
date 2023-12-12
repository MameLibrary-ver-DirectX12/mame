#include "Texture.h"

#include "../Graphics/Graphics.h"

// --- �R���X�g���N�^ ---
Texture::Texture(const wchar_t* filename)
{
    // --- �e�N�X�`���̃��[�h ---
    Graphics::Instance().LoadTextureFromFile(filename, buffer_.GetAddressOf());

    bufferView_.Format = buffer_->GetDesc().Format;
    bufferView_.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
    bufferView_.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
    bufferView_.Texture2D.MipLevels = 1;

    descriptor_ = Graphics::Instance().GetSrvCbvUavDescriptorHeap()->PopDescriptor();

    Graphics::Instance().GetDevice()->CreateShaderResourceView(
        buffer_.Get(),
        &bufferView_,
        descriptor_->GetCpuHandle()
    );
}

Texture::Texture(const char* dummy)
{
    // --- �_�~�[�e�N�X�`���̃��[�h ---
    Graphics::Instance().CreateDummyTexture(buffer_.GetAddressOf());

    bufferView_.Format = buffer_->GetDesc().Format;
    bufferView_.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
    bufferView_.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
    bufferView_.Texture2D.MipLevels = 1;

    descriptor_ = Graphics::Instance().GetSrvCbvUavDescriptorHeap()->PopDescriptor();

    Graphics::Instance().GetDevice()->CreateShaderResourceView(
        buffer_.Get(),
        &bufferView_,
        descriptor_->GetCpuHandle()
    );
}
