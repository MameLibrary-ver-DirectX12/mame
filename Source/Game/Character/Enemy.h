#pragma once
#include "Character.h"

class Enemy : public Character
{
public:
    Enemy(const char* fbxFilename, bool triangulate = true, float samplingRate = 0);
    ~Enemy() {}

    void Initialize()                       override {} // ������ 
    void Finalize()                         override {} // �I����
    void Update(const float& elapsedTime)   override {} // �X�V
    void Render(ID3D12GraphicsCommandList* commandList,
        const DirectX::XMMATRIX& world = {}) override;   // �`��

    void DrawDebug()                        override {} // ImGui�p
};

