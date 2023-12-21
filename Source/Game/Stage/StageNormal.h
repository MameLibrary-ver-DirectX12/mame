#pragma once
#include "Stage.h"

class StageNormal : public Stage
{
public:
    StageNormal(const char* fbxFilename);
    ~StageNormal() override {}

    void Initialize()                                   override;
    void Finalize()                                     override;
    void Update(const float& elapsedTime)               override;
    void Render(ID3D12GraphicsCommandList* commandList,
        DirectX::XMMATRIX world)                        override;
    void DrawDebug()                                    override;

private:
    static int imguiNum_;

};

