#include "event_loop.h"
#include "timer_handler.h"

#include <iostream>

using namespace std::chrono_literals;

class MyTimerHandler
    : public event_loop::ITimerHandler
    , public std::enable_shared_from_this<MyTimerHandler>
{
public:
    void Handle(event_loop::Timer* timer) noexcept override final
    {
        ++calledTimes;
        std::cout << "MyTimerHandler::Handle" << std::endl;

        constexpr uint32_t kMaxTimes = 3;
        if (calledTimes == kMaxTimes)
            return;

        timer->Start(shared_from_this(), 1000ms);
    }

private:
    uint32_t calledTimes{ 0 };
};

int main()
{
    event_loop::EventLoop ev;

    event_loop::Timer timer = ev.CreateTimer();
    timer.Start(std::make_shared<MyTimerHandler>(), 3000ms);

    ev.Run();

    return 0;
}
