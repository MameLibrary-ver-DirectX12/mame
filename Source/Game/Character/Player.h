#pragma once
#include "Character.h"

class Player : public Character
{
public:
    Player();
    ~Player();


    void Initialize()                                   override;   // ������
    void Finalize()                                     override;   // �I����
    void Update(const float& elapsedTime)               override;   // �X�V
    void Render(ID3D12GraphicsCommandList* commandList,
        const DirectX::XMMATRIX world = {})             override;   // �`��
    void DrawDebug()                                    override;   // ImGui�p

private:
    int animationIndex_ = 0;

    std::unique_ptr<Character> arm_; // �r

};

