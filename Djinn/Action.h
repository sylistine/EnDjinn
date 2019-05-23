#pragma once

#include <functional>

#define EVENTS_TEMPLATE template<class ... args>

namespace Djn
{
    EVENTS_TEMPLATE
    class Action
    {
    private:
        static uint32_t nextHandle;
    public:
        Action(std::function<void(args...)> callback) : handle(nextHandle++)
        {
            this->callback = callback;
        }
        ~Action() = default;
        Action(const Action& other) = delete;
        Action& operator=(const Action& other) = delete;
        void operator()(args...);
        uint32_t GetHandle() const;
    private:
        std::function<void(args...)> callback;
        uint32_t handle;
    };

    EVENTS_TEMPLATE
    uint32_t Action<args...>::nextHandle = 0;

    EVENTS_TEMPLATE
    void Action<args...>::operator()(args...)
    {
        callback(args...);
    }

    EVENTS_TEMPLATE
    uint32_t Action<args...>::GetHandle() const
    {
        return handle;
    }
}
