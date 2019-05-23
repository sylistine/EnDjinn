#pragma once

#include "Djinn.h"

class Timer {
public:
    Timer();
    Timer(const Timer& other) = delete;
    ~Timer();
    void Start();
    void Stop();
    void Update();
    void Reset();
    double DeltaTime() const;
    double TimeSinceStartup() const;
private:
    bool _stopped;
    double _secondsPerCount;
    double _deltaTime;
    __int64 _baseTime;
    __int64 _prevTime;
    __int64 _currTime;
    __int64 _pausedTime;
    __int64 _stopTime;
};

