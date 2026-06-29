#include "Core/Scene/FieldScene.h"
#include "Core/Event/PlayerGetEvent.h"
#include "Core/Event/SaveCompleteEvent.h"
#include "Core/Event/SaveRequestEvent.h"
#include "Core/Event/SceneChangeEvent.h"
#include "Core/Event/ScreenRefreshEvent.h"
#include "Entity/Player.h"
#include "Utility/ScreenUtility.h"
#include <chrono>
#include <ftxui/component/component.hpp>
#include <ftxui/component/component_base.hpp>
#include <ftxui/dom/elements.hpp>
#include <iomanip>
#include <random>
#include <sstream>
#include <string>

using namespace ftxui;
namespace TTOT::Core::Scenes {
FieldScene::FieldScene(TTOT::Core::GameContext &context) : SceneBase(context) {}

std::string FieldScene::GetCurrentTimeStr() {
  auto now = std::chrono::system_clock::now();
  auto now_time = std::chrono::system_clock::to_time_t(now);
  std::tm localTm;
  localtime_s(&localTm, &now_time);
  std::stringstream ss;
  ss << std::put_time(&localTm, "%H:%M:%S");
  return ss.str();
}

void FieldScene::AddLog(const std::string &msg) {
  std::string timeStr = GetCurrentTimeStr();
  gameLogs.push_back("[" + timeStr + "] " + msg);
  if (gameLogs.size() > 5) {
    gameLogs.erase(gameLogs.begin());
  }
}

void FieldScene::OnEnter() {
  camera = std::make_unique<TTOT::Utilities::Camera>(
      30, 30, 2, context.playerBaseInfo.gender);

  int cols = 30 * 2 + 1 + 42 + 4;
  int rows = 30 + 15;
  TTOT::Utilities::ScreenUtility::Resize(cols, rows);

  if (context.playerBaseInfo.mapSeed != 0) {
    seed = context.playerBaseInfo.mapSeed;
  } else {
    std::random_device rd;
    seed = rd();
    context.playerBaseInfo.mapSeed = seed;
  }

  width = 100;
  height = 100;
  mapGenerator =
      std::make_unique<TTOT::Core::BSPMapGenerator>(seed, width, height);
  mapGenerator->Generate();

  if (context.playerBaseInfo.playerX != -1 &&
      context.playerBaseInfo.playerY != -1) {
    playerX = context.playerBaseInfo.playerX;
    playerY = context.playerBaseInfo.playerY;
  } else {
    auto startPos = mapGenerator->GetFirstRoomCenter();
    playerX = startPos.first;
    playerY = startPos.second;
    context.playerBaseInfo.playerX = playerX;
    context.playerBaseInfo.playerY = playerY;
  }

  camera->SetTargetPosition(playerX, playerY);
  camera->OnPositionUpdated(playerX, playerY);

  context.eventBus.Publish(TTOT::Core::Events::PlayerGetEvent{
      [this](const TTOT::Entities::Player &playerStates) {
        userInfoUI = std::make_unique<TTOT::UI::UserInformation>(playerStates);
      }});

  // Subscribe to Save Complete Event
  saveCompleteSubId =
      context.eventBus.Subscribe<TTOT::Core::Events::SaveCompletevent>(
          [this](const TTOT::Core::Events::SaveCompletevent &event) {
            if (event.success) {
              AddLog("게임이 성공적으로 저장되었습니다. (Slot " +
                     std::to_string(event.slotIndex) + ")");
            } else {
              AddLog("세이브 실패: " + event.errorMessage);
            }
            context.eventBus.Publish(TTOT::Core::Events::ScreenRefreshEvent{});
          });

  AddLog("탑 " + std::to_string(context.playerBaseInfo.currentFloor) +
         "층에 진입하였습니다. (시드: " + std::to_string(seed) + ")");
}
void FieldScene::OnExit() {
  if (saveCompleteSubId != 0) {
    context.eventBus.Unsubscribe<TTOT::Core::Events::SaveCompletevent>(
        saveCompleteSubId);
    saveCompleteSubId = 0;
  }
}
void FieldScene::Update() {}
void FieldScene::HandleInput(const ftxui::Event &event) {
  int dx = 0;
  int dy = 0;

  if (event == ftxui::Event::ArrowUp || event == ftxui::Event::Character('w')) {
    dy = -1;
  } else if (event == ftxui::Event::ArrowDown ||
             event == ftxui::Event::Character('s')) {
    dy = 1;
  } else if (event == ftxui::Event::ArrowLeft ||
             event == ftxui::Event::Character('a')) {
    dx = -1;
  } else if (event == ftxui::Event::ArrowRight ||
             event == ftxui::Event::Character('d')) {
    dx = 1;
  } else if (event == ftxui::Event::Character('p') ||
             event == ftxui::Event::Character('P')) {
    // Save hotkey: Save to slot 1
    AddLog("저장 요청을 전송하였습니다...");
    context.eventBus.Publish(TTOT::Core::Events::SaveRequestEvent{
        1, context.playerBaseInfo.currentFloor, seed, playerX, playerY});
  } else if (event == ftxui::Event::Escape) {
    // Transition to Pause Menu
    context.eventBus.Publish(TTOT::Core::Events::SceneChangeEvent{5});
    return;
  }

  if (dx != 0 || dy != 0) {
    int nextX = playerX + dx;
    int nextY = playerY + dy;
    if (nextX >= 0 && nextX < static_cast<int>(width) && nextY >= 0 &&
        nextY < static_cast<int>(height)) {

      int cellValue = mapGenerator->GetBSP()[nextY * width + nextX];
      if (cellValue == 1 || cellValue == 2) {
        playerX = nextX;
        playerY = nextY;

        // Sync context
        context.playerBaseInfo.playerX = playerX;
        context.playerBaseInfo.playerY = playerY;

        camera->OnPositionUpdated(playerX, playerY);
      }
    }
  }
}
ftxui::Element FieldScene::Render() {
  if (!userInfoUI) {
    return vbox({text("불러오는 중...")}) | border;
  }

  ftxui::Element overlayText_Map =
      hbox({text("["), text(to_string(context.playerBaseInfo.currentFloor)),
            text("F - "), text("시련의 층")}) |
      bold | color(Color::Yellow) | bgcolor(Color::Black);

  // Render last 5 game logs
  Elements logElements;
  for (const auto &log : gameLogs) {
    logElements.push_back(text(log) | color(Color::GrayLight));
  }
  // Fill remaining space up to 5 lines if fewer logs exist
  while (logElements.size() < 5) {
    logElements.push_back(text(""));
  }

  auto mainContent =
      hbox({dbox({camera->RenderViewport(
                      mapGenerator->GetBSP(), mapGenerator->GetWidth(),
                      mapGenerator->GetHeight(), playerX, playerY),
                  overlayText_Map}),
            separator(), userInfoUI->Render()});

  auto logConsole = vbox({text(" 🔔 로그 콘솔 ") | bold | color(Color::Cyan),
                          separatorLight(), vbox(std::move(logElements))}) |
                    border;

  return vbox({mainContent | flex, separator(), logConsole});
}
} // namespace TTOT::Core::Scenes