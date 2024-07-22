#pragma once

namespace event_loop {

class Timer;

class ITimerHandler
{
public:
    virtual void Handle(Timer* timer) = 0;

    virtual ~ITimerHandler() = default;
};

} // namespace event_loop
