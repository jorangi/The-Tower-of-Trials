#define STB_IMAGE_IMPLEMENTATION
#include "Utility/ImageRenderer.h"

using namespace ftxui;
using namespace TTOT::Utilities;

namespace TTOT::Utilities
{
    Element ImageRenderer::ImageRender(const std::string& file_path)
    {
        max_width = 60;
        max_height = 30;
        int width, height, channels;
        unsigned char* img_data = stbi_load(file_path.c_str(), &width, &height, &channels, 3);
        if(!img_data) return text("❌ 이미지를 로드할 수 없습니다: " + file_path) | color(ftxui::Color::Red);
        int new_width = width;
        int new_height = height;

        if(new_width > max_width)
        {
            new_height = (height * max_width) / width;
            new_width = max_width;
        }
        if(new_height > max_height)
        {
            new_width = (width * max_height) / height;
            new_height = max_height;
        }
        Elements rows;

        for(int y = 0; y < new_height; y += 2)
        {
            Elements line;
            for(int x = 0; x < new_width; ++x)
            {
                int orig_x = (x * width) / new_width;
                int orig_y = (y * height) / new_height;
                int orig_y_bot = (y + 1) * height / new_height;

                int top_idx = (orig_y * width + orig_x) * 3;
                Color top_color = Color::RGB(img_data[top_idx], img_data[top_idx + 1], img_data[top_idx + 2]);

                Color bottom_color = Color::RGB(0, 0, 0);
                if(y + 1 < new_height && orig_y_bot < height)
                {
                    int bot_idx = (orig_y_bot * width + orig_x) * 3;
                    bottom_color = Color::RGB(img_data[bot_idx], img_data[bot_idx + 1], img_data[bot_idx + 2]);
                }
                line.push_back(text("▄") | color(bottom_color) | bgcolor(top_color));
            }
            rows.push_back(hbox(std::move(line)));
        }
        stbi_image_free(img_data);
        return vbox(std::move(rows));
    }
}