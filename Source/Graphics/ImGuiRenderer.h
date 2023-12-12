#pragma once

#include <vector>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include "../../External/ImGui/imgui.h"

#include "Descriptor.h"

class ImGuiRenderer
{
public:
    ImGuiRenderer(HWND hwnd);
    ~ImGuiRenderer();

    void NewFrame();                                        // フレーム開始処理
    void Render(ID3D12GraphicsCommandList* commandList);    // 描画実行
};

