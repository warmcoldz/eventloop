#pragma once

#include "timer.h"
#include "event_loop_controller.h"

#include <map>
#include <chrono>

namespace event_loop {

class EventLoop : private IEventLoopController
{
public:
    EventLoop();

    Timer CreateTimer();
    void Run();

private:
    std::chrono::system_clock::time_point CurrentTime() const override final;
    void AddTimer(Timer* timer) override final;
    void RemoveTimer(Timer* timer) override final;

private:
    static std::chrono::system_clock::time_point TimeNow();
    void CheckTimersExpired();
    bool HasTimers() const;
    std::chrono::system_clock::time_point EarliestTimerExpirationTime() const;

private:
    std::chrono::system_clock::time_point time_;
    std::multimap<std::chrono::system_clock::time_point /*expirationTime*/, Timer*> timers_;
};

} // namespace event_loop
