#pragma once

// �݌v�l���Ă�
class Skill
{
public:
    Skill() {}
    virtual ~Skill() {}

    virtual void Update(const float& elapsedTime);
    virtual void Render();

};

