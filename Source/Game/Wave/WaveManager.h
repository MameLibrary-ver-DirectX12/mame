#pragma once


class WaveManager
{
private:
    WaveManager()   = default;
    ~WaveManager()  = default;

public:
    static WaveManager& Instance()
    {
        static WaveManager instance;
        return instance;
    }

    

};

