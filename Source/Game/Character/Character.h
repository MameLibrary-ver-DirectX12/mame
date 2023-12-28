#pragma once

#include "../../Resource/Model.h"

class Character
{
public:
    Character(const char* fbxFilename, bool triangulate = false, float samplingRate = 0);
    virtual ~Character() {}

    virtual void Initialize() {}                                    // 初期化
    virtual void Finalize() {}                                      // 終了化
    virtual void Update(const float& elapsedTime) {}                // 更新
    virtual void Render(ID3D12GraphicsCommandList* commandList,
        const DirectX::XMMATRIX world);                             // 描画
    virtual void DrawDebug();                                       // ImGui用

    // --- Transform ---
    Transform* GetTransform() { return model_->GetTransform(); }
    DirectX::XMFLOAT4 GetColor() { return model_->GetColor(); }
    void SetColor(const DirectX::XMFLOAT4& color) { model_->SetColor(color); }
    void SetColor(const float& r, const float& g, const float& b, const float& a) { model_->SetColor({ r, g, b, a }); }

    // --- アニメーション再生 ---
    void PlayAnimation(
        const int& index,                   // アニメーション番号
        const bool& loop,                   // ループするか
        const float& speed = 1.0f,          // 再生速度
        const float& blendSeconds = 1.0f    // スムーズ切り替え時間 (速度)
    )
    {
        model_->PlayAnimation(index, loop, speed, blendSeconds);
    }

    // --- アニメーションブレンド ---
    void PlayBlendAnimation(
        const int& index1,          // ブレンドする１つ目のアニメーション番号
        const int& index2,          // ブレンドする２つ目のアニメーション番号
        const bool& loop,           // ループするか
        const float& speed = 1.0f   // 再生速度
    )
    {
        model_->PlayBlendAnimation(index1, index2, loop, speed);
    }

    // --- アニメーション更新 ---
    void UpdateAnimation(const float& elapsedTime) { model_->UpdateAnimation(elapsedTime); }

    // --- アニメーションが再生中なら true ---
    const bool IsPlayAnimation() { model_->IsPlayAnimation(); }


    // --- ImGui用 ---
    const char* const GetName() const { return model_->GetName(); }
    void SetName(const std::string& name) { model_->SetName(name); }

private:
    std::unique_ptr<Model> model_;
};

