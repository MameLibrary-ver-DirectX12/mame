#pragma once

// 設計考えてる
class Skill
{
public:
    Skill() {}
    virtual ~Skill() {}

    virtual void Update(const float& elapsedTime);
    virtual void Render();

};

