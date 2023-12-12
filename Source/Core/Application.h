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

    // --- �R�s�[�Ƒ�����֎~ ---
    Application(const Application&) = delete;
    void operator=(const Application) = delete;

public:// Function
    int Run();
    LRESULT CALLBACK HandleMessage(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

    bool Initialize();                      // ������
    bool Finalize();                        // �I����
    void Update(const float& elapsedTime);  // �X�V
    void Render();                          // �`��
   
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