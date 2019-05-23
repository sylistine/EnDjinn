#pragma once

#include <vector>
#include <functional>

#include "Action.h"

#define EVENTS_TEMPLATE template<class ... args>

namespace Djn
{
    EVENTS_TEMPLATE
    class Event
    {
    public:
        Event() = default;
        ~Event() = default;
        Event(const Event& other) = delete;
        Event& operator=(const Event& rhs) = delete;
        void operator()(args...);
        Event<args...>& operator+=(Action<args...>* callback);
        Event<args...>& operator-=(Action<args...>* callback);
    private:
        std::vector<Action<args...>*> callbacks;
    };

    template<class ... args>
    void
    Event<args...>::operator()(args...)
    {
        for (auto i = 0; i < callbacks.size(); ++i) {
            (*callbacks[i])(args...);
        }
    }

    EVENTS_TEMPLATE
    Event<args...>&
    Event<args...>::operator+=(Action<args...>* callback)
    {
        for (auto& cb : callbacks) {
            if (cb->GetHandle() == callback->GetHandle())
                throw Exception("Callback already exists in event.");
        }
        callbacks.push_back(callback);
        return *this;
    }

    EVENTS_TEMPLATE
    Event<args...>&
    Event<args...>::operator-=(Action<args...>* callback)
    {
        for (auto it = callbacks.begin(); it != callbacks.end();) {
            if ((*it)->GetHandle() == callback->GetHandle()) {
                callbacks.erase(it);
                return *this;
            }
            it++;
        }
        throw Exception("Callback does not exist in event.");
    }
}
