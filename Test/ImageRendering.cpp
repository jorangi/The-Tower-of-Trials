// #include <string>

// // FTXUI 헤더
// #include <ftxui/dom/elements.hpp>
// #include <ftxui/screen/screen.hpp>
// #include <ftxui/component/screen_interactive.hpp>
// #include <ftxui/component/component.hpp>

// // 이미지 로드 라이브러리 (stb_image)
// #define STB_IMAGE_IMPLEMENTATION
// #include "Utility/stb_image.h"

// using namespace ftxui;

// // 이미지 파일을 읽어서 지정한 크기 내로 리사이징하여 FTXUI Element로 변환하는 함수
// // max_width: 터미널 가로 칸 수에 맞출 이미지 최대 가로 픽셀 (기본값 80)
// // max_height: 터미널 세로 칸 수에 맞출 이미지 최대 세로 픽셀 (기본값 60 -> 터미널 30줄 차지)
// Element LoadAndRenderImage(const std::string& file_path, int max_width = 80, int max_height = 60) {
//     int width, height, channels;
    
//     // 강제로 3채널(RGB)로 이미지를 읽어옵니다.
//     unsigned char* img_data = stbi_load(file_path.c_str(), &width, &height, &channels, 3);
    
//     if (!img_data) {
//         return text("❌ 이미지를 로드할 수 없습니다: " + file_path) | color(Color::Red);
//     }

//     // --- [리사이징 크기 계산] 원본 비율을 유지하면서 max_width/max_height 안으로 맞춤 ---
//     int new_width = width;
//     int new_height = height;

//     // 가로가 기준을 초과하면 축소 비율 계산
//     if (new_width > max_width) {
//         new_height = (height * max_width) / width;
//         new_width = max_width;
//     }
//     // 축소 후에도 세로가 기준을 초과하면 다시 축소 비율 계산
//     if (new_height > max_height) {
//         new_width = (width * max_height) / height;
//         new_height = max_height;
//     }

//     Elements rows;

//     // 축소된 해상도(new_height) 기준으로 루프를 돕니다. (세로 2픽셀씩 묶음)
//     for (int y = 0; y < new_height; y += 2) {
//         Elements line;
//         for (int x = 0; x < new_width; ++x) {
            
//             // 최근접 이웃(Nearest Neighbor) 매핑: 축소된 좌표 (x, y)를 원본 좌표 (orig_x, orig_y)로 역산
//             int orig_x = (x * width) / new_width;
//             int orig_y = (y * height) / new_height;
//             int orig_y_bot = ((y + 1) * height) / new_height;

//             // 1. 상단 픽셀 색상 추출 (원본 데이터에서 가져옴)
//             int top_idx = (orig_y * width + orig_x) * 3;
//             Color top_color = Color::RGB(img_data[top_idx], img_data[top_idx + 1], img_data[top_idx + 2]);

//             // 2. 하단 픽셀 색상 추출
//             Color bottom_color = Color::RGB(0, 0, 0); 
//             if (y + 1 < new_height && orig_y_bot < height) {
//                 int bot_idx = (orig_y_bot * width + orig_x) * 3;
//                 bottom_color = Color::RGB(img_data[bot_idx], img_data[bot_idx + 1], img_data[bot_idx + 2]);
//             }

//             // 하프 블록에 전경색/배경색 입히기
//             line.push_back(text("▄") | color(bottom_color) | bgcolor(top_color));
//         }
//         rows.push_back(hbox(std::move(line)));
//     }

//     // 메모리 해제
//     stbi_image_free(img_data);
    
//     return vbox(std::move(rows));
// }

// int main() {
//     auto screen = ScreenInteractive::FitComponent();

//     std::string image_path = "Assets/Images/my_image.png"; 

//     // UI 레이아웃 구성 
//     // 터미널 크기에 맞게 가로 최대 70픽셀, 세로 최대 50픽셀로 제한하여 렌더링하도록 설정
//     auto document = vbox({
//         text("FTXUI Auto-Resized Image Viewer") | bold | center,
//         separator(),
//         LoadAndRenderImage(image_path, 70, 50) | center, 
//     }) | border;

//     auto renderer = Renderer([&] { return document; });
//     screen.Loop(renderer);

//     return 0;
// }