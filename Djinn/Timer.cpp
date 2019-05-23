#include <Windows.h>

#include "Timer.h"

// Non-static members.
Timer::Timer() : _stopped(false), _secondsPerCount(0.0), _deltaTime(0.0),
    _baseTime(0), _prevTime(0), _currTime(0), _pausedTime(0), _stopTime(0)
{
    __int64 frequency;
    QueryPerformanceFrequency(reinterpret_cast<LARGE_INTEGER*>(&frequency));
    _secondsPerCount = 1.0 / static_cast<double>(frequency);
}

Timer::~Timer() {}

void Timer::Start()
{
    if (!_stopped) return;

    __int64 currTime;
    QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&currTime));

    _pausedTime += currTime - _stopTime;
    _stopTime = 0;
    _stopped = false;
}

void Timer::Stop()
{
    if (_stopped) return;

    __int64 currTime;
    QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&currTime));

    _stopTime = currTime;
    _stopped = true;
}

void Timer::Update()
{
    if (_stopped) {
        _deltaTime = 0.0;
        return;
    }
    
    QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&this->_currTime));

    _deltaTime = (_currTime - _prevTime) * _secondsPerCount;
    if (_deltaTime < 0.0) _deltaTime = 0.0;

    _prevTime = _currTime;
}

void Timer::Reset()
{
    __int64 currTime;
    QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&currTime));

    _baseTime = currTime;
    _prevTime = currTime;
    _pausedTime = 0;
    _stopTime = 0;
    _stopped = false;
}

double Timer::DeltaTime() const {
    return _deltaTime;
}

double Timer::TimeSinceStartup() const {
    if (_stopped) {
        return ((_stopTime - _pausedTime) - _baseTime) * _secondsPerCount;
    } else {
        return ((_currTime - _pausedTime) - _baseTime) * _secondsPerCount;
    }
}
