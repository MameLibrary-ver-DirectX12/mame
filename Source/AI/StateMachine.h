#pragma once

#include <vector>
#include "State.h"

#include "../../External/ImGui/imgui.h"
#include "../../External/ImGui/imgui_impl_win32.h"
#include "../../External/ImGui/imgui_impl_dx12.h"

template <class T>
class StateMachine
{
public:
    StateMachine() {};
    ~StateMachine();

    void Update(float elapsedTime);     // 更新処理

    void DrawDebug();                   // ImGui用

    void SetState(int newState);        // ステートセット
    void ChangeState(int newState);     // ステート変更
    void RegisterState(T* state);       // ステート登録

public:// 取得・設定
    int GetStateIndex();                            // 現在のステート番号取得
    T* GetCurrentState() { return currentState_; }  // 現在のステート取得

private:
    T* currentState_;            // 現在のステート
    std::vector<T*> statePool_;  // 各ステートを保持する配列
};

template<class T>
inline StateMachine<T>::~StateMachine()
{
    // --- 登録したステートを削除する ---
    for (T* state : statePool_)
    {
        delete state;
    }
    statePool_.clear();
}

template<class T>
inline void StateMachine<T>::Update(float elapsedTime)
{
    currentState_->Update(elapsedTime);
}

template<class T>
inline void StateMachine<T>::DrawDebug()
{
    static int state = 0;
    if (ImGui::TreeNode("stateMachine"))
    {
        ImGui::Text(currentState_->GetName());

        float temp = currentState_->GetTimer();
        ImGui::DragFloat("time", &temp);

        ImGui::SliderInt("state", &state, 0, statePool_.size() - 1);
        if (ImGui::Button("Set State"))
        {
            ChangeState(state);
        }

        ImGui::TreePop();
    }
}

template<class T>
inline void StateMachine<T>::SetState(int newState)
{
    currentState_ = statePool_.at(newState);
    currentState_->Initialize();
}

template<class T>
inline void StateMachine<T>::ChangeState(int newState)
{
    currentState_->Finalize();
    SetState(newState);
}

template<class T>
inline void StateMachine<T>::RegisterState(T* state)
{
    statePool_.emplace_back(state);
}

template<class T>
inline int StateMachine<T>::GetStateIndex()
{
    int i = 0;
    for (T* state : statePool_)
    {
        if (state == currentState_)
        {
            // --- i番号目のステートをリターン ---
            return i;
        }
        ++i;
    }

    // --- ステートが見つからなかったとき ---
    return -1;
}
