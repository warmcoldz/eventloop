#pragma once

namespace velocizzio {

class Timer;

class ITimerHandler
{
public:
    virtual void Handle(Timer* timer) noexcept = 0;

    virtual ~ITimerHandler() = default;
};

} // namespace velocizzio
