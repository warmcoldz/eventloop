#include "event_loop.h"
#include "timer_handler.h"

#include <iostream>

using namespace std::chrono_literals;

class MyTimerHandler : public event_loop::ITimerHandler
{
public:
    void Handle(event_loop::Timer* timer) noexcept override final
    {
        ++calledTimes;
        std::cout << "MyTimerHandler::Handle" << std::endl;

        constexpr uint32_t kMaxTimes = 4;
        if (calledTimes == kMaxTimes)
            return;

        timer->Start(calledTimes * 1000ms);
    }

private:
    uint32_t calledTimes{ 0 };
};

int main()
{
    event_loop::EventLoop ev;

    event_loop::Timer timer = ev.CreateTimer(std::make_shared<MyTimerHandler>());
    timer.Start(500ms);

    ev.Run();

    return 0;
}
