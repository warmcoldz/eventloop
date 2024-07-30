#include "event_loop.h"

#include <thread>

namespace velocizzio {
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

Timer EventLoop::CreateTimer(std::shared_ptr<ITimerHandler> handler)
{
    return Timer{ *this, std::move(handler) };
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
    const auto insertedIt = timers_.emplace(timer->GetExpirationTime(), timer);

    try
    {
        timerPositions_.try_emplace(timer, insertedIt);
    }
    catch (const std::exception&)
    {
        timers_.erase(insertedIt);
        throw;
    }
}

void EventLoop::RemoveTimer(ITimerInternalController* const timer)
{
    const auto it = timerPositions_.find(timer);
    if (it != timerPositions_.end())
    {
        timers_.erase(it->second);
        timerPositions_.erase(it);
    }
}

void EventLoop::CheckTimersExpired()
{
    for (auto it = timers_.begin(); it != timers_.end(); it = timers_.begin())
    {
        if (time_ < it->first)
            break;

        ITimerInternalController* const timer = it->second;
        timers_.erase(it);
        timerPositions_.erase(timer);

        timer->Expire();
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

} // namespace velocizzio
