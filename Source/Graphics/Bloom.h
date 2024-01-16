#pragma once
#include <d3d12.h>
#include <wrl.h>
#include <memory>

#include "FrameBuffer.h"
#include "Descriptor.h"

class Bloom
{
public:
    Bloom();
    ~Bloom() = default;

    void Make();


private:
    Microsoft::WRL::ComPtr<ID3D12Resource> colorResource_;
    Microsoft::WRL::ComPtr<ID3D12Resource> depthResource_;
    Descriptor* rtvDescriptor_;
    Descriptor* srvDescriptor_;
    Descriptor* dsvDescriptor_;
};

