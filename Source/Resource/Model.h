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
        const DirectX::XMMATRIX& world);    // �`��
    void DrawDebug();                                       // ImGui�p

    // �A�j���[�V�����Đ�
    void PlayAnimation(
        const int& index,                   // �A�j���[�V�����ԍ�
        const bool& loop,                   // ���[�v���邩
        const float& speed = 1.0f,          // �Đ����x
        const float& blendSeconds = 1.0f    // �X���[�Y�؂�ւ����� (���x)
    );

    // �A�j���[�V�����u�����h
    void PlayBlendAnimation(
        const int& index1,          // �u�����h����P�ڂ̃A�j���[�V�����ԍ�
        const int& index2,          // �u�����h����Q�ڂ̃A�j���[�V�����ԍ�
        const bool& loop,           // ���[�v���邩
        const float& speed = 1.0f   // �Đ����x
    );

    // �A�j���[�V�����X�V����
    void UpdateAnimation(const float& elapsedTime);

    // �A�j���[�V�������Đ����Ȃ� true
    const bool IsPlayAnimation();

    // JOINT_POSITION
    DirectX::XMFLOAT3 JointPosition(const std::string& meshName, const std::string& boneName, ModelResource::Animation::Keyframe* keyframe, const DirectX::XMFLOAT4X4& transform);
    DirectX::XMFLOAT3 JointPosition(size_t meshIndex, size_t boneIndex, const ModelResource::Animation::Keyframe* keyframe, const DirectX::XMFLOAT4X4& transform);

public:// --- �擾�E�ݒ� ---
    Transform* GetTransform() { return &transform_; }

private:// --- �����֐� ---
    // �u�����h���̌v�Z�X�V����
    void UpdateBlendRate(float blendRate, const float& elapsedTime);

    // �u�����h�A�j���[�V�����X�V���� ( �X�V���Ă����� true )
    bool UpdateBlendAnimation(const float& elapsedTime);

    // �A�j���[�V�����f�[�^�擾 
    std::vector<ModelResource::Animation>* GetAnimation() { return modelResource_.GetAnimationClips(); }

    // �A�j���[�V�����u�����h
    void BlendAnimations(const ModelResource::Animation::Keyframe* keyframes[2], float factor, ModelResource::Animation::Keyframe& keyframe);

    // �A�j���[�V�����X�V
    void UpdateAnimation(ModelResource::Animation::Keyframe& keyframe);


private:// --- �����o�ϐ� ---
    ModelResource modelResource_;
    std::vector<std::unique_ptr<ConstantBuffer>> constantBuffers_;

    // ---------- Transform ----------
    Transform transform_;

    // ---------- �A�j���[�V���� ----------
    int     currentAnimationIndex_      = -1;       // ���݂̃A�j���[�V�����ԍ�
    float   currentAnimationSeconds_    = 0.0f;     // ���݂̃A�j���[�V�����Đ�����
    int     currentKeyframeIndex_       = 0;        // ���݂̃A�j���[�V�����Đ��t���[��
    bool    animationLoopFlag_          = false;    // �A�j���[�V���������[�v���邩 
    bool    animationEndFlag_           = false;    // �A�j���[�V�������I��������
    float   animationBlendTime_         = 0.0f;     // ���݂̃u�����h�^�C�}�[
    float   animationBlendSeconds_      = 0.0f;     // �u�����h����
    int     blendAnimationIndex1_       = -1;       // �P�ڃu�����h�p�A�j���[�V�����ԍ�
    int     blendAnimationIndex2_       = -1;       // �Q�ڃu�����h�p�A�j���[�V�����ԍ�
    float   blendAnimationSeconds_      = 0.0f;     // ���݂̃A�j���[�V�����Đ�����
    float   blendThreshold_             = 0.0f;     // �u�����h�A�j���[�V�����̃x�[�X�ƂȂ�A�j���[�V�����̐؂�ւ�臒l
    float   blendRate_                  = 0.0f;     // �u�����h��
    float   weight_                     = 0.0f;     // �e���l
    float   animationSpeed_             = 1.0f;     // �A�j���[�V�����Đ����x
    ModelResource::Animation::Keyframe keyframe_ = {};

    // ---------- ImGui�p ----------
    std::string name_ = "";

public:// --- ImGui�p ---
    const char* const GetName() const { return name_.c_str(); }
    void SetName(const std::string& name) { name_ = name; }
};