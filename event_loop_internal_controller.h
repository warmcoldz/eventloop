#pragma once

#include "timer_internal_controller.h"

#include <chrono>

namespace event_loop {

class IEventLoopInternalController
{
public:
    virtual std::chrono::system_clock::time_point CurrentTime() const = 0;
    virtual void AddTimer(ITimerInternalController* timer) = 0;
    virtual void RemoveTimer(ITimerInternalController* timer) = 0;

    virtual ~IEventLoopInternalController() = default;
};

} // namespace event_loop
