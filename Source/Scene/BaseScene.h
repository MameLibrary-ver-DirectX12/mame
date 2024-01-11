#pragma once

#include <d3d12.h>

#include "../../External/ImGui/imgui.h"
#include "../../External/ImGui/imgui_impl_win32.h"
#include "../../External/ImGui/imgui_impl_dx12.h"

class BaseScene
{
public:
    BaseScene() {}
    virtual ~BaseScene() {}

    virtual void CreateResource()                                   = 0; // リソース生成
    virtual void Initialize()                                       = 0; // 初期化
    virtual void Finalize()                                         = 0; // 終了化
    virtual void Update(const float& elapsedTime)                   = 0; // 更新
    virtual void Render(ID3D12GraphicsCommandList* commandList)     = 0; // 描画
    virtual void UIRender(ID3D12GraphicsCommandList* commandList)   = 0; // UI描画
    virtual void DrawDebug()                                        = 0; // ImGui用

public:
    bool IsReady()const { return ready_; }   // 準備完了しているかどうか
    void SetReady() { ready_ = true; }       // 準備完了

private:
    bool ready_ = false;
};