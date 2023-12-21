#include "ModelResource.h"

#include <d3dcompiler.h>
#include <filesystem>
#include <sstream>
#include <fstream>

#include "../Graphics/Graphics.h"
#include "../Other/Misc.h"


// ----- inline -----
#pragma region inline
inline DirectX::XMFLOAT4X4 ToXMFLOAT4X4(const FbxAMatrix& fbxamatrix)
{
    DirectX::XMFLOAT4X4 xmfloat4x4;
    for (int row = 0; row < 4; ++row)
    {
        for (int column = 0; column < 4; ++column)
        {
            xmfloat4x4.m[row][column] = static_cast<float>(fbxamatrix[row][column]);
        }
    }
    return xmfloat4x4;
}
inline DirectX::XMFLOAT3 ToXMFLOAT3(const FbxDouble3& fbxdouble3)
{
    DirectX::XMFLOAT3 xmfloat3;
    xmfloat3.x = static_cast<float>(fbxdouble3[0]);
    xmfloat3.y = static_cast<float>(fbxdouble3[1]);
    xmfloat3.z = static_cast<float>(fbxdouble3[2]);
    return xmfloat3;
}
inline DirectX::XMFLOAT4 ToXMFLOAT4(const FbxDouble4& fbxdouble4)
{
    DirectX::XMFLOAT4 xmfloat4;
    xmfloat4.x = static_cast<float>(fbxdouble4[0]);
    xmfloat4.y = static_cast<float>(fbxdouble4[1]);
    xmfloat4.z = static_cast<float>(fbxdouble4[2]);
    xmfloat4.w = static_cast<float>(fbxdouble4[3]);
    return xmfloat4;
}

#pragma endregion inline

// ----- コンストラクタ -----
ModelResource::ModelResource(const char* fbxFilename, bool triangulate, float samplingRate)
{
    // ----- fbx or cereal 読み込み -----
    std::filesystem::path cerealFilename(fbxFilename);
    cerealFilename.replace_extension("mame");
    if (std::filesystem::exists(cerealFilename.c_str()))
    {
        std::ifstream ifs(cerealFilename.c_str(), std::ios::binary);
        cereal::BinaryInputArchive deserialization(ifs);
        deserialization(sceneView_, meshes_, materials_, animationClips_);
    }
    else
    {
        FetchScene(fbxFilename, triangulate, samplingRate);

        std::ofstream ofs(cerealFilename.c_str(), std::ios::binary);
        cereal::BinaryOutputArchive serialization(ofs);
        serialization(sceneView_, meshes_, materials_, animationClips_);
    }

    // --- リソース生成 ---
    CreateObject(fbxFilename);

    // --- バウンディングボックス ---
    ComputeBoundingBox();
}

// ----- リソース生成 -----
void ModelResource::CreateObject(const char* fbxFilename)
{
    HRESULT result = S_OK;

    // ----- メッシュのVertex,Index情報読み取り -----
    for (ModelResource::Mesh& mesh : meshes_)
    {
        mesh.vertexBuffer_ = std::make_unique<VertexBuffer>(mesh.vertices_.data(), mesh.vertices_.size(), sizeof(Vertex));
        mesh.indexBuffer_ = std::make_unique<IndexBuffer>(mesh.indices_.data(), mesh.indices_.size(), sizeof(UINT32));

        mesh.vertices_.clear();
        mesh.indices_.clear();
    }

    // ----- テクスチャ読み込み -----
    for (std::unordered_map<uint64_t, Material>::iterator iterator = materials_.begin();
        iterator != materials_.end(); ++iterator)
    {
        // --- BaseColor ---
        if (iterator->second.textureFilenames_[0].size() > 0)
        {
            std::filesystem::path filePath(fbxFilename);
            filePath.replace_filename(iterator->second.textureFilenames_[0]);
            iterator->second.texture_[0] = std::make_unique<Texture>(filePath.c_str());
        }
        else
        {
            iterator->second.texture_[0] = std::make_unique<Texture>("dummy");
        }

        // --- Normal ---
        if (iterator->second.textureFilenames_[1].size() > 0)
        {
            std::filesystem::path filePath(fbxFilename);
            filePath.replace_filename(iterator->second.textureFilenames_[1]);
            iterator->second.texture_[1] = std::make_unique<Texture>(filePath.c_str());
        }
        else
        {
            iterator->second.texture_[1] = std::make_unique<Texture>("dummy");
        }
    }
}

// ----- FetchScene -----
void ModelResource::FetchScene(const char* fbxFilename, bool triangulate, float samplingRate)
{
    FbxManager* fbxManager = FbxManager::Create();
    FbxScene* fbxScene = FbxScene::Create(fbxManager, "");
    FbxImporter* fbxImporter = FbxImporter::Create(fbxManager, "");
    bool importStatus = false;

    importStatus = fbxImporter->Initialize(fbxFilename);
    _ASSERT_EXPR_A(importStatus, fbxImporter->GetStatus().GetErrorString());

    importStatus = fbxImporter->Import(fbxScene);
    _ASSERT_EXPR_A(importStatus, fbxImporter->GetStatus().GetErrorString());

    FbxGeometryConverter fbxConverter(fbxManager);

    // --- 三角化をする ---
    if (triangulate)
    {
        fbxConverter.Triangulate(fbxScene, true/*replace*/, false/*legacy*/);
        fbxConverter.RemoveBadPolygonsFromMeshes(fbxScene);
    }

    std::function<void(FbxNode*)>traverse
    {
        [&](FbxNode* fbxNode)
        {
            Scene::Node& node = sceneView_.nodes_.emplace_back();
            node.attribute_ = fbxNode->GetNodeAttribute() ? fbxNode->GetNodeAttribute()->GetAttributeType() : FbxNodeAttribute::EType::eUnknown;
            node.name_ = fbxNode->GetName();
            node.uniqueId_ = fbxNode->GetUniqueID();
            node.parentIndex_ = sceneView_.IndexOf(fbxNode->GetParent() ? fbxNode->GetParent()->GetUniqueID() : 0);

            // --- ノードを uniqueId をキーとしたリストに格納 ---
            nodeList_[node.uniqueId_] = fbxNode;

            for (int childIndex = 0; childIndex < fbxNode->GetChildCount(); ++childIndex)
            {
                traverse(fbxNode->GetChild(childIndex));
            }
        }
    };
    traverse(fbxScene->GetRootNode());

    FetchMeshes(fbxScene, meshes_);
    FetchMaterials(fbxScene, materials_);
    FetchAnimations(fbxScene, animationClips_, samplingRate);

    fbxManager->Destroy();
}

// ----- FetchMeshes -----
void ModelResource::FetchMeshes(FbxScene* fbxScene, std::vector<Mesh>& meshes)
{
    for (const Scene::Node& node : sceneView_.nodes_)
    {
        if (node.attribute_ != FbxNodeAttribute::EType::eMesh) continue;

        FbxNode* fbxNode = nodeList_[node.uniqueId_];
        FbxMesh* fbxMesh = fbxNode->GetMesh();

        Mesh& mesh      = meshes.emplace_back();
        mesh.uniqueId_  = fbxMesh->GetNode()->GetUniqueID();
        mesh.name_      = fbxMesh->GetNode()->GetName();
        //mesh.uniqueId_  = fbxNode->GetUniqueID();
        //mesh.name_      = fbxNode->GetName();
        mesh.nodeIndex_ = sceneView_.IndexOf(mesh.uniqueId_);

        // --- fbxScene内の 位置・姿勢・スケール 情報 ---
        mesh.defaultGlobalTransform_ = ToXMFLOAT4X4(fbxMesh->GetNode()->EvaluateGlobalTransform());

        // --- ボーン情報 ---
        std::vector<std::vector<BoneInfluence>> boneInfluences;
        FetchBoneInfluences(fbxMesh, boneInfluences);
        FetchSkeleton(fbxMesh, mesh.bindPose_);

        // --- サブセット ---
        std::vector<Mesh::Subset>& subsets = mesh.subsets_;
        const int materialCount = fbxMesh->GetNode()->GetMaterialCount();
        subsets.resize(materialCount > 0 ? materialCount : 1);
        for (int materialIndex = 0; materialIndex < materialCount; ++materialIndex)
        {
            const FbxSurfaceMaterial* fbxMaterial = fbxMesh->GetNode()->GetMaterial(materialIndex);
            subsets.at(materialIndex).materialName_ = fbxMaterial->GetName();
            subsets.at(materialIndex).materialUniqueId_ = fbxMaterial->GetUniqueID();
        }
        if (materialCount > 0)
        {
            const int polygonCount = fbxMesh->GetPolygonCount();
            for (int polygonIndex = 0; polygonIndex < polygonCount; ++polygonIndex)
            {
                const int materialIndex = fbxMesh->GetElementMaterial()->GetIndexArray().GetAt(polygonIndex);
                subsets.at(materialIndex).indexCount_ += 3;
            }

            uint32_t offset = 0;

            for (Mesh::Subset& subset : subsets)
            {
                subset.startIndexLocation_ = offset;
                offset += subset.indexCount_;
                subset.indexCount_ = 0;
            }
        }

        const int polygonCount = fbxMesh->GetPolygonCount();
        mesh.vertices_.resize(polygonCount * 3LL);
        mesh.indices_.resize(polygonCount * 3LL);

        FbxStringList uvNames;
        fbxMesh->GetUVSetNames(uvNames);
        const FbxVector4* controlPoints = fbxMesh->GetControlPoints();
        for (int polygonIndex = 0; polygonIndex < polygonCount; ++polygonIndex)
        {
            const int materialIndex = (materialCount > 0) ? fbxMesh->GetElementMaterial()->GetIndexArray().GetAt(polygonIndex) : 0;
            Mesh::Subset& subset = subsets.at(materialIndex);
            const uint32_t offset = subset.startIndexLocation_ + subset.indexCount_;

            for (int positionInPolygon = 0; positionInPolygon < 3; ++positionInPolygon)
            {
                const int vertexIndex = polygonIndex * 3 + positionInPolygon;

                Vertex vertex;
                const int polygonVertex = fbxMesh->GetPolygonVertex(polygonIndex, positionInPolygon);
                vertex.position_.x = static_cast<float>(controlPoints[polygonVertex][0]);
                vertex.position_.y = static_cast<float>(controlPoints[polygonVertex][1]);
                vertex.position_.z = static_cast<float>(controlPoints[polygonVertex][2]);

                // --- ボーン情報 ---
                const std::vector<BoneInfluence>& influencesPerControlPoint = boneInfluences.at(polygonVertex);
                for (size_t influenceIndex = 0; influenceIndex < influencesPerControlPoint.size(); ++influenceIndex)
                {
                    if (influenceIndex < MAX_BONE_INFLUENCES)
                    {
                        vertex.boneWeights_[influenceIndex] = influencesPerControlPoint.at(influenceIndex).boneWeight_;
                        vertex.boneIndices_[influenceIndex] = influencesPerControlPoint.at(influenceIndex).boneIndex_;
                    }
                    else
                    {
                        _ASSERT_EXPR(FALSE, L"BoneWeightOver");
                        // --- どうしますか？ ---
                    }
                }

                // ----- weights の合計が 1 を超えていた時のための正規化 -----
                for (auto& influence : boneInfluences)
                {
                    float t = 0;
                    for (auto& bone : influence)
                    {
                        t += bone.boneWeight_;
                    }

                    for (auto& bone : influence)
                    {
                        bone.boneWeight_ /= t;
                    }
                }

                if (fbxMesh->GetElementNormalCount() > 0)
                {
                    FbxVector4 normal;
                    fbxMesh->GetPolygonVertexNormal(polygonIndex, positionInPolygon, normal);
                    vertex.normal_.x = static_cast<float>(normal[0]);
                    vertex.normal_.y = static_cast<float>(normal[1]);
                    vertex.normal_.z = static_cast<float>(normal[2]);
                }
                if (fbxMesh->GetElementUVCount() > 0)
                {
                    FbxVector2 uv;
                    bool unmappedUv;
                    fbxMesh->GetPolygonVertexUV(polygonIndex, positionInPolygon, uvNames[0], uv, unmappedUv);
                    vertex.texcoord_.x = static_cast<float>(uv[0]);
                    vertex.texcoord_.y = 1.0f - static_cast<float>(uv[1]);
                }
                if (fbxMesh->GenerateTangentsData(0, false))
                {
                    const FbxGeometryElementTangent* tangent = fbxMesh->GetElementTangent(0);
                    vertex.tangent_.x = static_cast<float>(tangent->GetDirectArray().GetAt(vertexIndex)[0]);
                    vertex.tangent_.y = static_cast<float>(tangent->GetDirectArray().GetAt(vertexIndex)[1]);
                    vertex.tangent_.z = static_cast<float>(tangent->GetDirectArray().GetAt(vertexIndex)[2]);
                    vertex.tangent_.w = static_cast<float>(tangent->GetDirectArray().GetAt(vertexIndex)[3]);
                }

                mesh.vertices_.at(vertexIndex) = std::move(vertex);
                mesh.indices_.at(static_cast<size_t>(offset) + positionInPolygon) = vertexIndex;
                subset.indexCount_++;
            }
        }

        for (const Vertex& v : mesh.vertices_)
        {
            mesh.boundingBox_[0].x = std::min<float>(mesh.boundingBox_[0].x, v.position_.x);
            mesh.boundingBox_[0].y = std::min<float>(mesh.boundingBox_[0].y, v.position_.y);
            mesh.boundingBox_[0].z = std::min<float>(mesh.boundingBox_[0].z, v.position_.z);
            mesh.boundingBox_[1].x = std::max<float>(mesh.boundingBox_[1].x, v.position_.x);
            mesh.boundingBox_[1].y = std::max<float>(mesh.boundingBox_[1].y, v.position_.y);
            mesh.boundingBox_[1].z = std::max<float>(mesh.boundingBox_[1].z, v.position_.z);
        }
    }
}

// ----- FetchMaterials -----
void ModelResource::FetchMaterials(FbxScene* fbxScene, std::unordered_map<uint64_t, Material>& materials)
{
    const size_t nodeCount = sceneView_.nodes_.size();
    for (size_t nodeIndex = 0; nodeIndex < nodeCount; ++nodeIndex)
    {
        const Scene::Node& node = sceneView_.nodes_.at(nodeIndex);
        const FbxNode* fbxNode = fbxScene->FindNodeByName(node.name_.c_str());

        const int materialCount = fbxNode->GetMaterialCount();
        for (int materialIndex = 0; materialIndex < materialCount; ++materialIndex)
        {
            const FbxSurfaceMaterial* fbxMaterial = fbxNode->GetMaterial(materialIndex);

            Material material;
            material.name_ = fbxMaterial->GetName();
            material.uniqueId_ = fbxMaterial->GetUniqueID();
            FbxProperty fbxProperty;
            // ----- プロパティから色やテクスチャファイルパスを取得 -----

            // --- Diffuse ---
            fbxProperty = fbxMaterial->FindProperty(FbxSurfaceMaterial::sDiffuse);
            if (fbxProperty.IsValid())
            {
                const FbxDouble3 color = fbxProperty.Get<FbxDouble3>();
                material.Kd_.x = static_cast<float>(color[0]);
                material.Kd_.y = static_cast<float>(color[1]);
                material.Kd_.z = static_cast<float>(color[2]);
                material.Kd_.w = 1.0f;

                const FbxFileTexture* fbxTexture = fbxProperty.GetSrcObject<FbxFileTexture>();
                material.textureFilenames_[0] = fbxTexture ? fbxTexture->GetRelativeFileName() : "";
            }

            // --- Ambient ---
            fbxProperty = fbxMaterial->FindProperty(FbxSurfaceMaterial::sAmbient);
            if (fbxProperty.IsValid())
            {
                const FbxDouble3 color = fbxProperty.Get<FbxDouble3>();
                material.Ka_.x = static_cast<float>(color[0]);
                material.Ka_.y = static_cast<float>(color[1]);
                material.Ka_.z = static_cast<float>(color[2]);
                material.Ka_.w = 1.0f;
            }

            // --- Specular ---
            fbxProperty = fbxMaterial->FindProperty(FbxSurfaceMaterial::sSpecular);
            if (fbxProperty.IsValid())
            {
                const FbxDouble3 color = fbxProperty.Get<FbxDouble3>();
                material.Ks_.x = static_cast<float>(color[0]);
                material.Ks_.y = static_cast<float>(color[1]);
                material.Ks_.z = static_cast<float>(color[2]);
                material.Ks_.w = 1.0f;
            }

            // --- Normal ---
            fbxProperty = fbxMaterial->FindProperty(FbxSurfaceMaterial::sNormalMap);
            if (fbxProperty.IsValid())
            {
                const FbxFileTexture* fileTexture = fbxProperty.GetSrcObject<FbxFileTexture>();
                material.textureFilenames_[1] = fileTexture ? fileTexture->GetRelativeFileName() : "";
            }

            materials.emplace(material.uniqueId_, std::move(material));
        }
    }

    // --- ダミーマテリアル ---
    materials.emplace();
}

// ----- FetchSkeleton -----
void ModelResource::FetchSkeleton(FbxMesh* fbxMesh, Skeleton& bindPose)
{
    const int deformerCount = fbxMesh->GetDeformerCount(FbxDeformer::eSkin);
    for (int deformerIndex = 0; deformerIndex < deformerCount; ++deformerIndex)
    {
        FbxSkin* skin = static_cast<FbxSkin*>(fbxMesh->GetDeformer(deformerIndex, FbxDeformer::eSkin));
        const int clusterCount = skin->GetClusterCount();
        bindPose.bones_.resize(clusterCount);
        for (int clusterIndex = 0; clusterIndex < clusterCount; ++clusterIndex)
        {
            FbxCluster* cluster = skin->GetCluster(clusterIndex);

            Skeleton::Bone& bone = bindPose.bones_.at(clusterIndex);
            bone.name_ = cluster->GetLink()->GetName();
            bone.uniqueId_ = cluster->GetLink()->GetUniqueID();
            bone.parentIndex_ = bindPose.IndexOf(cluster->GetLink()->GetParent()->GetUniqueID());
            bone.nodeIndex_ = sceneView_.IndexOf(bone.uniqueId_);

            FbxAMatrix referenceGlobalInitPosition;
            cluster->GetTransformMatrix(referenceGlobalInitPosition);

            FbxAMatrix clusterGlobalInitPosition;
            cluster->GetTransformLinkMatrix(clusterGlobalInitPosition);

            bone.offsetTransform = ToXMFLOAT4X4(clusterGlobalInitPosition.Inverse() * referenceGlobalInitPosition);
        }
    }
}

// ----- FetchAnimations -----
void ModelResource::FetchAnimations(FbxScene* fbxScene, std::vector<Animation>& animationClips, float samplingRate)
{
    FbxArray<FbxString*> animationStackNames;
    fbxScene->FillAnimStackNameArray(animationStackNames);
    const int animationStackCount = animationStackNames.GetCount();
    for (int animationStackIndex = 0; animationStackIndex < animationStackCount; ++animationStackIndex)
    {
        Animation& animationClip = animationClips.emplace_back();
        animationClip.name_ = animationStackNames[animationStackIndex]->Buffer();

        FbxAnimStack* animationStack = fbxScene->FindMember<FbxAnimStack>(animationClip.name_.c_str());
        fbxScene->SetCurrentAnimationStack(animationStack);

        const FbxTime::EMode timeMode = fbxScene->GetGlobalSettings().GetTimeMode();
        FbxTime oneSecond;
        oneSecond.SetTime(0, 0, 1, 0, 0, timeMode);
        animationClip.samplingRate_ = samplingRate > 0 ? samplingRate : static_cast<float>(oneSecond.GetFrameRate(timeMode));
        const FbxTime samplingInterval = static_cast<FbxLongLong>(oneSecond.Get() / animationClip.samplingRate_);
        const FbxTakeInfo* takeInfo = fbxScene->GetTakeInfo(animationClip.name_.c_str());
        const FbxTime startTime = takeInfo->mLocalTimeSpan.GetStart();
        const FbxTime stopTime  = takeInfo->mLocalTimeSpan.GetStop();
        for (FbxTime time = startTime; time < stopTime; time += samplingInterval)
        {
            Animation::Keyframe& keyframe = animationClip.sequence_.emplace_back();

            const size_t nodeCount = sceneView_.nodes_.size();
            keyframe.nodes_.resize(nodeCount);
            for (size_t nodeIndex = 0; nodeIndex < nodeCount; ++nodeIndex)
            {
                //FbxNode* fbxNode = nodeList_[sceneView_.nodes_.at(nodeIndex).uniqueId_];
                FbxNode* fbxNode = fbxScene->FindNodeByName(sceneView_.nodes_.at(nodeIndex).name_.c_str());
                if (fbxNode)
                {
                    Animation::Keyframe::Node& node = keyframe.nodes_.at(nodeIndex);
                    node.globalTransform_ = ToXMFLOAT4X4(fbxNode->EvaluateGlobalTransform(time));

                    const FbxAMatrix& localTransform = fbxNode->EvaluateLocalTransform(time);
                    node.scaling_       = ToXMFLOAT3(localTransform.GetS());
                    node.rotation_      = ToXMFLOAT4(localTransform.GetQ());
                    node.translation_   = ToXMFLOAT3(localTransform.GetT());
                }
            }
        }
    }
    for (int animationStackIndex = 0; animationStackIndex < animationStackCount; ++animationStackIndex)
    {
        delete animationStackNames[animationStackIndex];
    }
}

// ----- FetchBoneInfluences -----
void ModelResource::FetchBoneInfluences(const FbxMesh* fbxMesh, std::vector<std::vector<BoneInfluence>>& boneInfluences)
{
    const int controlPointsCount = fbxMesh->GetControlPointsCount();
    boneInfluences.resize(controlPointsCount);

    const int skinCount = fbxMesh->GetDeformerCount(FbxDeformer::eSkin);
    for (int skinIndex = 0; skinIndex < skinCount; ++skinIndex)
    {
        const FbxSkin* fbxSkin = static_cast<FbxSkin*>(fbxMesh->GetDeformer(skinIndex, FbxDeformer::eSkin));

        const int clusterCount = fbxSkin->GetClusterCount();
        for (int clusterIndex = 0; clusterIndex < clusterCount; ++clusterIndex)
        {
            const FbxCluster* fbxCluster = fbxSkin->GetCluster(clusterIndex);

            const int controlPointIndicesCount = fbxCluster->GetControlPointIndicesCount();
            for (int controlPointIndicesIndex = 0; controlPointIndicesIndex < controlPointIndicesCount; ++controlPointIndicesIndex)
            {
                int controlPointIndex = fbxCluster->GetControlPointIndices()[controlPointIndicesIndex];
                double controlPointWeight = fbxCluster->GetControlPointWeights()[controlPointIndicesIndex];
                BoneInfluence& boneInfluence = boneInfluences.at(controlPointIndex).emplace_back();
                boneInfluence.boneIndex_    = static_cast<uint32_t>(clusterIndex);
                boneInfluence.boneWeight_   = static_cast<float>(controlPointWeight);
            }
        }

        //// ----- weights の合計が 1 を超えていた時のための正規化 -----
        //for (auto& influence : boneInfluences)
        //{
        //    float t = 0;
        //    for (auto& bone : influence)
        //    {
        //        t += bone.boneWeight_;
        //    }

        //    for (auto& bone : influence)
        //    {
        //        bone.boneWeight_ /= t;
        //    }
        //}
    }
}

// ----- バウンディングボックス -----
void ModelResource::ComputeBoundingBox()
{
    for (Mesh& mesh : meshes_)
    {
        DirectX::XMMATRIX M = DirectX::XMLoadFloat4x4(&mesh.defaultGlobalTransform_);
        DirectX::XMStoreFloat3(&mesh.boundingBox_[0], DirectX::XMVector3Transform(DirectX::XMLoadFloat3(&mesh.boundingBox_[0]), M));
        DirectX::XMStoreFloat3(&mesh.boundingBox_[1], DirectX::XMVector3Transform(DirectX::XMLoadFloat3(&mesh.boundingBox_[1]), M));

        if (mesh.boundingBox_[0].x > mesh.boundingBox_[1].x) std::swap<float>(mesh.boundingBox_[0].x, mesh.boundingBox_[1].x);
        if (mesh.boundingBox_[0].y > mesh.boundingBox_[1].y) std::swap<float>(mesh.boundingBox_[0].y, mesh.boundingBox_[1].y);
        if (mesh.boundingBox_[0].z > mesh.boundingBox_[1].z) std::swap<float>(mesh.boundingBox_[0].z, mesh.boundingBox_[1].z);

        boundingBox_[0].x = std::min<float>(mesh.boundingBox_[0].x, boundingBox_[0].x);
        boundingBox_[0].y = std::min<float>(mesh.boundingBox_[0].y, boundingBox_[0].y);
        boundingBox_[0].z = std::min<float>(mesh.boundingBox_[0].z, boundingBox_[0].z);
        boundingBox_[1].x = std::max<float>(mesh.boundingBox_[1].x, boundingBox_[1].x);
        boundingBox_[1].y = std::max<float>(mesh.boundingBox_[1].y, boundingBox_[1].y);
        boundingBox_[1].z = std::max<float>(mesh.boundingBox_[1].z, boundingBox_[1].z);
    }
}
