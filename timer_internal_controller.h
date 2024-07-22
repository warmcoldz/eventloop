#pragma once

#include <chrono>

namespace event_loop {

class ITimerInternalController
{
public:
    virtual void Expire() = 0;
    virtual std::chrono::system_clock::time_point GetExpirationTime() const = 0;

    virtual ~ITimerInternalController() = default;
};

} // namespace event_loop
