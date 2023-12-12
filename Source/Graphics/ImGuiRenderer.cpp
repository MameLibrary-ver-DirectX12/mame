#include "ImGuiRenderer.h"
#include "Graphics.h"

#include <stdio.h>
#include <memory>
#include <d3dcompiler.h>

#include "../Other/Misc.h"
#include "../../External/ImGui/imgui.h"
#include "../../External/ImGui/imgui_internal.h"
#include "../../External/ImGui/imgui_impl_dx12.h"
#include "../../External/ImGui/imgui_impl_win32.h"

#include "ImGuiJaGryphRanges.h"

// コンストラクタ
ImGuiRenderer::ImGuiRenderer(HWND hwnd)
{
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();

	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking

	//io.Fonts->AddFontFromFileTTF("./Resources/Font/ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
	io.Fonts->AddFontFromFileTTF("C:/Windows/Fonts/meiryo.ttc", 18.0f, NULL, glyphRangesJapanese);

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

	ImGui_ImplWin32_Init(hwnd);


	Graphics& graphics = Graphics::Instance();
	auto heap = graphics.GetSrvCbvUavDescriptorHeap()->GetD3DDescriptorHeap();
	ImGui_ImplDX12_Init(graphics.GetDevice(), graphics.GetBufferCount(),
		DXGI_FORMAT_R8G8B8A8_UNORM, heap,
		heap->GetCPUDescriptorHandleForHeapStart(),
		heap->GetGPUDescriptorHandleForHeapStart());
}

// デストラクタ
ImGuiRenderer::~ImGuiRenderer()
{
	ImGui_ImplDX12_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

// フレーム開始処理
void ImGuiRenderer::NewFrame()
{
	ImGui_ImplDX12_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

// 描画
void ImGuiRenderer::Render(ID3D12GraphicsCommandList* commandList)
{
	ImGui::Render();
	ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), commandList);
}