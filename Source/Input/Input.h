#pragma once

#include "../Input/GamePad.h"
#include "../Input/Mouse.h"

class Input
{
public:
    Input(HWND hwnd);
    ~Input() {}

public:
    // --- �C���X�^���X�擾 ---
    static Input& Instance() { return *instance_; }

    void Update();  // �X�V

    GamePad& GetGamePad() { return gamePad_; }
    Mouse& GetMouse() { return mouse_; }

private:
    static Input* instance_;
    GamePad gamePad_;
    Mouse mouse_;
};

