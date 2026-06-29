#include "Core/Scene/IntroSceneState/ProcessingWriteClassState.h"
#include "Core/Event/ScreenRefreshEvent.h"
#include "Core/Scene/IntroScene.h"
#include "Datas/ClassDTO.h"
#include "Utility/FormatString.h"
#include "Utility/PathHelper.h"
#include <fstream>


using namespace ftxui;
using namespace TTOT::Core::Scenes;
using namespace TTOT::Core::Scenes::IntroSceneState;

namespace TTOT::Core::Scenes::IntroSceneState {
void ProcessingWriteClassState::OnEnter(IntroScene &scene) {
  lastUpdateTime = std::chrono::steady_clock::now();
  scene.introContext.geminiFuture = std::async(std::launch::async, [this,
                                                                    &scene]() {
    std::ifstream schemaFile(
        TTOT::Utilities::GetAssetPath("Assets/Schema/ClassSchema.json"));
    nlohmann::json schemaJson;
    if (!schemaFile.is_open()) {
      return std::string(
          "Error: 스키마 파일을 찾을 수 없습니다. 경로를 확인해주세요.");
    }
    schemaFile >> schemaJson;
    schemaFile.close();
    std::string _prompt = R"(
                본 게임은 로그(Rogue)와 같은 텍스트 기반 탑뷰 게임이며, 전투는 턴제로 이루어집니다.
                포켓몬스터나 고전 RPG처럼 서로 마주 본 상태에서 기술을 주고받는 턴제 전투를 치르지만, 당신의 설명에서는 이러한 '메타적인 시스템'이 드러나서는 안 됩니다.
                당신은 텍스트 RPG '시련의 탑(The Tower of Trials)'의 초입에서 모험가를 심사하고 인도하는 탑의 신비로운 의지(Voice of the Tower)이자 탑의 여신입니다.
                다음 입력된 예시와 정보를 바탕으로 직업(클래스)를 만들고 서술하시오.
                예시) 전사: 무너진 전장에서 홀로 살아남아 아군을 잃은 죄책감과 더 강력한 힘에 대한 갈망으로 탑의 문을 두드렸습니다. 앞을 가로막는 시련 앞에서도 물러서지 않으며, 강인한 신체와 굳건한 방패로 적의 맹공을 묵묵히 버텨내고 반격의 기회를 노리는 듬직한 전위입니다. 하지만 복잡한 마법이나 기교와는 거리가 멀어 변칙적인 위기 상황에는 둔감하게 반응하곤 합니다. 탑의 거주민들은 이 무모하리만치 단단한 기백을 경외하면서도, 그 고집스러움을 은밀히 이용하려 들 것입니다. (따라서 체력과 힘, 방어력에 보정과 기본값을 높게 책정. 반면 지능이나 마나 등은 약점으로 보정과 기본값을 낮게 책정.)
                도적: 지하 세계의 피비린내 나는 숙청에서 도망쳐, 전설의 유물을 훔치거나 과거를 세탁하기 위해 그림자 속에서 탑으로 흘러들었습니다. 찰나의 틈을 놓치지 않는 기민함으로 누구보다 먼저 움직이며, 상대가 숨을 고르기도 전에 치명적인 급소를 꿰뚫는 암살자입니다. 다만 일격에 적의 숨통을 끊지 못해 피를 말리는 진흙탕 싸움으로 이어지거나, 묵직한 공격을 정면에서 허용하면 한순간에 목숨이 위태로워집니다. 상인과 정보상들은 이 자의 날카로운 감각을 신뢰하면서도, 언제 등 뒤에서 단검이 날아올지 몰라 늘 경계합니다.(따라서 민첩과 스피드 등에 보정과 기본값을 높게...)
                직업의 보정치는 게임 전반적인 밸런스를 고려하여 특화형 혹은 육각형밸런스형을 고려하여 배정하여야 합니다.
                또한 답변 시 직업의 이름과 특징(배경, 전투, 세간의 시선 - 이야기 전체와의 상호작용을 위함 등)을 서술하시오.
                답변에는 가능한 이 자가 탑에 오게된 배경을 작성하세요. 해당 내용을 바탕으로 탑의 모든 존재들과 상호작용될 것입니다.
                [직업 설명(classDesc) 작성 규칙]
                1. '메타 발언 절대 금지': 턴(Turn), 인카운터, 스탯, 데미지 같은 게임 시스템 용어를 절대 사용하지 마세요. 대신 누구보다 먼저 움직여, 묵직한 공격을 허용하면, 숨 돌릴 틈 없는 난전 속에서 같은 몰입감 있는 TRPG 서사 용어로 대체하세요.
                2. 대괄호(예: [배경])나 줄바꿈 없이, 오직 하나의 유려하게 흐르는 문단(통텍스트)으로 작성하세요.
                3. 문단에는 다음 내용이 자연스럽게 녹아들어야 합니다.
                - 탑에 발을 들이게 된 고유하고 비극적이거나 신비로운 배경
                - 적과 마주했을 때 싸우는 방식 (재빠른 일격, 묵묵히 버텨내는 맷집, 체력 소모전 등)
                - 탑의 주민들, 상인, 학자 등이 이 직업을 가진 자를 어떻게 바라보고 대하는지에 대한 세계관 상호작용
                [오버파워(OP) 제약 및 밸런스 규칙]
                1. 모든 초기 능력치(hp, mp, str, dex, int, wis, cha, def, spd)의 총합은 '20 이하의 자연수'여야 합니다. (각 능력치 개별 최대 10 이하)
                2. 사용자가 세계관을 무너뜨리는 무적의 사기 캐릭터(OP)를 요구할 경우, 요구를 수용하되 그에 상응하는 '치명적인 서사적 패널티'를 부여하여 밸런스를 강제로 맞추세요.
                - (예: "과거엔 신을 베었으나 현재는 심연의 저주로 육신이 부서져 내린 상태", "압도적인 파괴력을 가졌으나 생명력을 갉아먹는 고통을 감수해야 함" 등)
                --
                사용자의 이름은: {{name}} 입니다.
                사용자의 성별은: {{gender}} 입니다.
                해당 개인 정보를 바탕으로 직업 설정에 잘 녹여내세요. 
                중요한 정보가 될 수 있습니다.
                이름의 경우 상대적으로 중요하지 않을 수 있습니다.
            )";
    _prompt = TTOT::Utilities::FormatString(_prompt, "{{name}}",
                                            scene.introContext.userName);
    _prompt = TTOT::Utilities::FormatString(
        _prompt, "{{gender}}", scene.introContext.userGender ? "여자" : "남자");

    nlohmann::json body = {
        {"contents",
         nlohmann::json::array(
             {{{"parts",
                nlohmann::json::array(
                    {{{"text",
                       std::string("사용자가 요청하는 커스텀 클래스의 정보: ") +
                           scene.introContext.customClass}}})}}})},
        {"systemInstruction",
         {{"parts", nlohmann::json::array({{{"text", _prompt}}})}}},
        {"generationConfig",
         {
             {"responseMimeType", "application/json"},
             {"responseSchema", schemaJson},
         }}};
    std::string responseStr = scene.context.gemini.Request(body);
    std::ofstream debugFile("gemini_response_debug.json");
    if (debugFile.is_open()) {
      debugFile << responseStr;
      debugFile.close();
    }
    return responseStr;
  });
  std::thread([this, &scene]() {
    while (scene.introStep == IntroStep::ProcessingWriteClass) {
      std::this_thread::sleep_for(std::chrono::milliseconds(50));
      scene.context.eventBus.Publish(TTOT::Core::Events::ScreenRefreshEvent{});
    }
  }).detach();
}
void ProcessingWriteClassState::OnExit(IntroScene &scene) {}
void ProcessingWriteClassState::Update(IntroScene &scene) {
  auto now = std::chrono::steady_clock::now();
  double elapsedTime =
      std::chrono::duration<double>(now - lastUpdateTime).count();
  if (elapsedTime >= 0.1) {
    lastUpdateTime = now;
    this->spinnerIndex =
        (this->spinnerIndex + 1) % scene.introContext.spinner.size();
    scene.context.eventBus.Publish(TTOT::Core::Events::ScreenRefreshEvent{});
  }
  if (scene.introContext.geminiFuture.valid() &&
      scene.introContext.geminiFuture.wait_for(std::chrono::seconds(0)) ==
          std::future_status::ready) {
    std::string result = scene.introContext.geminiFuture.get();
    try {
      auto parsedJson = nlohmann::json::parse(result);
      auto classDto = parsedJson.get<TTOT::Datas::ClassDTO>();
      scene.introContext.classInfo =
          std::make_unique<TTOT::Class::ClassBase>(classDto);
      scene.introContext.customClass = scene.introContext.classInfo->GetName();
      scene.introContext.customClassDesc =
          scene.introContext.classInfo->GetDesc();
    } catch (const std::exception &e) {
      scene.introContext.customClass = "⚠️ 클래스 데이터 파싱 실패";
      scene.introContext.customClassDesc =
          "원본 응답:\n" + result + "\n\n오류 내용:\n" + e.what();
    }
    scene.ChangeState(IntroStep::ConfirmClass);
    scene.context.eventBus.Publish(TTOT::Core::Events::ScreenRefreshEvent{});
  }
}
void ProcessingWriteClassState::HandleInput(IntroScene &scene,
                                            const Event &event) {}
Element ProcessingWriteClassState::Render(IntroScene &scene) {
  return hbox({text("⚙ 설정에 맞는 사용자 정의 클래스를 생성 중입니다..."),
               text(scene.introContext.spinner[this->spinnerIndex])});
}
} // namespace TTOT::Core::Scenes::IntroSceneState