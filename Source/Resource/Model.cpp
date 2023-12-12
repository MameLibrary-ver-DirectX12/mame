#include "Model.h"

#include <filesystem>

#include "../Graphics/Graphics.h"
#include "../Other/Misc.h"

// --- �R���X�g���N�^ ---
Model::Model(const char* fbxFilename, bool triangulate, float samplingRate)
    : modelResource_(fbxFilename, triangulate, samplingRate)
{
    constantBuffer_ = std::make_unique<ConstantBuffer>(sizeof(ModelResource::Constants));
}

// --- �f�X�g���N�^ ---
Model::~Model()
{
}

// --- �`�� ---
void Model::Render(ID3D12GraphicsCommandList* commandList)
{
    constantBuffer_->UpdateSubresource(modelResource_.GetConstants());
    
    for (auto& mesh : modelResource_.GetMeshes())
    {
        // --- �萔�o�b�t�@�[�X�V ---
        commandList->SetGraphicsRootDescriptorTable(1, constantBuffer_->GetDescriptor()->GetGpuHandle());

        // --- ���_�o�b�t�@�[�ݒ� ---
        commandList->IASetVertexBuffers(0, 1, mesh.vertexBuffer_->GetVertexBufferView());
        commandList->IASetIndexBuffer(mesh.indexBuffer_->GetIndexBufferView());
        commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

        DirectX::XMStoreFloat4x4(&modelResource_.GetConstants()->world_, DirectX::XMLoadFloat4x4(&mesh.defaultGlobalTransform_) * transform_.CalcWorldMatrix(0.1f));
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
    }
}

void Model::DrawDebug()
{
    ImGui::Begin("Model");
    transform_.DrawDebug();
    ImGui::End();
}
