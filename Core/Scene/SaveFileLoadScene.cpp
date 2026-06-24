#include "Core/Scene/SaveFileLoadScene.h"
#include "Core/Event/LoadRequestEvent.h"
#include "Core/Event/SceneChangeEvent.h"
#include "Core/GameContext.h"
#include <fstream>
#include <nlohmann/json.hpp>

using namespace TTOT::Core::Events;

namespace TTOT::Core::Scenes {
SaveFileLoadScene::SaveFileLoadScene(GameContext &context)
    : SceneBase(context) {}

void SaveFileLoadScene::OnEnter() {
  selectedIndex = 0;
  outputText = "";

  // Check each of the 10 save slots
  for (int i = 1; i <= 10; ++i) {
    std::string filename = "Save_Slot_" + std::to_string(i) + ".json";
    std::ifstream file(filename);
    if (file.is_open()) {
      try {
        nlohmann::json j;
        file >> j;
        saveSlots[i - 1].exists = true;
        saveSlots[i - 1].playerClass =
            j["playerData"]["classData"]["className"];
        saveSlots[i - 1].saveTime = j.value("saveTime", "");
        saveSlots[i - 1].currentFloor = j.value("currentFloor", 1);
        if (j.contains("playerData") && j["playerData"].contains("name")) {
          saveSlots[i - 1].playerName =
              j["playerData"]["name"].get<std::string>();
        }
      } catch (...) {
        saveSlots[i - 1].exists = false;
      }
      file.close();
    } else {
      saveSlots[i - 1].exists = false;
    }
  }
}

void SaveFileLoadScene::OnExit() {}
void SaveFileLoadScene::Update() {}

void SaveFileLoadScene::HandleInput(const ftxui::Event &event) {
  if (event == ftxui::Event::ArrowDown) {
    selectedIndex = (selectedIndex + 1) % 10;
    return;
  } else if (event == ftxui::Event::ArrowUp) {
    selectedIndex = (selectedIndex - 1 + 10) % 10;
    return;
  } else if (event == ftxui::Event::Escape) {
    context.eventBus.Publish(SceneChangeEvent{0});
    return;
  } else if (event == ftxui::Event::Return) {
    if (saveSlots[selectedIndex].exists) {
      outputText =
          "슬롯 " + std::to_string(selectedIndex + 1) + " 불러오기 진행 중...";
      context.eventBus.Publish(LoadRequestEvent{selectedIndex + 1});
      context.eventBus.Publish(SceneChangeEvent{3});
    } else {
      outputText = "선택한 슬롯에 세이브 파일이 없습니다.";
    }
    return;
  }
}

ftxui::Element SaveFileLoadScene::Render() {
  using namespace ftxui;
  Elements slotElements;
  for (int i = 0; i < 10; ++i) {
    std::string slotText = " 슬롯 " + std::to_string(i + 1) + ": ";
    if (saveSlots[i].exists) {
      slotText += "<" + saveSlots[i].playerClass + ">" +
                  saveSlots[i].playerName + " - " +
                  std::to_string(saveSlots[i].currentFloor) + "층 (" +
                  saveSlots[i].saveTime + ")";
    } else {
      slotText += "세이브 없음";
    }

    auto element = text(slotText);
    if (selectedIndex == i) {
      element = element | bold | color(Color::Cyan);
    } else {
      element = element | color(Color::White);
    }
    slotElements.push_back(element);
  }

  return vbox({vbox({text("The Tower of Trials - 불러오기") | bold |
                     color(Color::Yellow) | hcenter}) |
                   border,
               vbox(std::move(slotElements)) | hcenter, separator(),
               vbox({text(outputText.empty()
                              ? "방향키: 이동   Enter: 불러오기   ESC: 뒤로가기"
                              : outputText) |
                     color(Color::GrayLight) | hcenter}) |
                   hcenter}) |
         border;
}
} // namespace TTOT::Core::Scenes
