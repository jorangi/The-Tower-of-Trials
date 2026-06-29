#include "Core/Scene/IntroScene.h"
#include "Core/Event/ScreenRefreshEvent.h"
#include "Core/Scene/IntroSceneState/ProcessingWritePurposeState.h"
#include "Utility/FormatString.h"
#include "Utility/PathHelper.h"
#include <string>
#include <fstream>

using namespace ftxui;
using namespace TTOT::Core::Scenes;
using namespace TTOT::Core::Scenes::IntroSceneState;

namespace TTOT::Core::Scenes::IntroSceneState
{
    void ProcessingWritePurposeState::OnEnter(IntroScene& scene)
    {
        lastUpdateTime = std::chrono::steady_clock::now();
        scene.introContext.geminiFuture = std::async(std::launch::async, [this, &scene]()
        {
            std::ifstream schemaFile(TTOT::Utilities::GetAssetPath("Assets/Schema/PurposeSchema.json"));
            nlohmann::json schemaJson;
            if(!schemaFile.is_open())
            {
                return std::string("Error: 스키마 파일을 찾을 수 없습니다. 경로를 확인해주세요.");
            }
            schemaFile >> schemaJson;
            schemaFile.close();

            _prompt = R"(
당신은 텍스트 RPG '시련의 탑(The Tower of Trials)'의 초입에서 모험가를 심사하고 인도하는 탑의 신비로운 의지(Voice of the Tower)이자 탑의 여신입니다.
사용자가 탑에 들어오려는 '목적'을 입력하면, 그 목적을 달성하기 위한 스토리 연출과 구체적인 1단계 퀘스트(Quest) 스펙을 설계해 주어야 합니다.

[작성 규칙]
1. guidanceText: 사용자의 목적을 적극적으로 반영하되, 맹목적으로 똑같은 단어만 반복하지 말고 세계관에 맞게 고급스럽고 세련된 2~3문장 이내(한 단락)의 신비로운 지침 대사를 작성하세요.
2. narrativeObjective: 퀘스트를 요약하는 명료한 한 문장을 작성하세요.
3. targetFloor: 이 시작 퀘스트를 수행하는 곳은 탑의 최하층(시작 구역)이므로 항상 1로 지정하세요.
4. mandatoryEntities: 퀘스트 진행을 위해 1층에 반드시 배치되어야 하는 주요 NPC나 몬스터를 1명 이상 생성하세요.
   - id: 영문 소문자 스네이크 케이스 (예: 'mechanic_vargas')
   - type: 'NPC' 또는 'MONSTER'
   - name: 한글 이름 (예: '기계공 바르가스')
   - symbol: 2D 맵에 렌더링될 2칸 크기의 알맞은 이모지 (예: '🧙', '🛡️', '💀')
   - description: 맵 배치 AI가 방 분위기를 설계할 수 있게 돕는 외형 및 디테일 묘사
   - purposeInQuest: 플레이어가 이 인물을 만나야 하는 이유와 퀘스트 상호작용
5. mandatoryObjects: 퀘스트와 상호작용할 수 있는 고유 오브젝트(조사 장치, 책상, 제단, 상자 등)를 1개 이상 생성하세요.
   - id: 영문 소문자 스네이크 케이스 (예: 'resonance_workbench')
   - name: 한글 이름 (예: '기계식 조립 작업대')
   - symbol: 알맞은 이모지 기호 (예: '⚙️', '📖', '🏺')
   - description: 물체에 대한 묘사
   - purposeInQuest: 조사 시 발생하는 퀘스트 상호작용 및 획득 정보
   - suggestedComponents: C++ 엔진 컴포넌트 목록 (예: ['Lockpickable', 'Smashable'] 또는 빈 배열 [])

당신은 눈앞에 선 모험가의 정보(이름, 성별, 직업, 스탯 등)를 꿰뚫어 보고 있으므로, 제공된 정보를 최대한 서사에 녹여내어 독창적인 내러티브를 제공하세요.

사용자의 이름은: {{name}} 입니다.
사용자의 성별은: {{gender}} 입니다.
사용자의 직업은: {{class}} 입니다.
사용자의 직업에 대한 설명은 {{classDesc}} 입니다.
사용자의 스탯은 {{stat}} 입니다.
)";
                        
                        _prompt = TTOT::Utilities::FormatString(_prompt, "{{name}}", scene.introContext.userName);
                        _prompt = TTOT::Utilities::FormatString(_prompt, "{{gender}}", scene.introContext.userGender ? "여자" : "남자");
                        _prompt = TTOT::Utilities::FormatString(_prompt, "{{class}}", scene.introContext.customClass);
                        _prompt = TTOT::Utilities::FormatString(_prompt, "{{classDesc}}", scene.introContext.customClassDesc);
                        _prompt = TTOT::Utilities::FormatString(_prompt, "{{stat}}", scene.introContext.tempStat);
            

            nlohmann::json body=
            {
                {"contents", nlohmann::json::array({
                    {
                        {"parts", nlohmann::json::array({
                            {{"text", std::string("플레이어가 탑으로 들어온 목적 ") + scene.introContext.adventurePurpose}}
                        })}
                    }
                })},
                {"systemInstruction", {
                    {"parts", nlohmann::json::array({
                        {
                            {
                                "text", _prompt
                                }
                            }
                        }
                    )}
                }},
                {"generationConfig", {
                    {"responseMimeType", "application/json"},
                    {"responseSchema", schemaJson},
                }}
            };
            std::string responseStr = scene.context.gemini.Request(body);
            return responseStr;
        });
        std::thread([this, &scene]()
        {
            while(scene.introStep == IntroStep::ProcessingWritePurpose)
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(50));
                scene.context.eventBus.Publish(TTOT::Core::Events::ScreenRefreshEvent{});
            }
        }).detach();
    }
    void ProcessingWritePurposeState::OnExit(IntroScene& scene)
    {
    }
    void ProcessingWritePurposeState::Update(IntroScene& scene)
    {
        auto now = std::chrono::steady_clock::now();
        double elapsedTime = std::chrono::duration<double>(now - lastUpdateTime).count();
        if(elapsedTime >= 0.1)
        {
            lastUpdateTime = now;
            spinnerIndex = (spinnerIndex + 1) % scene.introContext.spinner.size();
            scene.context.eventBus.Publish(TTOT::Core::Events::ScreenRefreshEvent{});
            
        }
        if(scene.introContext.geminiFuture.valid() && scene.introContext.geminiFuture.wait_for(std::chrono::seconds(0)) == std::future_status::ready)
        {
            std::string result = scene.introContext.geminiFuture.get();
            try
            {
                auto parsedJson = nlohmann::json::parse(result);
                scene.introContext.guidance = parsedJson["guidanceText"].get<std::string>();
                scene.introContext.questDataJson = result;
            }
            catch(const std::exception& e)
            {
                scene.introContext.customClass = "⚠️ 클래스 데이터 파싱 실패";
                scene.introContext.customClassDesc = "원본 응답:\n" + result + "\n\n오류 내용:\n" + e.what();
            }
            scene.ChangeState(IntroStep::Proposal);
            scene.context.eventBus.Publish(TTOT::Core::Events::ScreenRefreshEvent{});
        }
    }
    void ProcessingWritePurposeState::HandleInput(IntroScene& scene, const Event& event)
    {
    }
    Element ProcessingWritePurposeState::Render(IntroScene& scene)
    {
        return hbox({
                text("⚙ 탑의 여신이 당신의 말을 듣고 조언을 생각 중입니다..."),
                text(scene.introContext.spinner[spinnerIndex])
            });
    }
}