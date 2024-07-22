#include "event_loop.h"

#include <thread>
#include <queue>

namespace event_loop {
namespace {

std::chrono::system_clock::time_point TimeNow()
{
    return std::chrono::system_clock::now();
}

} // namespace

EventLoop::EventLoop()
    : time_{ TimeNow() }
{
}

Timer EventLoop::CreateTimer()
{
    return Timer{ *this };
}

void EventLoop::Run()
{
    while (true)
    {
        time_ = TimeNow();

        CheckTimersExpired();
        if (!HasTimers())
            break;

        std::this_thread::sleep_until(EarliestTimerExpirationTime());
    }
}

std::chrono::system_clock::time_point EventLoop::CurrentTime() const
{
    return time_;
}

void EventLoop::AddTimer(ITimerInternalController* const timer)
{
    timers_.emplace(timer->GetExpirationTime(), timer);
}

void EventLoop::RemoveTimer(ITimerInternalController* const timer)
{
    const std::chrono::system_clock::time_point expirationTime = timer->GetExpirationTime();

    const auto begin = timers_.lower_bound(expirationTime);
    const auto end = timers_.upper_bound(expirationTime);

    for (auto it = begin; it != end; ++it)
    {
        if (it->second == timer)
        {
            timers_.erase(it);
            break;
        }
    }
}

void EventLoop::CheckTimersExpired()
{
    for (auto it = timers_.begin(); it != timers_.end();)
    {
        if (time_ < it->first)
            break;

        ITimerInternalController* const timer = it->second;
        timer->ExpireTimer();
        it = timers_.erase(it);
    }
}

bool EventLoop::HasTimers() const
{
    return !timers_.empty();
}

std::chrono::system_clock::time_point EventLoop::EarliestTimerExpirationTime() const
{
    return timers_.begin()->first;
}

} // namespace event_loop
