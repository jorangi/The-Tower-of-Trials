#pragma once
#include <functional>
#include <vector>
#include <algorithm>
#include <stdint.h>

namespace TTOT::Utilities
{
    template<typename... Args>
    class Action
    {
        private:
            struct ListenerNode
            {
                uint32_t _id;
                std::function<void(Args...)> _callback;
            };
            std::vector<ListenerNode> _listeners;
            uint32_t _nextId = 0;
        public:
            uint32_t AddListener(function<void(Args...)> callback)
            {
                uint32_t funcId = _nextId++;
                _listeners.push_back({funcId, callback});
                return funcId;
            }
            uint32_t operator+=(std::function<void(Args...)> callback)
            {
                return AddListener(callback);
            }
            void Invoke(Args... args)
            {
                for(const auto& listener : _listeners)
                {
                    listener._callback(args...);
                }
            }
            void operator()(Args... args)
            {
                Invoke(args...);
            }
            void RemoveListener(uint32_t id)
            {
                _listeners.erase(
                    remove_if(_listeners.begin(), _listeners.end(), [id](const ListenerNode& node)
                    {
                        return node._id == targetId;
                    }),
                    _listeners.end()
                );
            }
            void operator-=(uint32_t id)
            {
                RemoveListener(id);
            }
    };
}