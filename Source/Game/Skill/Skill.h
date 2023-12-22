#pragma once

// İŒvl‚¦‚Ä‚é
class Skill
{
public:
    Skill() {}
    virtual ~Skill() {}

    virtual void Update(const float& elapsedTime);
    virtual void Render();

};

