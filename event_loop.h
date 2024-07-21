#include <map>
#include <memory>
#include <chrono>
#include <thread>

namespace event_loop {

class ITimerHandler;

class ITimer
{
public:
    virtual void Start(ITimerHandler* timeHandler, std::chrono::milliseconds timeout) = 0;
    virtual void Stop() = 0;
    virtual ITimerHandler* GetTimerHandler() const = 0;
    virtual std::chrono::system_clock::time_point GetExpirationTime() const = 0;

    virtual ~ITimer() = default;
};

class ITimerHandler
{
public:
    virtual void Handle(ITimer* eventLoop) = 0;

    virtual ~ITimerHandler() = default;
};

class IEventLoopImpl
{
public:
    virtual std::chrono::system_clock::time_point CurrentTime() const = 0;
    virtual void AddTimer(ITimer* timer) = 0;
    virtual void RemoveTimer(ITimer* timer) = 0;

    virtual ~IEventLoopImpl() = default;
};

class Timer : public ITimer
{
public:
    explicit Timer(IEventLoopImpl& ev)
        : ev_{ ev }
    {
    }

    void Start(ITimerHandler* const timeHandler, const std::chrono::milliseconds timeout) override final
    {
        handler_ = timeHandler;
        expirationTime_ = ev_.CurrentTime() + timeout;
        ev_.AddTimer(this);
    }

    void Stop() override final
    {
        ev_.RemoveTimer(this);
    }

    ITimerHandler* GetTimerHandler() const override final
    {
        return handler_;
    }

    std::chrono::system_clock::time_point GetExpirationTime() const override final
    {
        return expirationTime_;
    }

private:
    IEventLoopImpl& ev_;
    std::chrono::system_clock::time_point expirationTime_;
    ITimerHandler* handler_{ nullptr };
};

class EventLoop : private IEventLoopImpl
{
public:
    EventLoop()
        : time_{ TimeNow() }
    {
    }

    std::shared_ptr<ITimer> CreateTimer()
    {
        IEventLoopImpl* const ev = this;
        return std::make_shared<Timer>(*ev);
    }

    void Run()
    {
        while (true)
        {
            CheckTimers();
            if (!HasTimers())
                break;

            std::this_thread::sleep_until(EarliestExpirationTime());
            time_ = TimeNow();
        }
    }


private:
    std::chrono::system_clock::time_point CurrentTime() const override final
    {
        return time_;
    }

    void AddTimer(ITimer* timer) override final
    {
        timers_.emplace(timer->GetExpirationTime(), timer);
    }

    void RemoveTimer(ITimer* timer) override final
    {
        const std::chrono::system_clock::time_point expirationTime = timer->GetExpirationTime();

        const auto begin = timers_.lower_bound(expirationTime);
        const auto end = timers_.upper_bound(expirationTime);

        for (auto it = begin; it != end; ++it)
        {
            if (it->second == timer)
            {
                timers_.erase(it);
                break;
            }
        }
    }

private:
    static std::chrono::system_clock::time_point TimeNow()
    {
        return std::chrono::system_clock::now();
    }

    void CheckTimers()
    {
        for (auto it = timers_.begin(); it != timers_.end();)
        {
            if (time_ < it->first)
                break;

            ITimer* const timer = it->second;
            timer->GetTimerHandler()->Handle(timer);
            it = timers_.erase(it);
        }
    }

    bool HasTimers() const
    {
        return !timers_.empty();
    }

    std::chrono::system_clock::time_point EarliestExpirationTime() const
    {
        return timers_.begin()->first;
    }

private:
    std::chrono::system_clock::time_point time_;
    std::multimap<std::chrono::system_clock::time_point /*expirationTime*/, ITimer*> timers_;
};

} // namespace event_loop
