#include "Core/Scene/IntroScene.h"
#include "Core/Event/ScreenRefreshEvent.h"
#include "Core/Scene/IntroSceneState/ProcessingWritePurposeState.h"
#include "Utility/FormatString.h"
#include <string>

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
            std::ifstream schemaFile("Assets/Schema/PurposeSchema.json");
            nlohmann::json schemaJson;
            if(!schemaFile.is_open())
            {
                return std::string("Error: 스키마 파일을 찾을 수 없습니다. 경로를 확인해주세요.");
            }
            schemaFile >> schemaJson;
            schemaFile.close();

            _prompt = R"(
                            당신은 텍스트 RPG '시련의 탑(The Tower of Trials)'의 초입에서 모험가를 심사하고 인도하는 탑의 신비로운 의지(Voice of the Tower)이자 탑의 여신입니다.
                            사용자가 탑에 들어오려는 '목적'을 입력하면, 그 목적을 달성하기 위해 탑 1층(또는 시작 마을)에서 '무엇부터 시작해야 할지' 구체적인 단서나 흥미로운 소문을 넌지시 가르쳐 주어야 합니다.
                            [작성 규칙]
                            1. 사용자의 목적을 적극적으로 반영하되, 맹목적으로 똑같은 단어만 반복하지 말고 세계관에 맞게 고급스럽고 세련되게 재해석하세요.
                            2. 1층의 구체적인 장소, 인물, 혹은 기묘한 오브젝트를 임의로 창작하여 언급하세요. 
                            (예: 1층 선술집의 늙은 정보상, 마을 광장의 부서진 여신상, 1층 구석의 버려진 제단, 촌장 알렉스 등)
                            3. 말투는 몽환적이고, 신비로우며, 약간은 묵직한 어조(~라더군요, ~해 보십시오, ~의 이야기가 있습니다 등)를 유지하세요.
                            4. 중요: 터미널 UI 화면의 크기 제한이 있으므로, 불필요한 서론이나 인사는 모두 생략하고 오직 본론만 '2~3문장 이내(한 단락)'로 짧고 강렬하게 답변하세요.
                            [답변 예시]
                            - 목적: "돈을 벌기 위해"
                            -> "부의 냄새를 쫓아 이곳까지 흘러들어왔군요. 그렇다면 탑 1층 광장에 있는 늙은 상인 NPC 피에르를 찾아가 보십시오. 그가 최근 지하 수로에서 발견된 황금 주화에 대한 은밀한 의뢰를 숨기고 있다더군요."
                            - 목적: "잃어버린 동생을 찾으러"
                            -> "혈육의 유대를 끊지 못하고 시련의 궤도에 오르셨군요. 1층 주막의 주모가 얼마 전 당신의 동생과 닮은 인물이 탑 상층부로 향하는 전송진에 올랐다는 이야기를 들었습니다. 우선 주막으로 가 그녀의 기억을 캐내어 보십시오."
                            --
                            당신은 전능한 신이므로, 눈앞에 선 모험가의 이름, 성별, 직업, 그리고 그 자의 영혼에 새겨진 비극적인 과거 서사를 모두 꿰뚫어 보고 있습니다.

                            사용자의 이름은: {{name}} 입니다.
                            사용자의 성별은: {{gender}} 입니다.
                            사용자의 직업은: {{class}} 입니다.
                            사용자의 직업에 대한 설명은 {{classDesc}} 입니다.
                            사용자의 스탯은 {{stat}} 입니다.

                            사용자가 앞으로 서술할 탑을 오르는 목적과, 사용자의 전체 정보에 대한 내용을 바탕으로 답변을 작성하세요.
                            제공된 정보를 최대한 활용하고 서사에 녹여내어 단순히 누구에게나 적용할 수 있는 서사가 아니라, 사용자만을 위한 내러티브한 서사를 작성하세요.
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