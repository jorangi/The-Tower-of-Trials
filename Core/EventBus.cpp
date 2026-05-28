#include "Core/EventBus.h"

namespace TTOT::Core
{
    template<typename T>
    std::shared_ptr<TTOT::Utilities::Action<const T&>> EventBus::GetAction()
    {
        std::type_index typeIndex(typeid(T));
        auto it = _actions.find(typeIndex);
        if(it == _actions.end())
        {
            auto action = std::make_shared<TTOT::Utilities::Action<const T&>>();
            _actions[typeIndex] = action;
            return action;
        }
        return std::static_pointer_cast<TTOT::Utilities::Action<const T&>>(_actions[typeIndex]);
    }
    template<typename T>
    uint32_t TTOT::Core::EventBus::Subscribe(std::function<void(const T&)> callback)
    {
        GetAction<T>()->AddListener(callback);
    }
    template<typename T>
    void TTOT::Core::EventBus::Unsubscribe(uint32_t id)
    {
        GetAction<T>()->RemoveListener(id);
    }
    template<typename T>
    void TTOT::Core::EventBus::Publish(const T& eventData)
    {
        GetAction<T>()->Invoke(eventData);
    }
}