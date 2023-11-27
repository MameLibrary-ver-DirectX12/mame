#pragma once
#include <Windows.h>
#include <memory>

class Application
{
private:
    HWND hwnd_;
    WNDCLASSEX wndClass_ = {};

public:
    void run();

private:
    void Initialize();
    void Finalize();
    void Update();
    void Render();
};

