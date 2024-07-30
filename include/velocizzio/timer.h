#pragma once

#include "detail/timer_internal_controller.h"
#include "detail/event_loop_internal_controller.h"
#include "timer_handler.h"

#include <chrono>
#include <memory>
#include <optional>

namespace velocizzio {

class Timer : public ITimerInternalController
{
public:
    Timer(IEventLoopInternalController& ev, std::shared_ptr<ITimerHandler> timerHandler);
    ~Timer();

    void Start(std::chrono::milliseconds timeout);
    void Start(std::shared_ptr<ITimerHandler> timerHandler, std::chrono::milliseconds timeout);

    void StartPeriodic(std::chrono::milliseconds interval);
    void StartPeriodic(std::shared_ptr<ITimerHandler> timerHandler, std::chrono::milliseconds interval);

    void Stop();
    void Destroy();

private:
    void Expire() override final;
    std::chrono::system_clock::time_point GetExpirationTime() const override final;

    void AddTimer(std::chrono::milliseconds timeout);

private:
    IEventLoopInternalController& ev_;
    std::shared_ptr<ITimerHandler> handler_{ nullptr };
    bool running_{ false };
    std::chrono::system_clock::time_point expirationTime_;
    std::optional<std::chrono::milliseconds> interval_;
};

} // namespace velocizzio
