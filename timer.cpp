#include "timer.h"

namespace event_loop {

Timer::Timer(IEventLoopController& ev)
    : ev_{ ev }
{
}

void Timer::Start(ITimerHandler* const timeHandler, const std::chrono::milliseconds timeout)
{
    handler_ = timeHandler;
    expirationTime_ = ev_.CurrentTime() + timeout;
    ev_.AddTimer(this);
}

void Timer::Stop()
{
    ev_.RemoveTimer(this);
}

ITimerHandler* Timer::GetTimerHandler() const
{
    return handler_;
}

std::chrono::system_clock::time_point Timer::GetExpirationTime() const
{
    return expirationTime_;
}

} // namespace event_loop
