#pragma once

#include "detail/event_loop_internal_controller.h"
#include "timer.h"

#include <map>
#include <unordered_map>
#include <chrono>

namespace velocizzio {

class EventLoop : private IEventLoopInternalController
{
public:
    EventLoop();

    Timer CreateTimer(std::shared_ptr<ITimerHandler> handler);
    void Run();

private:
    std::chrono::system_clock::time_point CurrentTime() const override final;
    void AddTimer(ITimerInternalController* timer) override final;
    void RemoveTimer(ITimerInternalController* timer) override final;

private:
    void CheckTimersExpired();
    bool HasTimers() const;
    std::chrono::system_clock::time_point EarliestTimerExpirationTime() const;

private:
    using TimerSearchTree = std::multimap<
        std::chrono::system_clock::time_point /*expirationTime*/,
        ITimerInternalController*>;

private:
    std::chrono::system_clock::time_point time_;
    TimerSearchTree timers_;
    std::unordered_map<ITimerInternalController*, TimerSearchTree::const_iterator> timerPositions_;
};

} // namespace velocizzio
