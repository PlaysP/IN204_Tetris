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

    void Reset()
    {
        time = 0.0f;
    }

    void SetInterval(float newInterval)
    {
        interval = newInterval;
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

class KeyPressTimer : public Timer {
private:
    KeyboardKey key;

    // Holding the key should not register multiple inputs until the interval has passed.
    // The timer starts only when the key is pressed
    // Exception: spamming the key (pressing and realeasing quickly) should register as multiple inputs.
    bool canBeInput = true;

public:
    KeyPressTimer(float interval, KeyboardKey key)
        : Timer(interval), key(key) {}

    
    bool IsPressedAndReady() {
        if (IsKeyDown(key) && canBeInput) {
            canBeInput = false;
            return true;
        }
        if (!IsKeyDown(key)) {
            canBeInput = true;
            Timer::Reset();
        }
        return false;
    }

    void Update() {
        if (!canBeInput) {
            Timer::Update();
            if (Timer::Trigger()) {
                canBeInput = true;
                Timer::Reset();
            }
        }
    }
};