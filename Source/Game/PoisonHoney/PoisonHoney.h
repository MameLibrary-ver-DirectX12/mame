#pragma once
#include "../../Resource/Model.h"
#include <string>

class PoisonHoney
{
public:
    PoisonHoney(const char* fbxFilename, bool triangulate = false, float samplingRate = 0);
    virtual ~PoisonHoney();

    virtual void Initialize() {}                                    // 初期化
    virtual void Finalize() {}                                      // 終了化
    virtual void Update(const float& elapsedTime) {}                // 更新
    virtual void Render(ID3D12GraphicsCommandList* commandList,
        const DirectX::XMMATRIX& world = {});                       // 描画
    virtual void DrawDebug();                                       // ImGui用

public:// --- 取得・設定 ---
#pragma region [Get, Set] Function
    // --- Transform ---
    Transform* GetTransform() { return model_->GetTransform(); }
    DirectX::XMFLOAT4 GetColor() { return model_->GetColor(); }
    void SetColor(const DirectX::XMFLOAT4& color) { model_->SetColor(color); }
    void SetColor(const float& r, const float& g, const float& b, const float& a) { model_->SetColor({ r, g, b, a }); }

    // --- 種類 ---
    int GetType() { return type_; }
    void SetType(int type) { type_ = type; }

    // --- ImGui用 ---
    const char* const GetName() const { return model_->GetName(); }
    void SetName(const std::string& name) { model_->SetName(name + std::to_string(nameNum_++)); }

#pragma endregion [Get, Set] Function

private:
    std::unique_ptr<Model> model_;
    static int nameNum_;

    int type_; // 種類
};

