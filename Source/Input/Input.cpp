#include "Input.h"

Input* Input::instance_ = nullptr;

// ----- �R���X�g���N�^ -----
Input::Input(HWND hwnd)
    : mouse_(hwnd)
{
    instance_ = this;
}

// ----- �X�V -----
void Input::Update()
{
    gamePad_.Update();
    mouse_.Update();
}