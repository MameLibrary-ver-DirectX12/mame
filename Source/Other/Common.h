#pragma once

// --- ƒ|ƒCƒ“ƒ^Á‹ŠÖ” ---
template <class T>
inline void SafeDeletePtr(T*& p)
{
    if (nullptr == p) return;

    delete (p);
    p = nullptr;
}