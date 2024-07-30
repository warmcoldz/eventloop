#include "timer.h"

namespace velocizzio {

Timer::Timer(IEventLoopInternalController& ev, std::shared_ptr<ITimerHandler> handler)
    : ev_{ ev }
    , handler_{ std::move(handler) }
{
}

Timer::~Timer()
{
    Stop();
}

void Timer::Start(std::shared_ptr<ITimerHandler> timerHandler, const std::chrono::milliseconds timeout)
{
    if (!timerHandler)
        return;

    Stop();
    handler_ = std::move(timerHandler);
    AddTimer(timeout);
}

void Timer::Start(const std::chrono::milliseconds timeout)
{
    if (!handler_)
        return;

    Stop();
    AddTimer(timeout);
}

void Timer::StartPeriodic(std::chrono::milliseconds interval)
{
    interval_ = interval;
    Start(interval);
}

void Timer::StartPeriodic(std::shared_ptr<ITimerHandler> timerHandler, std::chrono::milliseconds interval)
{
    interval_ = interval;
    Start(timerHandler, interval);
}

void Timer::Destroy()
{
    handler_.reset();
}

void Timer::Expire()
{
    running_ = interval_.has_value();
    handler_->Handle(this);

    if (running_ && interval_.has_value())
    {
        AddTimer(*interval_);
    }
}

void Timer::Stop()
{
    if (!running_)
        return;

    ev_.RemoveTimer(this);
    running_ = false;
}

void Timer::AddTimer(const std::chrono::milliseconds timeout)
{
    expirationTime_ = ev_.CurrentTime() + timeout;
    ev_.AddTimer(this);
    running_ = true;
}

std::chrono::system_clock::time_point Timer::GetExpirationTime() const
{
    return expirationTime_;
}

} // namespace velocizzio
