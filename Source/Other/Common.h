#pragma once

// --- ポインタ消去関数 ---
template <class T>
inline void SafeDeletePtr(T*& p)
{
    if (nullptr == p) return;

    delete (p);
    p = nullptr;
}