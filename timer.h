#pragma once

#include "timer_handler.h"
#include "event_loop_controller.h"

#include <chrono>

namespace event_loop {

class Timer
{
public:
    explicit Timer(IEventLoopController& ev);

    void Start(ITimerHandler* const timeHandler, const std::chrono::milliseconds timeout);
    void Stop();
    ITimerHandler* GetTimerHandler() const;
    std::chrono::system_clock::time_point GetExpirationTime() const;

private:
    IEventLoopController& ev_;
    std::chrono::system_clock::time_point expirationTime_;
    ITimerHandler* handler_{ nullptr };
};

} // namespace event_loop
