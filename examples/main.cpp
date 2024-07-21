#include "event_loop.h"

#include <iostream>

using namespace std::chrono_literals;

class MyTimerHandler : public event_loop::ITimerHandler
{
public:
    void Handle(event_loop::ITimer* eventLoop) override final
    {
        ++calledTimes;
        std::cout << "MyTimerHandler::Handle" << std::endl;

        constexpr uint32_t kMaxTimes = 3;
        if (calledTimes == kMaxTimes)
            return;

        eventLoop->Start(this, 1000ms);
    }

private:
    uint32_t calledTimes{ 0 };
};

int main()
{
    MyTimerHandler handler;

    event_loop::EventLoop ev;
    const std::shared_ptr<event_loop::ITimer> timer = ev.CreateTimer();
    timer->Start(&handler, 2000ms);

    ev.Run();

    return 0;
}
