#include "Core/Scene/PauseScene.h"
#include "Core/Event/GameLoopEvent.h"
#include "Core/Event/SaveCompleteEvent.h"
#include "Core/Event/SaveRequestEvent.h"
#include "Core/Event/SceneChangeEvent.h"
#include "Core/Event/ScreenRefreshEvent.h"
#include "Utility/PathHelper.h"
#include <Utility/ScreenUtility.h>
#include <fileapi.h>
#include <filesystem>
#include <ftxui/component/component.hpp>
#include <ftxui/dom/elements.hpp>
#include <winnt.h>

using namespace ftxui;

namespace TTOT::Core::Scenes {

PauseScene::PauseScene(TTOT::Core::GameContext &context) : SceneBase(context) {}

void PauseScene::OnEnter() {
  selectedIndex = 0;
  outputText = "";
  subState = PauseSubState::MainMenu;
  targetSlot = 1;
  warningSelectedIndex = 1; // Default to 'No' (Cancel) for safety

  // Set the return scene ID so SettingScene knows where to go back
  context.settingReturnSceneId = 5;
}

void PauseScene::OnExit() {}
void PauseScene::Update() {}

void PauseScene::HandleInput(const ftxui::Event &event) {
  // 1. OVERWRITE WARNING SUB-STATE
  if (subState == PauseSubState::OverwriteWarning) {
    if (event == ftxui::Event::ArrowDown ||
        event == ftxui::Event::Character('s') ||
        event == ftxui::Event::ArrowUp ||
        event == ftxui::Event::Character('w')) {
      warningSelectedIndex = (warningSelectedIndex + 1) % 2;
      context.eventBus.Publish(TTOT::Core::Events::ScreenRefreshEvent{});
      return;
    }

    if (event == ftxui::Event::Escape) {
      subState = PauseSubState::SlotSelection;
      selectedIndex = targetSlot - 1; // Return cursor to the selected slot
      outputText = "저장을 취소했습니다.";
      context.eventBus.Publish(TTOT::Core::Events::ScreenRefreshEvent{});
      return;
    }

    if (event == ftxui::Event::Return) {
      if (warningSelectedIndex == 0) {
        // Yes, Overwrite!
        outputText = "저장 요청을 전송하였습니다...";

        // Subscribe temporarily to SaveCompletevent (synchronous execution)
        uint32_t subId =
            context.eventBus.Subscribe<TTOT::Core::Events::SaveCompletevent>(
                [this](const TTOT::Core::Events::SaveCompletevent &e) {
                  if (e.success) {
                    this->outputText =
                        "게임이 성공적으로 저장되었습니다. (Slot " +
                        std::to_string(this->targetSlot) + ")";
                  } else {
                    this->outputText = "저장 실패: " + e.errorMessage;
                  }
                });

        context.eventBus.Publish(TTOT::Core::Events::SaveRequestEvent{
            targetSlot, context.playerBaseInfo.currentFloor,
            context.playerBaseInfo.mapSeed, context.playerBaseInfo.playerX,
            context.playerBaseInfo.playerY});

        // Unsubscribe immediately after synchronous save completes to prevent
        // dangling reference crash
        context.eventBus.Unsubscribe<TTOT::Core::Events::SaveCompletevent>(
            subId);

        subState = PauseSubState::MainMenu;
        selectedIndex = 1; // Back to 'Save Game' option
      } else {
        // No, Cancel!
        subState = PauseSubState::SlotSelection;
        selectedIndex = targetSlot - 1;
        outputText = "저장을 취소했습니다.";
      }
      context.eventBus.Publish(TTOT::Core::Events::ScreenRefreshEvent{});
      return;
    }
    return;
  }

  // 2. SLOT SELECTION SUB-STATE (10 SLOTS)
  if (subState == PauseSubState::SlotSelection) {
    if (event == ftxui::Event::ArrowDown ||
        event == ftxui::Event::Character('s')) {
      selectedIndex = (selectedIndex + 1) % 11;
      context.eventBus.Publish(TTOT::Core::Events::ScreenRefreshEvent{});
      return;
    } else if (event == ftxui::Event::ArrowUp ||
               event == ftxui::Event::Character('w')) {
      selectedIndex = (selectedIndex - 1 + 11) % 11;
      context.eventBus.Publish(TTOT::Core::Events::ScreenRefreshEvent{});
      return;
    }

    if (event == ftxui::Event::Escape) {
      subState = PauseSubState::MainMenu;
      selectedIndex = 1; // Select 'Save Game' option
      outputText = "";
      context.eventBus.Publish(TTOT::Core::Events::ScreenRefreshEvent{});
      return;
    }

    if (event == ftxui::Event::Return) {
      if (selectedIndex >= 0 && selectedIndex <= 9) {
        targetSlot = selectedIndex + 1;
        std::string filename =
            "Save_Slot_" + std::to_string(targetSlot) + ".json";
        std::string filepath = TTOT::Utilities::GetSavePath(filename).string();

        bool fileExists;
        fileExists = [filepath]() -> bool {
          DWORD dwAttrib = GetFileAttributesA(filepath.c_str());
          return (dwAttrib != INVALID_FILE_ATTRIBUTES &&
                  !(dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
        }();
        if (fileExists) {
          subState = PauseSubState::OverwriteWarning;
          warningSelectedIndex = 1;
          outputText = "[경고] 기존 세이브 파일이 존재합니다!";
        } else {
          outputText = "저장 요청을 전송하였습니다...";

          uint32_t subId =
              context.eventBus.Subscribe<TTOT::Core::Events::SaveCompletevent>(
                  [this](const TTOT::Core::Events::SaveCompletevent &e) {
                    if (e.success) {
                      this->outputText =
                          "게임이 성공적으로 저장되었습니다. (Slot " +
                          std::to_string(this->targetSlot) + ")";
                    } else {
                      this->outputText = "저장 실패: " + e.errorMessage;
                    }
                  });

          context.eventBus.Publish(TTOT::Core::Events::SaveRequestEvent{
              targetSlot, context.playerBaseInfo.currentFloor,
              context.playerBaseInfo.mapSeed, context.playerBaseInfo.playerX,
              context.playerBaseInfo.playerY});

          // Unsubscribe immediately after synchronous save completes to prevent
          // dangling reference crash
          context.eventBus.Unsubscribe<TTOT::Core::Events::SaveCompletevent>(
              subId);

          subState = PauseSubState::MainMenu;
          selectedIndex = 1; // Back to Save Game
        }
      } else {
        // Back Option (index 10)
        subState = PauseSubState::MainMenu;
        selectedIndex = 1;
        outputText = "";
      }
      context.eventBus.Publish(TTOT::Core::Events::ScreenRefreshEvent{});
    }
    return;
  }

  // 3. MAIN MENU NAVIGATION
  if (event == ftxui::Event::ArrowDown ||
      event == ftxui::Event::Character('s')) {
    selectedIndex = (selectedIndex + 1) % 5;
    context.eventBus.Publish(TTOT::Core::Events::ScreenRefreshEvent{});
    return;
  } else if (event == ftxui::Event::ArrowUp ||
             event == ftxui::Event::Character('w')) {
    selectedIndex = (selectedIndex - 1 + 5) % 5;
    context.eventBus.Publish(TTOT::Core::Events::ScreenRefreshEvent{});
    return;
  }

  if (event == ftxui::Event::Escape) {
    // Escape in Pause Menu goes back to game
    context.eventBus.Publish(TTOT::Core::Events::SceneChangeEvent{3});
    return;
  }

  if (event == ftxui::Event::Return) {
    if (selectedIndex == 0) {
      // Resume Game
      context.eventBus.Publish(TTOT::Core::Events::SceneChangeEvent{3});
    } else if (selectedIndex == 1) {
      // Save Game - transition to 10 Slot Selection
      subState = PauseSubState::SlotSelection;
      selectedIndex = 0; // Default to Slot 1
      outputText = "저장할 슬롯을 고르십시오.";
    } else if (selectedIndex == 2) {
      // Settings
      context.eventBus.Publish(TTOT::Core::Events::SceneChangeEvent{1});
    } else if (selectedIndex == 3) {
      // Go to Title
      context.eventBus.Publish(TTOT::Core::Events::SceneChangeEvent{0});
    } else if (selectedIndex == 4) {
      // Exit Game
      context.eventBus.Publish(TTOT::Core::Events::GameLoopEvent{
          TTOT::Core::Events::GameLoopAction::Exitgame});
    }
    context.eventBus.Publish(TTOT::Core::Events::ScreenRefreshEvent{});
  }
}

ftxui::Element PauseScene::Render() {
  // Overwrite Warning Screen
  if (subState == PauseSubState::OverwriteWarning) {
    auto make_warning_option = [&](int index, const std::string &label) {
      if (index == warningSelectedIndex) {
        return text(" > " + label) | bold | color(Color::Red);
      } else {
        return text("   " + label) | color(Color::GrayLight);
      }
    };

    auto warningBox =
        vbox({
            text("[경고] 기존 데이터가 존재합니다. 정말 덮어씌우시겠습니까?") |
                bold | color(Color::Yellow) | hcenter,
            separator(),
            make_warning_option(0, "예 (Yes, Overwrite)"),
            make_warning_option(1, "아니오 (No, Cancel)"),
        }) |
        border | color(Color::Red);

    auto content =
        vbox({
            text("[ 일시 정지 ]") | bold | color(Color::Yellow) | hcenter,
            separator(),
            warningBox,
            separatorLight(),
            text(outputText) | color(Color::Yellow) | hcenter,
        }) |
        size(WIDTH, EQUAL, 55) | center;

    return content | border;
  }

  // Slot Selection Screen (10 slots)
  if (subState == PauseSubState::SlotSelection) {
    auto make_slot_option = [&](int index, const std::string &label) {
      if (index == selectedIndex) {
        return text(" > " + label) | bold | color(Color::Cyan);
      } else {
        return text("   " + label) | color(Color::GrayLight);
      }
    };

    std::vector<Element> slotOptions;
    for (int i = 0; i < 10; ++i) {
      std::string label = "슬롯 " + std::to_string(i + 1) + " (Slot " +
                          std::to_string(i + 1) + ")";
      std::string filename = "Save_Slot_" + std::to_string(i + 1) + ".json";
      std::string filepath = TTOT::Utilities::GetSavePath(filename).string();

      if (std::filesystem::exists(filepath)) {
        slotOptions.push_back(make_slot_option(i, label + " [데이터 존재]"));
      } else {
        slotOptions.push_back(make_slot_option(i, label + " [비어 있음]"));
      }
    }
    slotOptions.push_back(separatorLight());
    slotOptions.push_back(make_slot_option(10, "뒤로 가기 (Back)"));

    auto slotsBox = vbox(slotOptions) | border;

    auto content =
        vbox({
            text("[ 저장 슬롯 선택 ]") | bold | color(Color::Yellow) | hcenter,
            separator(),
            slotsBox,
            separatorLight(),
            text(outputText) | color(Color::Cyan) | hcenter,
        }) |
        size(WIDTH, EQUAL, 55) | center;

    return content | border;
  }

  // Main Pause Menu
  auto make_option = [&](int index, const std::string &label) {
    if (index == selectedIndex) {
      return text(" > " + label) | bold | color(Color::Cyan);
    } else {
      return text("   " + label) | color(Color::GrayLight);
    }
  };

  auto menuBox = vbox({
                     make_option(0, "계속하기 (Resume)"),
                     make_option(1, "저장하기 (Save Game)"),
                     make_option(2, "설정 (Settings)"),
                     make_option(3, "타이틀 화면으로 (Go to Title)"),
                     make_option(4, "게임 종료 (Exit Game)"),
                 }) |
                 border;

  auto content =
      vbox({
          text("[ 일시 정지 ]") | bold | color(Color::Yellow) | hcenter,
          separator(),
          menuBox,
          separatorLight(),
          text(outputText) | color(Color::Green) | hcenter,
      }) |
      size(WIDTH, EQUAL, 55) | center;

  return content | border;
}

} // namespace TTOT::Core::Scenes
