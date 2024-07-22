#pragma once

#include "timer_handler.h"
#include "event_loop_controller.h"

#include <chrono>
#include <memory>

namespace event_loop {

class Timer
{
public:
    explicit Timer(IEventLoopController& ev);
    ~Timer();

    void Start(std::shared_ptr<ITimerHandler> timerHandler, std::chrono::milliseconds timeout);
    void Stop();

private:
    void ExpireTimer();
    std::chrono::system_clock::time_point GetExpirationTime() const;

    friend class EventLoop;

private:
    IEventLoopController& ev_;
    bool running_{ false };
    std::shared_ptr<ITimerHandler> handler_{ nullptr };
    std::chrono::system_clock::time_point expirationTime_;
};

} // namespace event_loop
