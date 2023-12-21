#include "Model.h"

#include <filesystem>

#include "../Graphics/Graphics.h"
#include "../Other/Misc.h"

// --- コンストラクタ ---
Model::Model(const char* fbxFilename, bool triangulate, float samplingRate)
    : modelResource_(fbxFilename, triangulate, samplingRate)
{
    constantBuffers_.resize(modelResource_.GetMeshes().size());
    for (auto& c : constantBuffers_)
    {
        c = std::make_unique<ConstantBuffer>(sizeof(ModelResource::Constants));
    }
}

// --- デストラクタ ---
Model::~Model()
{
}

// --- 描画 ---
void Model::Render(ID3D12GraphicsCommandList* commandList, const DirectX::XMMATRIX& world)
{   
    size_t mesh_index = 0;
    for (const ModelResource::Mesh& mesh : modelResource_.GetMeshes())
    {
        // --- 頂点バッファー設定 ---
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

        // --- 定数バッファー更新 ---
        commandList->SetGraphicsRootDescriptorTable(1, constantBuffers_[mesh_index]->GetDescriptor()->GetGpuHandle());

        // --- サブセット ---
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

// --- ImGui用 ---
void Model::DrawDebug()
{
    if (ImGui::TreeNode("Model"))
    {
        transform_.DrawDebug();
        ImGui::ColorEdit4("Color", &modelResource_.GetConstants()->materialColor_.x);
        ImGui::TreePop();
    }
}

// --- アニメーション再生 ---
void Model::PlayAnimation(const int& index, const bool& loop, const float& speed, const float& blendSeconds)
{
    // --- 引数のアニメーション番号が現在のアニメーション番号と同じ場合は return ---
    if (index == currentAnimationIndex_) return;

    currentAnimationIndex_      = index;        // 再生するアニメーション番号を設定
    currentAnimationSeconds_    = 0.0f;         // アニメーション再生時間をリセット

    animationLoopFlag_          = loop;         // ループさせるか
    animationEndFlag_           = false;        // 再生終了フラグをリセット

    animationSpeed_             = speed;        // アニメーション再生速度

    blendAnimationIndex1_       = -1;           // ブレンド無
    blendAnimationIndex2_       = -1;           // ブレンド無
    animationBlendTime_         = 0.0f;         // ブレンド無
    animationBlendSeconds_      = blendSeconds; // ブレンド無
}

// --- アニメーションブレンド ---
void Model::PlayBlendAnimation(const int& index1, const int& index2, const bool& loop, const float& speed)
{
    // --- 引数のアニメーション番号が現在のアニメーション番号と同じ場合は return ---
    if (blendAnimationIndex1_ == index1 && blendAnimationIndex2_ == index2) return;

    currentAnimationIndex_  = -1;       // 通常アニメーションを使わないので -1

    blendAnimationIndex1_   = index1;   // 再生するアニメーションを設定
    blendAnimationIndex2_   = index2;   // 再生するアニメーションを設定
    blendAnimationSeconds_  = 0.0f;     // アニメーション再生時間をリセット

    animationLoopFlag_      = loop;     // ループさせるか
    animationEndFlag_       = false;    // 再生終了フラグ

    animationSpeed_         = speed;    // アニメーション再生速度

    animationBlendTime_     = 0.0f;     // ブレンド用タイマー                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                            
}

// --- アニメーション更新処理 ---
void Model::UpdateAnimation(const float& elapsedTime)
{
    // --- アニメーション再生してなかったら処理しない ---
    if (!IsPlayAnimation()) return;

    // --- 最終フレーム処理 ( 再生終了フラグがたっていれば再生終了 ) ---
    if (animationEndFlag_)
    {
        animationEndFlag_ = false;      // 終了フラグをリセット
        currentAnimationIndex_ = -1;    // アニメーション番号をリセット
        return;
    }

    // --- ブレンドアニメーション再生 ( ブレンドアニメーションの場合はここで終了 ) ---
    if (UpdateBlendAnimation(elapsedTime)) return;

    // --- アニメーション再生時間経過 ---
    currentAnimationSeconds_ += elapsedTime;

    // --- 指定のアニメーションデータを取得 ---
    ModelResource::Animation& animation = GetAnimation()->at(currentAnimationIndex_);

    // --- 現在のフレームを取得 ---
    // 警告が邪魔なので一時的にfloat変数に格納
    const float frameIndexFloat = (currentAnimationSeconds_ * animation.samplingRate_) * animationSpeed_;
    const size_t frameIndex = static_cast<size_t>(frameIndexFloat);

    // --- 外部から現在のフレームを取ってこれるようにする ---
    currentKeyframeIndex_ = frameIndex;

    // --- 最後のフレームを取得 ---
    const size_t frameEnd = (animation.sequence_.size() - 1);

    // --- アニメーションが再生しきってた場合 ---
    if (frameIndex > frameEnd)
    {
        if (animationLoopFlag_)
        {
            // --- ループ有 : 最初のフレームに戻る ---
            currentAnimationSeconds_ = 0.0f;
            return;
        }
        else
        {
            // --- ループ無 : 終了 ---
            animationEndFlag_ = true;
            return;
        }
    }
    else if ((keyframe_.nodes_.size() > 0) && (frameIndex < frameEnd))
    {
        // --- ブレンド率の計算 ---
        blendRate_ = 1.0f;
        UpdateBlendRate(blendRate_, elapsedTime);

        // --- キーフレーム取得 ---
        const std::vector<ModelResource::Animation::Keyframe>& keyframe = animation.sequence_;

        // --- 現在の前後のキーフレームを取得 ---
        const ModelResource::Animation::Keyframe* keyframeArray[2] =
        {
            &keyframe_,
            &keyframe.at(frameIndex + 1),
        };

        // --- アニメーションを滑らかに切り替える ---
        BlendAnimations(keyframeArray, blendRate_, keyframe_);

        // --- アニメーション更新 ---
        UpdateAnimation(keyframe_);
    }
    // --- キーフレームが一度も更新されていなくてアニメーションが再生しきっていなければ現在のフレームを保存 ---
    else
    {
        keyframe_ = animation.sequence_.at(frameIndex);
    }
}

// --- アニメーションが再生中なら true ---
const bool Model::IsPlayAnimation() 
{
    // --- アニメーション再生されてない ---
    if (currentAnimationIndex_ < 0 && blendAnimationIndex1_ < 0) return false;

    // --- アニメーション番号が存在しない ---
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

// --- ブレンド率計算更新処理 ---
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

// --- ブレンドアニメーション更新処理 ( 更新していたら true ) ---
bool Model::UpdateBlendAnimation(const float& elapsedTime)
{
    // --- アニメーション番号が設定されていない ( 再生しない ) ---
    if (blendAnimationIndex1_ < 0) return false;

    // --- アニメーションが終わっているので処理しない ---
    if (animationEndFlag_)
    {
        animationEndFlag_       = false;    // 終了フラグをリセット
        blendAnimationIndex1_   = -1;       // アニメーション番号をリセット
        blendAnimationIndex2_   = -1;       // アニメーション番号をリセット
        return false;
    }

    // --- weight値を 0 ~ 1 の間に収める ---
    if (weight_ < 0) weight_ = 0;
    else if (weight_ > 1) weight_ = 1;

    // --- アニメーション再生時間更新 ---
    blendAnimationSeconds_ += elapsedTime;

    ModelResource::Animation& bAnimation1 = GetAnimation()->at(blendAnimationIndex1_);
    ModelResource::Animation& bAnimation2 = GetAnimation()->at(blendAnimationIndex2_);

    const size_t frameCount1 = bAnimation1.sequence_.size();
    const size_t frameCount2 = bAnimation2.sequence_.size();

    // --- 基準となるアニメーションフレーム数を合わせる ---
    size_t maxFrameCount = frameCount1;
    if (blendThreshold_ < weight_) maxFrameCount = frameCount2;

    const float frameIndex = (animationBlendSeconds_ * bAnimation1.samplingRate_) * animationSpeed_;

    // --- アニメーションが再生しきっている場合 ---
    if (frameIndex > maxFrameCount)
    {
        if (animationLoopFlag_)
        {
            // --- ループ有 : 最初のフレームに戻る ---
            blendAnimationSeconds_ = 0.0f;
            return true;
        }
        else
        {
            // --- ループ無 : 終了 ---
            animationEndFlag_ = true;
            return true;
        }
    }

    // --- 再生フレームを正規化して再生時間の長さを合わせる ---
    UINT64 frameIndex1 = static_cast<UINT64>(frameIndex / maxFrameCount * (frameCount1 - 1));
    UINT64 frameIndex2 = static_cast<UINT64>(frameIndex / maxFrameCount * (frameCount2 - 1));

    // --- ブレンド率を更新 ---
    blendRate_ = 1.0f;
    UpdateBlendRate(blendRate_, elapsedTime);

    // --- キーフレーム取得 ---
    const std::vector<ModelResource::Animation::Keyframe>& keyframes1 = bAnimation1.sequence_;

    // --- 現在の前後のキーフレームを取得 ---
    const ModelResource::Animation::Keyframe* keyframeArray1[2] =
    {
        &keyframes1.at(frameIndex1),
        &keyframes1.at(frameIndex1 + 1),
    };

    // --- アニメーションを滑らかに切り替える ---
    ModelResource::Animation::Keyframe keyframe1;
    BlendAnimations(keyframeArray1, blendRate_, keyframe1);

    // --- キーフレーム取得 ---
    const std::vector<ModelResource::Animation::Keyframe>& keyframes2 = bAnimation2.sequence_;

    // --- 現在の前後のキーフレームを取得 ---
    const ModelResource::Animation::Keyframe* keyframeArray2[2] =
    {
        &keyframes2.at(frameIndex2),
        &keyframes2.at(frameIndex2 + 1)
    };

    // --- アニメーションを滑らかに切り替える ---
    ModelResource::Animation::Keyframe keyframe2;
    BlendAnimations(keyframeArray2, blendRate_, keyframe2);

    // --- ２つのアニメーションをいい感じにする ---
    const ModelResource::Animation::Keyframe* resultKeyframeArray[2] = { &keyframe1, &keyframe2 };
    BlendAnimations(resultKeyframeArray, weight_, keyframe_);

    // --- アニメーション更新 ---
    UpdateAnimation(keyframe_);

    return true;
}

// --- アニメーションブレンド ---
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

// --- アニメーション更新 ---
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
