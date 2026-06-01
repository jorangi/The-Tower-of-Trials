#pragma once
#include <typeindex>
#include <functional>
#include <unordered_map>
#include <memory>
#include "Utility/Action.h"

namespace TTOT::Core
{
    /// @brief EventBus 클래스, 게임 내에서 이벤트를 관리하고 전달
    class EventBus
    {   
        private:
            std::unordered_map<std::type_index, std::shared_ptr<void>> _actions;

            /// @brief 내부 함수로 특정 타입 T에 대한 Action 객체를 찾아 반환. 없으면 새로 생성
            /// @tparam T 이벤트 데이터 타입
            /// @return T 타입에 대한 Action 객체의 shared_ptr
            template<typename T>
            std::shared_ptr<TTOT::Utilities::Action<const T&>> GetAction()
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
        public:
            /// @brief 이벤트를 구독하는 함수, 내부에서 GetAction을 통해 해당 타입의 Action 객체를 얻고 콜백을 등록
            /// @tparam T 이벤트 데이터 타입
            /// @param callback 이벤트 처리를 위한 콜백 함수
            template<typename T>
            uint32_t Subscribe(std::function<void(const T&)> callback)
            {
                GetAction<T>()->AddListener(callback);
            }

            /// @brief 이벤트 구독을 해제하는 함수
            /// @tparam T 이벤트 데이터 타입
            /// @param id 해제할 구독의 ID
            template<typename T>
            void Unsubscribe(uint32_t id)
            {
                GetAction<T>()->RemoveListener(id);
            }

            /// @brief 이벤트를 발행하는 함수
            /// @tparam T 이벤트 데이터 타입
            /// @param eventData 발행할 이벤트 데이터
            template<typename T>
            void Publish(const T& eventData)
            {
                GetAction<T>()->Invoke(eventData);
            }
    };
}