#pragma once

#include <d3d12.h>
#include <wrl.h>

#include "../Graphics/Descriptor.h"

class Texture
{
public:
    Texture(const wchar_t* filename);
    Texture(const char* dummy);
    ~Texture() {}

    Descriptor* GetDescriptor() { return descriptor_; }

private:
    Microsoft::WRL::ComPtr<ID3D12Resource> buffer_;
    D3D12_SHADER_RESOURCE_VIEW_DESC bufferView_;
    Descriptor* descriptor_;
};

