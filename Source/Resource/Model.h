#pragma once

#include <vector>

#include "ModelResource.h"
#include "../Graphics/ConstantBuffer.h"
#include "Transform.h"

class Model
{
public:
    Model(const char* fbxFilename, bool triangulate = true, float samplingRate = 0);
    ~Model();

    void Render(ID3D12GraphicsCommandList* commandList,
        const DirectX::XMMATRIX& world);    // 描画
    void DrawDebug();                                       // ImGui用

    // アニメーション再生
    void PlayAnimation(
        const int& index,                   // アニメーション番号
        const bool& loop,                   // ループするか
        const float& speed = 1.0f,          // 再生速度
        const float& blendSeconds = 1.0f    // スムーズ切り替え時間 (速度)
    );

    // アニメーションブレンド
    void PlayBlendAnimation(
        const int& index1,          // ブレンドする１つ目のアニメーション番号
        const int& index2,          // ブレンドする２つ目のアニメーション番号
        const bool& loop,           // ループするか
        const float& speed = 1.0f   // 再生速度
    );

    // アニメーション更新処理
    void UpdateAnimation(const float& elapsedTime);

    // アニメーションが再生中なら true
    const bool IsPlayAnimation();

    // JOINT_POSITION
    DirectX::XMFLOAT3 JointPosition(const std::string& meshName, const std::string& boneName, ModelResource::Animation::Keyframe* keyframe, const DirectX::XMFLOAT4X4& transform);
    DirectX::XMFLOAT3 JointPosition(size_t meshIndex, size_t boneIndex, const ModelResource::Animation::Keyframe* keyframe, const DirectX::XMFLOAT4X4& transform);

public:// --- 取得・設定 ---
    Transform* GetTransform() { return &transform_; }

private:// --- 内部関数 ---
    // ブレンド率の計算更新処理
    void UpdateBlendRate(float blendRate, const float& elapsedTime);

    // ブレンドアニメーション更新処理 ( 更新していたら true )
    bool UpdateBlendAnimation(const float& elapsedTime);

    // アニメーションデータ取得 
    std::vector<ModelResource::Animation>* GetAnimation() { return modelResource_.GetAnimationClips(); }

    // アニメーションブレンド
    void BlendAnimations(const ModelResource::Animation::Keyframe* keyframes[2], float factor, ModelResource::Animation::Keyframe& keyframe);

    // アニメーション更新
    void UpdateAnimation(ModelResource::Animation::Keyframe& keyframe);


private:// --- メンバ変数 ---
    ModelResource modelResource_;
    std::vector<std::unique_ptr<ConstantBuffer>> constantBuffers_;

    // ---------- Transform ----------
    Transform transform_;

    // ---------- アニメーション ----------
    int     currentAnimationIndex_      = -1;       // 現在のアニメーション番号
    float   currentAnimationSeconds_    = 0.0f;     // 現在のアニメーション再生時間
    int     currentKeyframeIndex_       = 0;        // 現在のアニメーション再生フレーム
    bool    animationLoopFlag_          = false;    // アニメーションをループするか 
    bool    animationEndFlag_           = false;    // アニメーションが終了したか
    float   animationBlendTime_         = 0.0f;     // 現在のブレンドタイマー
    float   animationBlendSeconds_      = 0.0f;     // ブレンド時間
    int     blendAnimationIndex1_       = -1;       // １つ目ブレンド用アニメーション番号
    int     blendAnimationIndex2_       = -1;       // ２つ目ブレンド用アニメーション番号
    float   blendAnimationSeconds_      = 0.0f;     // 現在のアニメーション再生時間
    float   blendThreshold_             = 0.0f;     // ブレンドアニメーションのベースとなるアニメーションの切り替え閾値
    float   blendRate_                  = 0.0f;     // ブレンド率
    float   weight_                     = 0.0f;     // 影響値
    float   animationSpeed_             = 1.0f;     // アニメーション再生速度
    ModelResource::Animation::Keyframe keyframe_ = {};

    // ---------- ImGui用 ----------
    std::string name_ = "";

public:// --- ImGui用 ---
    const char* const GetName() const { return name_.c_str(); }
    void SetName(const std::string& name) { name_ = name; }
};