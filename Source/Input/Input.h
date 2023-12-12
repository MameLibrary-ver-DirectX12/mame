#pragma once

#include "../Input/GamePad.h"
#include "../Input/Mouse.h"

class Input
{
public:
    Input(HWND hwnd);
    ~Input() {}

public:
    // --- インスタンス取得 ---
    static Input& Instance() { return *instance_; }

    void Update();  // 更新

    GamePad& GetGamePad() { return gamePad_; }
    Mouse& GetMouse() { return mouse_; }

private:
    static Input* instance_;
    GamePad gamePad_;
    Mouse mouse_;
};

