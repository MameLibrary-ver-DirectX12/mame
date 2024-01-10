#pragma once
#include "Character.h"

class Honey : public Character
{
public:
    Honey();
    ~Honey() override;

    void Initialize()                                   override;   // ������
    void Finalize()                                     override;   // �I����
    void Update(const float& elapsedTime)               override;   // �X�V
    void Render(ID3D12GraphicsCommandList* commandList,
        const DirectX::XMMATRIX& world = {})             override;  // �`��
    void DrawDebug()                                    override;   // ImGui�p

private:
    int health_ = 0;
};

