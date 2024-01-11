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

    virtual void CreateResource()                                   = 0; // ���\�[�X����
    virtual void Initialize()                                       = 0; // ������
    virtual void Finalize()                                         = 0; // �I����
    virtual void Update(const float& elapsedTime)                   = 0; // �X�V
    virtual void Render(ID3D12GraphicsCommandList* commandList)     = 0; // �`��
    virtual void UIRender(ID3D12GraphicsCommandList* commandList)   = 0; // UI�`��
    virtual void DrawDebug()                                        = 0; // ImGui�p

public:
    bool IsReady()const { return ready_; }   // �����������Ă��邩�ǂ���
    void SetReady() { ready_ = true; }       // ��������

private:
    bool ready_ = false;
};