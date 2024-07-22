#pragma once

#include <chrono>

namespace event_loop {

class Timer;

class IEventLoopController
{
public:
    virtual std::chrono::system_clock::time_point CurrentTime() const = 0;
    virtual void AddTimer(Timer* timer) = 0;
    virtual void RemoveTimer(Timer* timer) = 0;

    virtual ~IEventLoopController() = default;
};

} // namespace event_loop
