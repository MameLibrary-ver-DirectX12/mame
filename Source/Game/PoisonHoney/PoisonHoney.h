#pragma once
#include "../../Resource/Model.h"
#include <string>

class PoisonHoney
{
public:
    PoisonHoney(const char* fbxFilename, bool triangulate = false, float samplingRate = 0);
    virtual ~PoisonHoney();

    virtual void Initialize() {}                                    // ������
    virtual void Finalize() {}                                      // �I����
    virtual void Update(const float& elapsedTime) {}                // �X�V
    virtual void Render(ID3D12GraphicsCommandList* commandList,
        const DirectX::XMMATRIX& world = {});                       // �`��
    virtual void DrawDebug();                                       // ImGui�p

public:// --- �擾�E�ݒ� ---
#pragma region [Get, Set] Function
    // --- Transform ---
    Transform* GetTransform() { return model_->GetTransform(); }
    DirectX::XMFLOAT4 GetColor() { return model_->GetColor(); }
    void SetColor(const DirectX::XMFLOAT4& color) { model_->SetColor(color); }
    void SetColor(const float& r, const float& g, const float& b, const float& a) { model_->SetColor({ r, g, b, a }); }

    // --- ��� ---
    int GetType() { return type_; }
    void SetType(int type) { type_ = type; }

    // --- ImGui�p ---
    const char* const GetName() const { return model_->GetName(); }
    void SetName(const std::string& name) { model_->SetName(name + std::to_string(nameNum_++)); }

#pragma endregion [Get, Set] Function

private:
    std::unique_ptr<Model> model_;
    static int nameNum_;

    int type_; // ���
};

