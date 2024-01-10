#pragma once
#include "../../Resource/Model.h"
#include <string>

class Stage
{
public:
    Stage(const char* fbxFilename, bool triangulate = true, float samplingRate = 0);
    virtual ~Stage() {}

    virtual void Initialize() {}                                // ������
    virtual void Finalize() {}                                  // �I����  
    virtual void Update(const float& elapsedTime) {}            // �X�V
    virtual void Render(ID3D12GraphicsCommandList* commandList,
        DirectX::XMMATRIX world = {});                               // �`��
    virtual void DrawDebug();                                   // ImGui�p

    // --- Transform ---
    Transform* GetTransform() { return model_->GetTransform(); }
    void SetColor(const DirectX::XMFLOAT4 color) { model_->SetColor(color); }

    void SetName(const std::string& name) { model_->SetName(name); }
    std::string GetName() { return model_->GetName(); }

private:
    std::unique_ptr<Model> model_;
};

