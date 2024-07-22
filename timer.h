#pragma once

#include "timer_internal_controller.h"
#include "timer_handler.h"
#include "event_loop_internal_controller.h"

#include <chrono>
#include <memory>

namespace event_loop {

class Timer : public ITimerInternalController
{
public:
    explicit Timer(IEventLoopInternalController& ev);
    ~Timer();

    void Start(std::shared_ptr<ITimerHandler> timerHandler, std::chrono::milliseconds timeout);
    void Stop();

private:
    void ExpireTimer() override final;
    std::chrono::system_clock::time_point GetExpirationTime() const override final;

private:
    IEventLoopInternalController& ev_;
    bool running_{ false };
    std::shared_ptr<ITimerHandler> handler_{ nullptr };
    std::chrono::system_clock::time_point expirationTime_;
};

} // namespace event_loop
