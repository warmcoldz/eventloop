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
    Timer(IEventLoopInternalController& ev, std::shared_ptr<ITimerHandler> timerHandler);
    ~Timer();

    void Start(std::chrono::milliseconds timeout);
    void Start(std::shared_ptr<ITimerHandler> timerHandler, std::chrono::milliseconds timeout);
    void Stop();

private:
    void Expire() override final;
    std::chrono::system_clock::time_point GetExpirationTime() const override final;

    void AddTimer(std::chrono::milliseconds timeout);

private:
    IEventLoopInternalController& ev_;
    std::shared_ptr<ITimerHandler> handler_{ nullptr };
    bool running_{ false };
    std::chrono::system_clock::time_point expirationTime_;
};

} // namespace event_loop
