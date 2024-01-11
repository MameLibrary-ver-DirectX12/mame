#pragma once
#include "Enemy.h"

class MushroomEnemy : public Enemy
{
public:
    MushroomEnemy();
    ~MushroomEnemy();

    void Initialize()                       override; // ������
    void Finalize()                         override; // �I����  
    void Update(const float& elapsedTime)   override; // �X�V
    void Render(ID3D12GraphicsCommandList* commandList,
        const DirectX::XMMATRIX& world = {})override; // �`��
    void DrawDebug()                        override; // ImGui�p
};

