#pragma once

#include "../../Resource/Model.h"

class Character
{
public:
    Character(const char* fbxFilename, bool triangulate = false, float samplingRate = 0);
    virtual ~Character() {}

    virtual void Initialize() {}                                    // ������
    virtual void Finalize() {}                                      // �I����
    virtual void Update(const float& elapsedTime) {}                // �X�V
    virtual void Render(ID3D12GraphicsCommandList* commandList,
        const DirectX::XMMATRIX world);                             // �`��
    virtual void DrawDebug();                                       // ImGui�p

    // --- Transform ---
    Transform* GetTransform() { return model_->GetTransform(); }
    DirectX::XMFLOAT4 GetColor() { return model_->GetColor(); }
    void SetColor(const DirectX::XMFLOAT4& color) { model_->SetColor(color); }
    void SetColor(const float& r, const float& g, const float& b, const float& a) { model_->SetColor({ r, g, b, a }); }

    // --- �A�j���[�V�����Đ� ---
    void PlayAnimation(
        const int& index,                   // �A�j���[�V�����ԍ�
        const bool& loop,                   // ���[�v���邩
        const float& speed = 1.0f,          // �Đ����x
        const float& blendSeconds = 1.0f    // �X���[�Y�؂�ւ����� (���x)
    )
    {
        model_->PlayAnimation(index, loop, speed, blendSeconds);
    }

    // --- �A�j���[�V�����u�����h ---
    void PlayBlendAnimation(
        const int& index1,          // �u�����h����P�ڂ̃A�j���[�V�����ԍ�
        const int& index2,          // �u�����h����Q�ڂ̃A�j���[�V�����ԍ�
        const bool& loop,           // ���[�v���邩
        const float& speed = 1.0f   // �Đ����x
    )
    {
        model_->PlayBlendAnimation(index1, index2, loop, speed);
    }

    // --- �A�j���[�V�����X�V ---
    void UpdateAnimation(const float& elapsedTime) { model_->UpdateAnimation(elapsedTime); }

    // --- �A�j���[�V�������Đ����Ȃ� true ---
    const bool IsPlayAnimation() { model_->IsPlayAnimation(); }


    // --- ImGui�p ---
    const char* const GetName() const { return model_->GetName(); }
    void SetName(const std::string& name) { model_->SetName(name); }

private:
    std::unique_ptr<Model> model_;
};

