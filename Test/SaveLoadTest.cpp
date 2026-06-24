#include "Core/Event/LoadCompleteEvent.h"
#include "Core/Event/LoadRequestEvent.h"
#include "Core/Event/SaveCompleteEvent.h"
#include "Core/Event/SaveRequestEvent.h"
#include "Core/EventBus.h"
#include "Core/GameContext.h"
#include "Core/MasterSaveData.h"
#include "Core/SaveManager.h"
#include "Network/ConnGemini.h"
#include "Utility/ImageRenderer.h"
#include <cassert>
#include <filesystem>
#include <iostream>

using namespace std;
namespace fs = std::filesystem;

int main() {
  cout << "============================================" << endl;
  cout << "        Save / Load 기능 단위 테스트          " << endl;
  cout << "============================================" << endl;

  // 1. Context & Managers 초기화
  TTOT::Core::EventBus eventBus;
  TTOT::Utilities::ImageRenderer imageRenderer;
  TTOT::Network::ConnGemini gemini;
  TTOT::Core::GameContext context(eventBus, imageRenderer, gemini);
  TTOT::Core::SaveManager saveManager;

  const int testSlotIndex = 99;
  std::string testFilename =
      "Save_Slot_" + std::to_string(testSlotIndex) + ".json";

  // 혹시 기존에 존재하던 테스트 세이브 파일이 있다면 제거
  if (fs::exists(testFilename)) {
    fs::remove(testFilename);
  }

  bool saveCompleted = false;
  bool saveSuccess = false;
  bool loadCompleted = false;
  bool loadSuccess = false;
  TTOT::Core::MasterSaveData loadedData;

  // 2. 이벤트 구독 설정
  eventBus.Subscribe<TTOT::Core::Events::SaveCompletevent>(
      [&](const TTOT::Core::Events::SaveCompletevent &event) {
        if (event.slotIndex == testSlotIndex) {
          saveCompleted = true;
          saveSuccess = event.success;
          cout << "[Test Log] SaveCompleteEvent 수신: slotIndex="
               << event.slotIndex
               << ", success=" << (event.success ? "true" : "false") << endl;
        }
      });

  eventBus.Subscribe<TTOT::Core::Events::LoadCompleteEvent>(
      [&](const TTOT::Core::Events::LoadCompleteEvent &event) {
        if (event.slotIndex == testSlotIndex) {
          loadCompleted = true;
          loadSuccess = event.success;
          loadedData = event.saveData;
          cout << "[Test Log] LoadCompleteEvent 수신: slotIndex="
               << event.slotIndex
               << ", success=" << (event.success ? "true" : "false") << endl;
        }
      });

  // GameManager처럼 EventBus를 통해 세이브/로드 요청 이벤트를 처리하도록 연결
  eventBus.Subscribe<TTOT::Core::Events::SaveRequestEvent>(
      [&](const TTOT::Core::Events::SaveRequestEvent &event) {
        // 더미 세이브 데이터를 생성하여 저장
        TTOT::Core::MasterSaveData saveData;
        saveData.slotIndex = event.slotIndex;
        saveData.saveTime = "2026-06-24 12:00:00";
        saveData.currentFloor = 5;
        saveData.playerData = {{"name", "테스터"},
                               {"HP", {{"currentHP", 100}, {"baseMaxHP", 100}}},
                               {"classData", {{"className", "전사"}}}};
        saveManager.OnSaveRequest(event.slotIndex, saveData, context);
      });

  eventBus.Subscribe<TTOT::Core::Events::LoadRequestEvent>(
      [&](const TTOT::Core::Events::LoadRequestEvent &event) {
        saveManager.OnLoadRequest(event.slotIndex, context);
      });

  // 3. 세이브 테스트 진행
  cout << "\n[Step 1] 세이브 요청 발행 (Slot " << testSlotIndex << ")" << endl;
  eventBus.Publish(TTOT::Core::Events::SaveRequestEvent{testSlotIndex});

  // 검증
  assert(saveCompleted && "세이브 완료 이벤트가 발행되지 않았습니다.");
  assert(saveSuccess && "세이브 처리에 실패했습니다.");
  assert(fs::exists(testFilename) && "세이브 파일이 생성되지 않았습니다.");
  cout << "[Step 1] 세이브 테스트 통과 (파일 존재 및 이벤트 수신 완료)" << endl;

  // 4. 로드 테스트 진행
  cout << "\n[Step 2] 로드 요청 발행 (Slot " << testSlotIndex << ")" << endl;
  eventBus.Publish(TTOT::Core::Events::LoadRequestEvent{testSlotIndex});

  // 검증
  assert(loadCompleted && "로드 완료 이벤트가 발행되지 않았습니다.");
  assert(loadSuccess && "로드 처리에 실패했습니다.");
  assert(loadedData.slotIndex == testSlotIndex &&
         "로드된 슬롯 인덱스가 다릅니다.");
  assert(loadedData.currentFloor == 5 && "로드된 층수가 다릅니다.");
  assert(loadedData.saveTime == "2026-06-24 12:00:00" &&
         "로드된 세이브 시간이 다릅니다.");
  assert(loadedData.playerData["name"] == "테스터" &&
         "로드된 플레이어 이름이 다릅니다.");
  assert(loadedData.playerData["classData"]["className"] == "전사" &&
         "로드된 플레이어 직업이 다릅니다.");
  cout << "[Step 2] 로드 테스트 통과 (데이터 무결성 검증 완료)" << endl;

  // 5. 정리 (테스트 파일 삭제)
  if (fs::exists(testFilename)) {
    fs::remove(testFilename);
    cout << "\n[Step 3] 테스트 세이브 파일 정리 완료." << endl;
  }

  cout << "\n모든 세이브/로드 단위 테스트가 성공적으로 완료되었습니다!" << endl;
  return 0;
}
