#pragma once
#include "ftxui/dom/elements.hpp"

namespace TTOT::UI
{
    class IUI
    {
        public:
            virtual ftxui::Element Render() = 0;
            virtual ~IUI() = default;
    };
}