#pragma once

#include <vector>

#include "ModelResource.h"
#include "../Graphics/ConstantBuffer.h"
#include "Transform.h"

class Model
{
public:
    Model(const char* fbxFilename, bool triangulate = false, float samplingRate = 0);
    ~Model();

    void Render(ID3D12GraphicsCommandList* commandList);
    void DrawDebug();

public:// éÊìæÅEê›íË
    Transform* GetTransform() { return &transform_; }

private:
    ModelResource modelResource_;
    std::unique_ptr<ConstantBuffer> constantBuffer_;
    Transform transform_;
};