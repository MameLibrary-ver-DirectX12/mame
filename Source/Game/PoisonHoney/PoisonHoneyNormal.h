#pragma once
#include "PoisonHoney.h"

class PoisonHoneyNormal : public PoisonHoney
{
public:
    PoisonHoneyNormal();
    ~PoisonHoneyNormal();

    void Initialize()                       override;
    void Finalize()                         override;
    void Update(const float& elapsedTime)   override;
    void Render(ID3D12GraphicsCommandList* commandList,
        const DirectX::XMMATRIX& world)     override;
    void DrawDebug()                        override;
};

