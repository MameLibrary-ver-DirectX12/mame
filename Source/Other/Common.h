#pragma once

// --- �|�C���^�����֐� ---
template <class T>
inline void SafeDeletePtr(T*& p)
{
    if (nullptr == p) return;

    delete (p);
    p = nullptr;
}