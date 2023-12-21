#pragma once

#include <string>
#include <vector>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>

#include "../Graphics/Descriptor.h"
#include "../Graphics/VertexBuffer.h"
#include "../Graphics/IndexBuffer.h"
#include "../Graphics/ConstantBuffer.h"
#include "../Resource/Texture.h"

#include <fbxsdk.h>
#pragma comment(lib, "zlib-md.lib")
#pragma comment(lib, "libxml2-md.lib")
#pragma comment(lib, "libfbxsdk-md.lib")

// ----- cereal -----
#pragma region cereal
#include <cereal/archives/binary.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/set.hpp>
#include <cereal/types/unordered_map.hpp>

// DirectXMath構造体用のserializeテンプレート
namespace DirectX
{
    template<class T>
    void serialize(T& archive, DirectX::XMFLOAT2& v)
    {
        archive(
            cereal::make_nvp("x", v.x),
            cereal::make_nvp("y", v.y)
        );
    }

    template<class T>
    void serialize(T& archive, DirectX::XMFLOAT3& v)
    {
        archive(
            cereal::make_nvp("x", v.x),
            cereal::make_nvp("y", v.y),
            cereal::make_nvp("z", v.z)
        );
    }

    template<class T>
    void serialize(T& archive, DirectX::XMFLOAT4& v)
    {
        archive(
            cereal::make_nvp("x", v.x),
            cereal::make_nvp("y", v.y),
            cereal::make_nvp("z", v.z),
            cereal::make_nvp("w", v.w)
        );
    }

    template<class T>
    void serialize(T& archive, DirectX::XMFLOAT4X4& m)
    {
        archive(
            cereal::make_nvp("_11", m._11), cereal::make_nvp("_12", m._12),
            cereal::make_nvp("_13", m._13), cereal::make_nvp("_14", m._14),
            cereal::make_nvp("_21", m._21), cereal::make_nvp("_22", m._22),
            cereal::make_nvp("_23", m._23), cereal::make_nvp("_24", m._24),
            cereal::make_nvp("_31", m._31), cereal::make_nvp("_32", m._32),
            cereal::make_nvp("_33", m._33), cereal::make_nvp("_34", m._34),
            cereal::make_nvp("_41", m._41), cereal::make_nvp("_42", m._42),
            cereal::make_nvp("_43", m._43), cereal::make_nvp("_44", m._44)
        );
    }
}

#pragma endregion cereal

class ModelResource
{
public:
    ModelResource(const char* fbxFilename, bool triangulate = true, float samplingRate = 0);
    ~ModelResource() {}

    static const int MAX_BONE_INFLUENCES = 4;
    static const int MAX_BONES = 256;

public:// --- 構造体 ---
    struct Skeleton
    {
        struct Bone
        {
            uint64_t    uniqueId_ = 0;      // ID
            std::string name_ = "";         // 名前
            int64_t     parentIndex_ = -1;  // 親ボーンのIndex
            int64_t     nodeIndex_ = 0;     // シーンノードのIndex

            // モデル (メッシュ) 空間からボーン (ノード) シーンへの変換に使用される。
            DirectX::XMFLOAT4X4 offsetTransform
            {
                1,0,0,0,
                0,1,0,0,
                0,0,1,0,
                0,0,0,1
            };

            bool IsOrphan() const { return parentIndex_ < 0; }

            // --- Serialize ---
            template<class T>
            void serialize(T& archive)
            {
                archive(uniqueId_, name_, parentIndex_, nodeIndex_, offsetTransform);
            }
        };

        std::vector<Bone> bones_;

        int64_t IndexOf(uint64_t uniqueId) const
        {
            int64_t index = 0;

            for (const Bone& bone : bones_)
            {
                if (bone.uniqueId_ == uniqueId)
                {
                    return index;
                }
                ++index;
            }
            return -1;
        }

        // --- Serialize ---
        template<class T>
        void serialize(T& archive)
        {
            archive(bones_);
        }
    };

    struct Animation
    {
        std::string name_ = "";
        float samplingRate_ = 0;

        struct Keyframe
        {
            struct Node
            {
                // ノードのローカル空間からシーンのグローバル空間に変換するために使用される。
                DirectX::XMFLOAT4X4 globalTransform_ =
                {
                    1,0,0,0,
                    0,1,0,0,
                    0,0,1,0,
                    0,0,0,1
                };

                // ノードの変換データには、親に対する移動、回転、スケーリング ベクトルが含まれる。
                DirectX::XMFLOAT3 scaling_ = { 1, 1, 1 };
                DirectX::XMFLOAT4 rotation_ = { 0, 0, 0, 1 };
                DirectX::XMFLOAT3 translation_ = { 0, 0, 0 };

                // --- Serialize ---
                template<class T>
                void serialize(T& archive)
                {
                    archive(globalTransform_, scaling_, rotation_, translation_);
                }
            };

            std::vector<Node> nodes_;

            // --- Serialize ---
            template<class T>
            void serialize(T& archive)
            {
                archive(nodes_);
            }
        };

        std::vector<Keyframe> sequence_;

        // --- Serialize ---
        template<class T>
        void serialize(T& archive)
        {
            archive(name_, samplingRate_, sequence_);
        }
    };    

    struct Scene
    {
        struct Node
        {
            uint64_t    uniqueId_ = 0;
            std::string name_ = "";
            FbxNodeAttribute::EType attribute_ = FbxNodeAttribute::EType::eUnknown;
            int64_t     parentIndex_ = -1;

            // --- Serialize ---
            template<class T>
            void serialize(T& archive)
            {
                archive(uniqueId_, name_, attribute_, parentIndex_);
            }
        };

        std::vector<Node> nodes_;

        int64_t IndexOf(uint64_t uniqueId) const
        {
            int64_t index = 0;

            for (const Node& node : nodes_)
            {
                if (node.uniqueId_ == uniqueId)
                {
                    return index;
                }
                ++index;
            }
            return -1;
        }

        // --- Serialize ---
        template<class T>
        void serialize(T& archive)
        {
            archive(nodes_);
        }
    };

    struct Vertex
    {
        DirectX::XMFLOAT3   position_;
        DirectX::XMFLOAT3   normal_;
        DirectX::XMFLOAT4   tangent_;
        DirectX::XMFLOAT2   texcoord_;
        float               boneWeights_[MAX_BONE_INFLUENCES] = { 1, 0, 0, 0 };
        uint32_t            boneIndices_[MAX_BONE_INFLUENCES] = {};

        // --- Serialize ---
        template<class T>
        void serialize(T& archive)
        {
            archive(position_, normal_, tangent_, texcoord_, boneWeights_, boneIndices_);
        }
    };
    
    struct Constants
    {
        DirectX::XMFLOAT4X4     world_;
        DirectX::XMFLOAT4       materialColor_ = { 1 ,1, 1, 1 };
        DirectX::XMFLOAT4X4     boneTransforms[MAX_BONES]
        {
            {
                1,0,0,0,
                0,1,0,0,
                0,0,1,0,
                0,0,0,1
            }
        };

        DirectX::XMFLOAT4       emissiveColor_ = { 0.0f, 0.0f, 0.0f, 1.0f };
        float                   emissiveIntensity_ = 3.0f;
        DirectX::XMFLOAT3       dummy_ = {};
    };    

    struct Mesh
    {
        uint64_t        uniqueId_ = 0;
        std::string     name_ = "";
        int64_t         nodeIndex_ = 0; // シーンのノード配置を参照するIndex

        std::vector<Vertex>     vertices_;
        std::vector<uint32_t>   indices_;

        struct Subset
        {
            uint64_t    materialUniqueId_ = 0;
            std::string materialName_ = "";
            uint32_t    startIndexLocation_ = 0;
            uint32_t    indexCount_ = 0;

            // --- Serialize ---
            template<class T>
            void serialize(T& archive)
            {
                archive(materialUniqueId_, materialName_, startIndexLocation_, indexCount_);
            }
        };

        std::vector<Subset> subsets_;

        // シーン内の位置・姿勢・スケール情報
        DirectX::XMFLOAT4X4 defaultGlobalTransform_ =
        {
            1,0,0,0,
            0,1,0,0,
            0,0,1,0,
            0,0,0,1
        };

        Skeleton bindPose_;

        DirectX::XMFLOAT3 boundingBox_[2]
        {
            { +D3D12_FLOAT32_MAX, +D3D12_FLOAT32_MAX, +D3D12_FLOAT32_MAX },
            { -D3D12_FLOAT32_MAX, -D3D12_FLOAT32_MAX, -D3D12_FLOAT32_MAX }
        };

        // --- Serialize ---
        template<class T>
        void serialize(T& archive)
        {
            archive(uniqueId_, name_, nodeIndex_, subsets_, defaultGlobalTransform_,
                bindPose_, boundingBox_, vertices_, indices_);
        }


        std::unique_ptr<VertexBuffer>   vertexBuffer_;
        std::unique_ptr<IndexBuffer>    indexBuffer_;
        //friend class Model;
    };

    struct Material
    {
        uint64_t    uniqueId_ = 0;
        std::string name_ = "";

        DirectX::XMFLOAT4 Ka_ = { 0.2f, 0.2f, 0.2f, 1.0f };
        DirectX::XMFLOAT4 Kd_ = { 0.8f, 0.8f, 0.8f, 1.0f };
        DirectX::XMFLOAT4 Ks_ = { 1.0f, 1.0f, 1.0f, 1.0f };

        std::string textureFilenames_[4];
        std::unique_ptr<Texture> texture_[4];

        // --- Serialize ---
        template<class T>
        void serialize(T& archive)
        {
            archive(uniqueId_, name_, Ka_, Kd_, Ks_, textureFilenames_);
        }
    };

    struct BoneInfluence
    {
        uint32_t    boneIndex_;
        float       boneWeight_;
    };

public:// --- 取得・設定 ---
    std::vector<Mesh>& GetMeshes() { return meshes_; }
    std::unordered_map<uint64_t, Material>& GetMaterials() { return materials_; }
    std::vector<Animation>* GetAnimationClips() { return &animationClips_; }
    
    Constants* GetConstants() { return &constants_; }
    Scene* GetScene() { return &sceneView_; }


private:// --- 内部関数 ---
    void CreateObject(const char* fbxFilename);

    void FetchScene(const char* fbxFilename, bool triangulate, float sampringRate);
    void FetchMeshes(FbxScene* fbxScene, std::vector<Mesh>& meshes);
    void FetchMaterials(FbxScene* fbxScene, std::unordered_map<uint64_t, Material>& materials);
    void FetchSkeleton(FbxMesh* fbxMesh, Skeleton& bindPose);
    void FetchAnimations(FbxScene* fbxScene, std::vector<Animation>& animationClips, float samplingRate);
    void FetchBoneInfluences(const FbxMesh* fbxMesh, std::vector<std::vector<BoneInfluence>>& boneInfluences);

    void ComputeBoundingBox();

public:
    DirectX::XMFLOAT3 boundingBox_[2]
    {
        { +D3D12_FLOAT32_MAX, +D3D12_FLOAT32_MAX, +D3D12_FLOAT32_MAX },
        { -D3D12_FLOAT32_MAX, -D3D12_FLOAT32_MAX, -D3D12_FLOAT32_MAX }
    };

private:// --- メンバ変数 ---
    Scene                                   sceneView_;
    Constants                               constants_;    
    std::unordered_map<uint64_t, FbxNode*>  nodeList_;
    std::unordered_map<uint64_t, Material>  materials_;
    std::vector<Mesh>                       meshes_;
    std::vector<Animation>                  animationClips_;
};