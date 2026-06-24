#pragma once
#include "Utility/stb_image.h"
#include <ftxui/dom/elements.hpp>

namespace TTOT::Utilities {
class ImageRenderer {
private:
  int max_width;
  int max_height;

public:
  ftxui::Element ImageRender(const std::string &file_path);
};
} // namespace TTOT::Utilities