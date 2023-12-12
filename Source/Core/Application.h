#pragma once
#include <Windows.h>

#include "HighResolutionTimer.h"

#include "../Input/Input.h"
#include "../Graphics/Graphics.h"

CONST LONG SCREEN_WIDTH{ 1280 };
CONST LONG SCREEN_HEIGHT{ 720 };
CONST BOOL FULLSCREEN{ FALSE };
CONST LPCWSTR APPLICATION_NAME{ L"Mame" };

class Application
{
public:
    Application(HWND hwnd);
    ~Application();

    // --- コピーと代入を禁止 ---
    Application(const Application&) = delete;
    void operator=(const Application) = delete;

public:// Function
    int Run();
    LRESULT CALLBACK HandleMessage(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

    bool Initialize();                      // 初期化
    bool Finalize();                        // 終了化
    void Update(const float& elapsedTime);  // 更新
    void Render();                          // 描画
   
    bool requestFinalize_ = false;

private:
    void CalculateFrameStats();

private:
    CONST HWND hwnd_;

    HighResolutionTimer tictoc_;
    uint32_t            frames_ = 0;
    float               elapsedTime_ = 0.0f;

    Graphics            graphics_;
    Input               input_;
};