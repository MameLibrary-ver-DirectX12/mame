#include "Sprite.h"

Sprite::Sprite()
{
    HRESULT result = { S_OK };

    // --- 頂点バッファー生成 ---
    auto heapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
    auto resDesc = CD3DX12_RESOURCE_DESC::Buffer(sizeof(vertices_));

    
}

Sprite::~Sprite()
{
}

void Sprite::Initialize()
{
}

void Sprite::Finalize()
{
}

void Sprite::Update()
{
}

void Sprite::Render()
{
}
