#include "timer.h"

namespace event_loop {

Timer::Timer(IEventLoopController& ev)
    : ev_{ ev }
{
}

Timer::~Timer()
{
    Stop();
}

void Timer::Start(std::shared_ptr<ITimerHandler> timerHandler, const std::chrono::milliseconds timeout)
{
    Stop();

    handler_ = std::move(timerHandler);
    expirationTime_ = ev_.CurrentTime() + timeout;
    ev_.AddTimer(this);
    running_ = true;
}

void Timer::ExpireTimer()
{
    running_ = false;
    handler_->Handle(this);
}

void Timer::Stop()
{
    if (!running_)
        return;

    ev_.RemoveTimer(this);
}

std::chrono::system_clock::time_point Timer::GetExpirationTime() const
{
    return expirationTime_;
}

} // namespace event_loop
