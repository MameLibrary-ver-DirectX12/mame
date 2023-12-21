#pragma once

template<class T>
class State
{
public:
    State(T* owner, const char* name) : owner_(owner), name_(name) {}
    virtual ~State() {}

    virtual void Initialize() = 0;
    virtual void Update(const float& elapsedTime) = 0;
    virtual void Finalize() = 0;

public:// �擾�E�ݒ�
    const char* GetName() { return name_; }

    // --- �^�C�}�[�֘A ---
    void SetTimer(float time) { timer_ = time; }
    float GetTimer()const { return timer_; }
    void SubtractTime(float time) { timer_ -= time; }

private:
    float timer_ = 0;    // �^�C�}�[

protected:
    T* owner_;
    const char* name_;
};