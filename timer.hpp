#pragma once

class Timer 
{
    // A one-shot timer
    friend class RepeatingTimer;
    float time;
    float interval;

public:
    Timer(float interval)
        : interval(interval), time(0.0f) {}

    void Update()
    {
        time += GetFrameTime();
    }

    bool Trigger()
    {
        if (time >= interval)
        {  
            return true;
        }
        return false;
    }

};

class RepeatingTimer : public Timer
{
    // A repeating timer that resets itself when triggered
public:
    RepeatingTimer(float interval)
        : Timer(interval)   {}

    bool Trigger()
    {
        if (time >= interval)
        {
            time -= interval;
            return true;
        }
        return false;
    }

    void SkipToNextTrigger()
    {
        time = interval;
    }
};

