#include "Model.h"

#include <filesystem>

#include "../Graphics/Graphics.h"
#include "../Other/Misc.h"

// --- �R���X�g���N�^ ---
Model::Model(const char* fbxFilename, bool triangulate, float samplingRate)
    : modelResource_(fbxFilename, triangulate, samplingRate)
{
    constantBuffers_.resize(modelResource_.GetMeshes().size());
    for (auto& c : constantBuffers_)
    {
        c = std::make_unique<ConstantBuffer>(sizeof(ModelResource::Constants));
    }
}

// --- �f�X�g���N�^ ---
Model::~Model()
{
}

// --- �`�� ---
void Model::Render(ID3D12GraphicsCommandList* commandList, const DirectX::XMMATRIX& world)
{   
    size_t mesh_index = 0;
    for (const ModelResource::Mesh& mesh : modelResource_.GetMeshes())
    {
        // --- ���_�o�b�t�@�[�ݒ� ---
        commandList->IASetVertexBuffers(0, 1, mesh.vertexBuffer_->GetVertexBufferView());
        commandList->IASetIndexBuffer(mesh.indexBuffer_->GetIndexBufferView());
        commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


        if (&keyframe_ && keyframe_.nodes_.size() > 0)
        {
            ModelResource::Animation::Keyframe::Node& meshNode = keyframe_.nodes_.at(mesh.nodeIndex_);
            DirectX::XMStoreFloat4x4(&modelResource_.GetConstants()->world_,
                DirectX::XMLoadFloat4x4(&meshNode.globalTransform_) * world);

            const size_t boneCount = mesh.bindPose_.bones_.size();
            _ASSERT_EXPR(boneCount < ModelResource::MAX_BONES, L"The value of the 'boneCount' has exceded MAX_BONES.");

            for (size_t boneIndex = 0; boneIndex < boneCount; ++boneIndex)
            {
                const ModelResource::Skeleton::Bone& bone = mesh.bindPose_.bones_.at(boneIndex);
                const ModelResource::Animation::Keyframe::Node& boneNode = keyframe_.nodes_.at(bone.nodeIndex_);
                DirectX::XMStoreFloat4x4(&modelResource_.GetConstants()->boneTransforms[boneIndex],
                    DirectX::XMLoadFloat4x4(&bone.offsetTransform) *
                    DirectX::XMLoadFloat4x4(&boneNode.globalTransform_) *
                    //DirectX::XMMatrixInverse(nullptr, DirectX::XMLoadFloat4x4(&mesh.defaultGlobalTransform_))
                    DirectX::XMMatrixInverse(nullptr, DirectX::XMLoadFloat4x4(&meshNode.globalTransform_))
                );
            }
        }
        else
        {
            DirectX::XMStoreFloat4x4(&modelResource_.GetConstants()->world_, DirectX::XMLoadFloat4x4(&mesh.defaultGlobalTransform_) * world);
            for (size_t boneIndex = 0; boneIndex < modelResource_.MAX_BONES; ++boneIndex)
            {
                modelResource_.GetConstants()->boneTransforms[boneIndex] =
                {
                    1,0,0,0,
                    0,1,0,0,
                    0,0,1,0,
                    0,0,0,1
                };
            }
        }

        constantBuffers_[mesh_index]->UpdateSubresource(modelResource_.GetConstants());

        // --- �萔�o�b�t�@�[�X�V ---
        commandList->SetGraphicsRootDescriptorTable(1, constantBuffers_[mesh_index]->GetDescriptor()->GetGpuHandle());

        // --- �T�u�Z�b�g ---
        for (const ModelResource::Mesh::Subset&  subset : mesh.subsets_)
        {
            const ModelResource::Material& material = modelResource_.GetMaterials().at(subset.materialUniqueId_);

            // --- ShaderResourceView ---
            commandList->SetGraphicsRootDescriptorTable(2, material.texture_[0]->GetDescriptor()->GetGpuHandle());
            commandList->SetGraphicsRootDescriptorTable(3, material.texture_[1]->GetDescriptor()->GetGpuHandle());

            // --- DrawCall ---
            commandList->DrawIndexedInstanced(subset.indexCount_, 1, subset.startIndexLocation_, 0, 0);
        }
        ++mesh_index;
    }
}

// --- ImGui�p ---
void Model::DrawDebug()
{
    if (ImGui::TreeNode("Model"))
    {
        transform_.DrawDebug();
        ImGui::ColorEdit4("Color", &modelResource_.GetConstants()->materialColor_.x);
        ImGui::TreePop();
    }
}

// --- �A�j���[�V�����Đ� ---
void Model::PlayAnimation(const int& index, const bool& loop, const float& speed, const float& blendSeconds)
{
    // --- �����̃A�j���[�V�����ԍ������݂̃A�j���[�V�����ԍ��Ɠ����ꍇ�� return ---
    if (index == currentAnimationIndex_) return;

    currentAnimationIndex_      = index;        // �Đ�����A�j���[�V�����ԍ���ݒ�
    currentAnimationSeconds_    = 0.0f;         // �A�j���[�V�����Đ����Ԃ����Z�b�g

    animationLoopFlag_          = loop;         // ���[�v�����邩
    animationEndFlag_           = false;        // �Đ��I���t���O�����Z�b�g

    animationSpeed_             = speed;        // �A�j���[�V�����Đ����x

    blendAnimationIndex1_       = -1;           // �u�����h��
    blendAnimationIndex2_       = -1;           // �u�����h��
    animationBlendTime_         = 0.0f;         // �u�����h��
    animationBlendSeconds_      = blendSeconds; // �u�����h��
}

// --- �A�j���[�V�����u�����h ---
void Model::PlayBlendAnimation(const int& index1, const int& index2, const bool& loop, const float& speed)
{
    // --- �����̃A�j���[�V�����ԍ������݂̃A�j���[�V�����ԍ��Ɠ����ꍇ�� return ---
    if (blendAnimationIndex1_ == index1 && blendAnimationIndex2_ == index2) return;

    currentAnimationIndex_  = -1;       // �ʏ�A�j���[�V�������g��Ȃ��̂� -1

    blendAnimationIndex1_   = index1;   // �Đ�����A�j���[�V������ݒ�
    blendAnimationIndex2_   = index2;   // �Đ�����A�j���[�V������ݒ�
    blendAnimationSeconds_  = 0.0f;     // �A�j���[�V�����Đ����Ԃ����Z�b�g

    animationLoopFlag_      = loop;     // ���[�v�����邩
    animationEndFlag_       = false;    // �Đ��I���t���O

    animationSpeed_         = speed;    // �A�j���[�V�����Đ����x

    animationBlendTime_     = 0.0f;     // �u�����h�p�^�C�}�[                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                            
}

// --- �A�j���[�V�����X�V���� ---
void Model::UpdateAnimation(const float& elapsedTime)
{
    // --- �A�j���[�V�����Đ����ĂȂ������珈�����Ȃ� ---
    if (!IsPlayAnimation()) return;

    // --- �ŏI�t���[������ ( �Đ��I���t���O�������Ă���΍Đ��I�� ) ---
    if (animationEndFlag_)
    {
        animationEndFlag_ = false;      // �I���t���O�����Z�b�g
        currentAnimationIndex_ = -1;    // �A�j���[�V�����ԍ������Z�b�g
        return;
    }

    // --- �u�����h�A�j���[�V�����Đ� ( �u�����h�A�j���[�V�����̏ꍇ�͂����ŏI�� ) ---
    if (UpdateBlendAnimation(elapsedTime)) return;

    // --- �A�j���[�V�����Đ����Ԍo�� ---
    currentAnimationSeconds_ += elapsedTime;

    // --- �w��̃A�j���[�V�����f�[�^���擾 ---
    ModelResource::Animation& animation = GetAnimation()->at(currentAnimationIndex_);

    // --- ���݂̃t���[�����擾 ---
    // �x�����ז��Ȃ̂ňꎞ�I��float�ϐ��Ɋi�[
    const float frameIndexFloat = (currentAnimationSeconds_ * animation.samplingRate_) * animationSpeed_;
    const size_t frameIndex = static_cast<size_t>(frameIndexFloat);

    // --- �O�����猻�݂̃t���[��������Ă����悤�ɂ��� ---
    currentKeyframeIndex_ = frameIndex;

    // --- �Ō�̃t���[�����擾 ---
    const size_t frameEnd = (animation.sequence_.size() - 1);

    // --- �A�j���[�V�������Đ��������Ă��ꍇ ---
    if (frameIndex > frameEnd)
    {
        if (animationLoopFlag_)
        {
            // --- ���[�v�L : �ŏ��̃t���[���ɖ߂� ---
            currentAnimationSeconds_ = 0.0f;
            return;
        }
        else
        {
            // --- ���[�v�� : �I�� ---
            animationEndFlag_ = true;
            return;
        }
    }
    else if ((keyframe_.nodes_.size() > 0) && (frameIndex < frameEnd))
    {
        // --- �u�����h���̌v�Z ---
        blendRate_ = 1.0f;
        UpdateBlendRate(blendRate_, elapsedTime);

        // --- �L�[�t���[���擾 ---
        const std::vector<ModelResource::Animation::Keyframe>& keyframe = animation.sequence_;

        // --- ���݂̑O��̃L�[�t���[�����擾 ---
        const ModelResource::Animation::Keyframe* keyframeArray[2] =
        {
            &keyframe_,
            &keyframe.at(frameIndex + 1),
        };

        // --- �A�j���[�V���������炩�ɐ؂�ւ��� ---
        BlendAnimations(keyframeArray, blendRate_, keyframe_);

        // --- �A�j���[�V�����X�V ---
        UpdateAnimation(keyframe_);
    }
    // --- �L�[�t���[������x���X�V����Ă��Ȃ��ăA�j���[�V�������Đ��������Ă��Ȃ���Ό��݂̃t���[����ۑ� ---
    else
    {
        keyframe_ = animation.sequence_.at(frameIndex);
    }
}

// --- �A�j���[�V�������Đ����Ȃ� true ---
const bool Model::IsPlayAnimation() 
{
    // --- �A�j���[�V�����Đ�����ĂȂ� ---
    if (currentAnimationIndex_ < 0 && blendAnimationIndex1_ < 0) return false;

    // --- �A�j���[�V�����ԍ������݂��Ȃ� ---
    const int animationIndexEnd = static_cast<int>(modelResource_.GetAnimationClips()->size());
    if (currentAnimationIndex_ >= animationIndexEnd && blendAnimationIndex1_ >= animationIndexEnd) return false;

    return true;
}

// JOINT_POSITION
DirectX::XMFLOAT3 Model::JointPosition(const std::string& meshName, const std::string& boneName, ModelResource::Animation::Keyframe* keyframe, const DirectX::XMFLOAT4X4& transform)
{
    DirectX::XMFLOAT3 position = {};

    for (const ModelResource::Mesh& mesh : modelResource_.GetMeshes())
    {
        if (mesh.name_ == meshName)
        {
            for (const ModelResource::Skeleton::Bone& bone : mesh.bindPose_.bones_)
            {
                if (bone.name_ == meshName)
                {
                    ModelResource::Animation::Keyframe::Node& node = keyframe->nodes_.at(bone.nodeIndex_);
                    DirectX::XMFLOAT4X4 globalTransform = node.globalTransform_;
                    position.x = globalTransform._41;
                    position.y = globalTransform._42;
                    position.z = globalTransform._43;
                    DirectX::XMMATRIX M = DirectX::XMLoadFloat4x4(&mesh.defaultGlobalTransform_);
                    DirectX::XMStoreFloat3(&position, DirectX::XMVector3Transform(DirectX::XMLoadFloat3(&position), M));
                    return position;
                }
            }
        }
    }

    _ASSERT_EXPR(FALSE, "Joint not found");

    return {};
}

// JOINT_POSITION
DirectX::XMFLOAT3 Model::JointPosition(size_t meshIndex, size_t boneIndex, const ModelResource::Animation::Keyframe* keyframe, const DirectX::XMFLOAT4X4& transform)
{
    DirectX::XMFLOAT3 position = { 0,0,0 };
    const ModelResource::Mesh& mesh = modelResource_.GetMeshes().at(meshIndex);
    const ModelResource::Skeleton::Bone& bone = mesh.bindPose_.bones_.at(boneIndex);
    const ModelResource::Animation::Keyframe::Node& boneNode = keyframe->nodes_.at(bone.nodeIndex_);
    DirectX::XMMATRIX M = DirectX::XMLoadFloat4x4(&boneNode.globalTransform_) * DirectX::XMLoadFloat4x4(&transform);
    DirectX::XMStoreFloat3(&position, DirectX::XMVector3TransformCoord(DirectX::XMLoadFloat3(&position), M));
    return position;
}

// --- �u�����h���v�Z�X�V���� ---
void Model::UpdateBlendRate(float blendRate, const float& elapsedTime)
{
    if (animationBlendTime_ < animationBlendSeconds_)
    {
        animationBlendTime_ += elapsedTime;
        if (animationBlendTime_ >= animationBlendSeconds_)
        {
            animationBlendTime_ = animationBlendSeconds_;
        }
        blendRate = animationBlendTime_ / animationBlendSeconds_;
        blendRate *= blendRate;
    }
}

// --- �u�����h�A�j���[�V�����X�V���� ( �X�V���Ă����� true ) ---
bool Model::UpdateBlendAnimation(const float& elapsedTime)
{
    // --- �A�j���[�V�����ԍ����ݒ肳��Ă��Ȃ� ( �Đ����Ȃ� ) ---
    if (blendAnimationIndex1_ < 0) return false;

    // --- �A�j���[�V�������I����Ă���̂ŏ������Ȃ� ---
    if (animationEndFlag_)
    {
        animationEndFlag_       = false;    // �I���t���O�����Z�b�g
        blendAnimationIndex1_   = -1;       // �A�j���[�V�����ԍ������Z�b�g
        blendAnimationIndex2_   = -1;       // �A�j���[�V�����ԍ������Z�b�g
        return false;
    }

    // --- weight�l�� 0 ~ 1 �̊ԂɎ��߂� ---
    if (weight_ < 0) weight_ = 0;
    else if (weight_ > 1) weight_ = 1;

    // --- �A�j���[�V�����Đ����ԍX�V ---
    blendAnimationSeconds_ += elapsedTime;

    ModelResource::Animation& bAnimation1 = GetAnimation()->at(blendAnimationIndex1_);
    ModelResource::Animation& bAnimation2 = GetAnimation()->at(blendAnimationIndex2_);

    const size_t frameCount1 = bAnimation1.sequence_.size();
    const size_t frameCount2 = bAnimation2.sequence_.size();

    // --- ��ƂȂ�A�j���[�V�����t���[���������킹�� ---
    size_t maxFrameCount = frameCount1;
    if (blendThreshold_ < weight_) maxFrameCount = frameCount2;

    const float frameIndex = (animationBlendSeconds_ * bAnimation1.samplingRate_) * animationSpeed_;

    // --- �A�j���[�V�������Đ��������Ă���ꍇ ---
    if (frameIndex > maxFrameCount)
    {
        if (animationLoopFlag_)
        {
            // --- ���[�v�L : �ŏ��̃t���[���ɖ߂� ---
            blendAnimationSeconds_ = 0.0f;
            return true;
        }
        else
        {
            // --- ���[�v�� : �I�� ---
            animationEndFlag_ = true;
            return true;
        }
    }

    // --- �Đ��t���[���𐳋K�����čĐ����Ԃ̒��������킹�� ---
    UINT64 frameIndex1 = static_cast<UINT64>(frameIndex / maxFrameCount * (frameCount1 - 1));
    UINT64 frameIndex2 = static_cast<UINT64>(frameIndex / maxFrameCount * (frameCount2 - 1));

    // --- �u�����h�����X�V ---
    blendRate_ = 1.0f;
    UpdateBlendRate(blendRate_, elapsedTime);

    // --- �L�[�t���[���擾 ---
    const std::vector<ModelResource::Animation::Keyframe>& keyframes1 = bAnimation1.sequence_;

    // --- ���݂̑O��̃L�[�t���[�����擾 ---
    const ModelResource::Animation::Keyframe* keyframeArray1[2] =
    {
        &keyframes1.at(frameIndex1),
        &keyframes1.at(frameIndex1 + 1),
    };

    // --- �A�j���[�V���������炩�ɐ؂�ւ��� ---
    ModelResource::Animation::Keyframe keyframe1;
    BlendAnimations(keyframeArray1, blendRate_, keyframe1);

    // --- �L�[�t���[���擾 ---
    const std::vector<ModelResource::Animation::Keyframe>& keyframes2 = bAnimation2.sequence_;

    // --- ���݂̑O��̃L�[�t���[�����擾 ---
    const ModelResource::Animation::Keyframe* keyframeArray2[2] =
    {
        &keyframes2.at(frameIndex2),
        &keyframes2.at(frameIndex2 + 1)
    };

    // --- �A�j���[�V���������炩�ɐ؂�ւ��� ---
    ModelResource::Animation::Keyframe keyframe2;
    BlendAnimations(keyframeArray2, blendRate_, keyframe2);

    // --- �Q�̃A�j���[�V���������������ɂ��� ---
    const ModelResource::Animation::Keyframe* resultKeyframeArray[2] = { &keyframe1, &keyframe2 };
    BlendAnimations(resultKeyframeArray, weight_, keyframe_);

    // --- �A�j���[�V�����X�V ---
    UpdateAnimation(keyframe_);

    return true;
}

// --- �A�j���[�V�����u�����h ---
void Model::BlendAnimations(const ModelResource::Animation::Keyframe* keyframes[2], float factor, ModelResource::Animation::Keyframe& keyframe)
{
    _ASSERT_EXPR(keyframes[0]->nodes_.size() == keyframes[1]->nodes_.size(), "The size of the two node array must be the same.");

    size_t nodeCount = keyframes[0]->nodes_.size();
    keyframe.nodes_.resize(nodeCount);
    for (size_t nodeIndex = 0; nodeIndex < nodeCount; ++nodeIndex)
    {
        DirectX::XMVECTOR S[2] =
        {
            DirectX::XMLoadFloat3(&keyframes[0]->nodes_.at(nodeIndex).scaling_),
            DirectX::XMLoadFloat3(&keyframes[1]->nodes_.at(nodeIndex).scaling_)
        };
        DirectX::XMStoreFloat3(&keyframe.nodes_.at(nodeIndex).scaling_, DirectX::XMVectorLerp(S[0], S[1], factor));

        DirectX::XMVECTOR R[2] =
        {
            DirectX::XMLoadFloat4(&keyframes[0]->nodes_.at(nodeIndex).rotation_),
            DirectX::XMLoadFloat4(&keyframes[1]->nodes_.at(nodeIndex).rotation_)
        };
        DirectX::XMStoreFloat4(&keyframe.nodes_.at(nodeIndex).rotation_, DirectX::XMQuaternionSlerp(R[0], R[1], factor));

        DirectX::XMVECTOR T[2] =
        {
            DirectX::XMLoadFloat3(&keyframes[0]->nodes_.at(nodeIndex).translation_),
            DirectX::XMLoadFloat3(&keyframes[1]->nodes_.at(nodeIndex).translation_)
        };
        DirectX::XMStoreFloat3(&keyframe.nodes_.at(nodeIndex).translation_, DirectX::XMVectorLerp(T[0], T[1], factor));
    }
}

// --- �A�j���[�V�����X�V ---
void Model::UpdateAnimation(ModelResource::Animation::Keyframe& keyframe)
{
    size_t nodeCount = keyframe.nodes_.size();
    for (size_t nodeIndex = 0; nodeIndex < nodeCount; ++nodeIndex)
    {
        ModelResource::Animation::Keyframe::Node& node = keyframe.nodes_.at(nodeIndex);
        DirectX::XMMATRIX S = DirectX::XMMatrixScaling(node.scaling_.x, node.scaling_.y, node.scaling_.z);
        DirectX::XMMATRIX R = DirectX::XMMatrixRotationQuaternion(DirectX::XMLoadFloat4(&node.rotation_));
        DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(node.translation_.x, node.translation_.y, node.translation_.z);
 
        int64_t parentIndex = modelResource_.GetScene()->nodes_.at(nodeIndex).parentIndex_;
        DirectX::XMMATRIX P = (parentIndex < 0) ? DirectX::XMMatrixIdentity() :
            DirectX::XMLoadFloat4x4(&keyframe.nodes_.at(parentIndex).globalTransform_);

        DirectX::XMStoreFloat4x4(&node.globalTransform_, S * R * T * P);
    }
}
