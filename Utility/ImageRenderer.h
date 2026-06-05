#include <ftxui/dom/elements.hpp>

#include "Utility/stb_image.h"

namespace TTOT::Utilities
{
    class ImageRenderer
    {
        private:
            int max_width;
            int max_height;
        public:
            ftxui::Element ImageRender(const std::string& file_path);
    };
}