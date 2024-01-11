#include "Application.h"

#include <memory>
#include <sstream>

#include "../../External/ImGui/imgui.h"
#include "../../External/ImGui/imgui_internal.h"
#include "../../External/ImGui/imgui.h"
#include "../../External/ImGui/imgui_impl_win32.h"
#include "../../External/ImGui/imgui_impl_dx12.h"

#include "../Scene/SceneManager.h"
#include "../Scene/TitleScene.h"
#include "../Scene/GameScene.h"
#include "../Scene/ShopScene.h"

// --- コンストラクタ ---
Application::Application(HWND hwnd)
    : hwnd_(hwnd),
    input_(hwnd),
    graphics_(hwnd, 2)
{
    frameBuffer_ = std::make_unique<FrameBuffer>();
    postProcess_ = std::make_unique<PostProcess>();
}

// --- デストラクタ ---
Application::~Application()
{
    graphics_.WaitIdle();
}

// --- 初期化 ---
bool Application::Initialize()
{
    //SceneManager::Instance().ChangeScene(new TitleScene);
    SceneManager::Instance().ChangeScene(new GameScene);
    //SceneManager::Instance().ChangeScene(new ShopScene);

    return true;
}

// --- 終了化 ---
bool Application::Finalize()
{
    SceneManager::Instance().Clear();

    return true;
}

// --- 更新 ---
void Application::Update(const float& elapsedTime)
{
    // --- ImGuiフレーム開始処理 ---
    Graphics::Instance().GetImGuiRenderer()->NewFrame();

    // --- 入力更新処理 ---
    Input::Instance().Update();

    SceneManager::Instance().Update(elapsedTime);


}

// --- 描画 ---
void Application::Render()
{
    ID3D12GraphicsCommandList* commandList = Graphics::Instance().Begin();
    
    frameBuffer_->Activate(commandList);
    SceneManager::Instance().Render(commandList);
    frameBuffer_->Deactivate(commandList);
    
    Graphics::Instance().SetRenderTarget();
    postProcess_->Draw(commandList, frameBuffer_->GetGpuHandle());
    SceneManager::Instance().UIRender(commandList);

    Graphics::Instance().GetImGuiRenderer()->Render(commandList);
    Graphics::Instance().End();
    Graphics::Instance().Execute();
    // ※ ----- ※ これより下に何も書かない事　※ ----- ※
}

// --- アプリケーションループ ---
int Application::Run()
{
    MSG msg{};

    if (!Initialize())
    {
        return 0;
    }
    while(!requestFinalize_)
    {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
        {
            tictoc_.Tick();
            CalculateFrameStats();
            Update(tictoc_.TimeInterval());
            Render();
        }
    }
    
    graphics_.WaitIdle();
    PostMessage(hwnd_, WM_CLOSE, 0, 0);

    return Finalize() ? static_cast<int>(msg.wParam) : 0;
}

void Application::CalculateFrameStats()
{
    if (++frames_, (tictoc_.TimeStamp() - elapsedTime_) >= 1.0f)
    {
        float fps = static_cast<float>(frames_);
        std::wostringstream outs;
        outs.precision(6);

#ifdef _DEBUG
        // FPS
        outs << L"FPS :" << fps << L" / " << L"Frame Time : " << 1000.0f / fps << L" (ms)";
#else
        // ApplicationName
        outs << APPLICATION_NAME;
#endif// _DEBUG

        SetWindowTextW(hwnd_, outs.str().c_str());

        frames_ = 0;
        elapsedTime_ += 1.0f;
    }
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

// --- メッセージハンドラー ---
LRESULT Application::HandleMessage(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam);

    switch (msg)
    {
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        BeginPaint(hwnd, &ps);

        EndPaint(hwnd, &ps);
    }
    break;

    case WM_DESTROY:
        //PostQuitMessage(0);
        requestFinalize_ = true;
        break;

    case WM_CREATE:
        break;

    case WM_KEYDOWN:
        if (wparam == VK_ESCAPE)
        {
            // ESC で落とす
            //PostMessage(hwnd, WM_CLOSE, 0, 0);
            requestFinalize_ = true;
        }
        break;

    case WM_ENTERSIZEMOVE:
        tictoc_.Stop();
        break;

    case WM_EXITSIZEMOVE:
        tictoc_.Start();
        break;

    case WM_SIZE:
    {
        RECT clientRect{};
        GetClientRect(hwnd, &clientRect);
    }
    break;

    case WM_MOUSEWHEEL:
        Input::Instance().GetMouse().SetWheel(GET_WHEEL_DELTA_WPARAM(wparam));
        break;

    default:
        return DefWindowProc(hwnd, msg, wparam, lparam);
    }

    return 0;
}