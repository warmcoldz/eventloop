#include <velocizzio/event_loop.h>
#include <iostream>

using namespace std::chrono_literals;

class MyTimerHandler : public velocizzio::ITimerHandler
{
public:
    void Handle(velocizzio::Timer* timer) noexcept override final
    {
        ++calledTimes;
        std::cout << "MyTimerHandler::Handle" << std::endl;

        constexpr uint32_t kMaxTimes = 4;
        if (calledTimes == kMaxTimes)
        {
            timer->Stop();
        }
    }

private:
    uint32_t calledTimes{ 0 };
};

int main()
{
    velocizzio::EventLoop ev;
    velocizzio::Timer timer = ev.CreateTimer(std::make_shared<MyTimerHandler>());

    timer.StartPeriodic(500ms);
    ev.Run();

    return 0;
}
