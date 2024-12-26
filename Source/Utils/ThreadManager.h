#ifndef THREAD_MANAGER_H
#define THREAD_MANAGER_H

#include <condition_variable>
#include <functional>
#include <mutex>
#include <queue>
#include <thread>
#include <tuple>
#include <utility>


namespace ClassicLauncher
{

    template <class... X>
    class ThreadManager
    {
    public:

        std::queue<std::function<void()>> callbackQueue;
        std::mutex queueMutex;
        std::condition_variable cv;
        std::function<void(X...)> callbackLoad;

        void SetCallback(std::function<void(X...)> callback) { callbackLoad = std::move(callback); }

        // Add the callback to the queue
        template <typename... Args>
        void Notify(Args&&... args)
        {
            {
                std::lock_guard<std::mutex> lock(queueMutex);  // Add the callback to the queue
                callbackQueue.emplace(
                    [this, args = std::make_tuple(std::forward<Args>(args)...)]() mutable
                    {
                        std::apply(
                            [this](auto&&... args)
                            {
                                callbackLoad(std::forward<decltype(args)>(args)...);
                            },
                            std::move(args));
                    });
            }
            cv.notify_all();
        }

        void CallbackExecution()
        {
            std::unique_lock<std::mutex> lock(queueMutex);
            // cv.wait(lock, [this] { return !callbackQueue.empty(); });
            while (!callbackQueue.empty())
            {
                const auto cb = callbackQueue.front();
                callbackQueue.pop();
                lock.unlock();
                cb();
                lock.lock();
            }
        }

        template <class Fn, class... Args>
        void StartThread(Fn&& f, Args&&... a)
        {
            auto task = std::make_shared<std::thread>(std::forward<Fn>(f), std::forward<Args>(a)...);
            task->detach();  // Detach the thread so it runs independently
        }
    };

}

#endif // THREAD_MANAGER_H