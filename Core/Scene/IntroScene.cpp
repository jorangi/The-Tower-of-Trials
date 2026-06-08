#include "Core/Scene/IntroScene.h"
#include "Core/Event/SceneChangeEvent.h"
#include "Core/Event/ScreenRefreshEvent.h"
#include <fstream>
#include "Class/ClassBase.h"

using namespace ftxui;
using namespace TTOT::Core::Scenes;

namespace TTOT::Core::Scenes
{
    IntroScene::IntroScene(TTOT::Core::GameContext& context) : SceneBase(context), statPoints(7, 0)
    {
        input = ftxui::Input(&typedBuffer, "이름을 입력해주세요...");
        input->TakeFocus();
    }
    void IntroScene::OnEnter()
    {
        //TODO: 인트로용 BGM 재생
    }
    void IntroScene::OnExit()
    {
        //TODO: 작성된 데이터를 저장하여 보관할 것
    }
    void IntroScene::Update()
    {
        if(introStep == IntroStep::ProcessingWriteClass)
        {
            auto now = std::chrono::steady_clock::now();
            double elapsedTime = std::chrono::duration<double>(now - lastUpdateTime).count();
            if(elapsedTime >= 0.1)
            {
                lastUpdateTime = now;
                spinnerIndex = (spinnerIndex + 1) % spinner.size();
                context.eventBus.Publish(TTOT::Core::Events::ScreenRefreshEvent{});
                
            }
            if(geminiFuture.valid() && geminiFuture.wait_for(std::chrono::seconds(0)) == std::future_status::ready)
            {
                std::string result = geminiFuture.get();
                try
                {
                    auto parsedJson = nlohmann::json::parse(result);
                    auto customJob = std::make_unique<TTOT::Class::ClassBase>();
                    *customJob = parsedJson.get<TTOT::Class::ClassBase>();
                    customClass = customJob->GetClassName();
                    customClassDesc = customJob->GetClassDesc();
                }
                catch(const std::exception& e)
                {
                    customClass = "⚠️ 클래스 데이터 파싱 실패";
                    customClassDesc = "원본 응답:\n" + result + "\n\n오류 내용:\n" + e.what();
                }
                introStep = IntroStep::ConfirmClass;
                selectedIndex = 0;
                context.eventBus.Publish(TTOT::Core::Events::ScreenRefreshEvent{});
            }
        }
    }
    void IntroScene::HandleInput(const ftxui::Event& event)
    {
        //esc
        if(event == ftxui::Event::Escape)
        {
            if(introStep == IntroStep::InputName)
            {
                context.eventBus.Publish(TTOT::Core::Events::SceneChangeEvent{0});
                return;
            }
            else if(introStep == IntroStep::ConfirmName)
            {
                introStep = IntroStep::InputName;
                userName = "";
                typedBuffer = "";
            }
            else if(introStep == IntroStep::SelectGender)
            {
                introStep = IntroStep::InputName;
                userName = "";
                typedBuffer = "";
                userGender = false;
            }
            else if(introStep == IntroStep::personalDataConfirm)
            {
                introStep = IntroStep::SelectGender;
                userGender = false;
            }
            else if(introStep == IntroStep::CustomClass)
            {
                introStep = IntroStep::personalDataConfirm;
            }
            else if(introStep == IntroStep::InputCustomClass)
            {
                introStep = IntroStep::CustomClass;
                typedBuffer = "";
                customClass = "";
                customClassDesc = "";
            }
            else if(introStep == IntroStep::SelectClass || introStep == IntroStep::ConfirmClass || introStep == IntroStep::StatPoint)
            {
                introStep = IntroStep::CustomClass;
                customClass = "";
                customClassDesc = "";
            }
            else if(introStep == IntroStep::ConfirmStats)
            {
                introStep = IntroStep::StatPoint;
            }
            else if(introStep == IntroStep::finalConfirm)
            {
                introStep = IntroStep::amendData;
            }
            else if(introStep == IntroStep::amendData)
            {
                introStep = IntroStep::finalConfirm;
            }
        }

        //타자 입력
        if(introStep == IntroStep::InputName)
        {
            if(event == ftxui::Event::Return)
            {
                if(!typedBuffer.empty())
                {
                    userName = typedBuffer;
                    if(prevStep == IntroStep::ConfirmName) savedName[userName]++;
                    introStep = IntroStep::ConfirmName;
                    selectedIndex = 0;
                }
                return;
            }
            input->OnEvent(event);
            return;
        }
        else if(introStep == IntroStep::InputCustomClass)
        {
            if(event == ftxui::Event::Return)
            {
                if(!typedBuffer.empty())
                {
                    customClass = typedBuffer;
                    introStep = IntroStep::ProcessingWriteClass;
                    lastUpdateTime = std::chrono::steady_clock::now();
                    geminiFuture = std::async(std::launch::async, [this]()
                    {
                        std::ifstream schemaFile("Assets/Schema/ClassSchema.json");
                        nlohmann::json schemaJson;
                        if(!schemaFile.is_open())
                        {
                            return std::string("Error: 스키마 파일을 찾을 수 없습니다. 경로를 확인해주세요.");
                        }
                        schemaFile >> schemaJson;
                        schemaFile.close();
                        nlohmann::json body=
                        {
                            {"contents", nlohmann::json::array({
                                {
                                    {"parts", nlohmann::json::array({
                                        {{"text", std::string("사용자가 요청하는 커스텀 클래스의 정보: ") + customClass}}
                                    })}
                                }
                            })},
                            {"systemInstruction", {
                                {"parts", nlohmann::json::array({
                                    {{"text", std::string("다음 입력된 정보를 바탕으로 직업(클래스)를 만들고 서술하시오.\n") +
                                            "예시) 전사: 단단한 신체와 강력한 힘으로 전선의 선봉에 서는 자. (따라서 체력과 힘, 방어력에 보정과 기본값을 높게 책정. 반면 지능이나 마나 등은 약점으로 보정과 기본값을 낮게 책정.)/\n" +
                                            "도적: 어둠 속에 숨어 적의 허점을 찌르고 유유히 사라지는 암살자.(따라서 민첩과 스피드 등에 보정과 기본값을 높게...)\n" +
                                            "직업의 HP,MP,STR,DEX,INT,WIS,CHA,DEF,SPD의 합산은 20 이하의 자연수여야 합니다.\n" +
                                            "직업의 보정치는 게임 전반적인 밸런스를 고려하여 특화형 혹은 육각형밸런스형을 고려하여 배정하여야 합니다.\n" +
                                            "또한 답변 시 직업의 이름과 특징(배경 등)을 서술하시오."}}
                                })}
                            }},
                            {"generationConfig", {
                                {"responseMimeType", "application/json"},
                                {"responseSchema", schemaJson},
                            }}
                        };
                        std::string responseStr = context.gemini.Request(body);
                        std::ofstream debugFile("gemini_response_debug.json");
                        if (debugFile.is_open())
                        {
                            debugFile << responseStr;
                            debugFile.close();
                        }
                        return responseStr;
                    });
                    std::thread([this]()
                    {
                        while(introStep == IntroStep::ProcessingWriteClass)
                        {
                            std::this_thread::sleep_for(std::chrono::milliseconds(50));
                            context.eventBus.Publish(TTOT::Core::Events::ScreenRefreshEvent{});
                        }
                    }).detach();
                }
                return;
            }
            input->OnEvent(event);
            return;
        }
        else if(introStep == IntroStep::Purpose)
        {
            if(event == ftxui::Event::Return)
            {
                if(!typedBuffer.empty())
                {
                    adventurePurpose = typedBuffer;
                    introStep = IntroStep::ProcessingWriteClass;
                    lastUpdateTime = std::chrono::steady_clock::now();
                    // geminiFuture = std::async(std::launch::async, [this]()
                    // {
                    //     return context.gemini.Request(std::string("다음 입력된 정보를 바탕으로 직업(클래스)를 만들고 서술하시오. 예시) 전사: 단단한 신체와 강력한 힘으로 전선의 선봉에 서는 자./도적: 어둠 속에 숨어 적의 허점을 찌르고 유유히 사라지는 암살자. 또한 답변 시 직업의 이름과 특징(배경 등)을 서술하시오.\n--\n사용자의 프롬프트 : ")+ std::string(customClass));
                    // });
                    // std::thread([this]()
                    // {
                    //     while(introStep == IntroStep::ProcessingWriteClass)
                    //     {
                    //         std::this_thread::sleep_for(std::chrono::milliseconds(50));
                    //         context.eventBus.Publish(TTOT::Core::Events::ScreenRefreshEvent{});
                    //     }
                    // }).detach();
                }
                return;
            }
            input->OnEvent(event);
            return;
        }
        
        //방향키(wasd)
        if((introStep == IntroStep::ConfirmName || introStep == IntroStep::SelectGender || 
            introStep == IntroStep::personalDataConfirm || introStep == IntroStep::CustomClass || introStep == IntroStep::ConfirmClass ||introStep == IntroStep::ConfirmStats || introStep == IntroStep::finalConfirm) &&
            (event == ftxui::Event::ArrowLeft || event == ftxui::Event::Character('a') || event == ftxui::Event::ArrowRight || event == ftxui::Event::Character('d')))
        {
            selectedIndex = (selectedIndex + 1) % 2;
        }
        else if(introStep == IntroStep::SelectClass)
        {
            if(event == ftxui::Event::ArrowUp || event == ftxui::Event::Character('w'))    selectedIndex = (selectedIndex - 1 + 5) % 5;
            if(event == ftxui::Event::ArrowDown || event == ftxui::Event::Character('s'))  selectedIndex = (selectedIndex + 1) % 5;
        }
        else if(introStep == IntroStep::StatPoint)
        {
            if(event == ftxui::Event::ArrowUp || event == ftxui::Event::Character('w'))    selectedIndex = (selectedIndex - 1 + 7) % 7;
            if(event == ftxui::Event::ArrowDown || event == ftxui::Event::Character('s'))  selectedIndex = (selectedIndex + 1) % 7;
            if((event == ftxui::Event::ArrowLeft || event == ftxui::Event::Character('a')) && statPoints[selectedIndex] > 0)
            {
                statPoint++;
                statPoints[selectedIndex]--;
            }
            if((event == ftxui::Event::ArrowRight || event == ftxui::Event::Character('d')) && statPoint > 0)
            {
                statPoint--;
                statPoints[selectedIndex]++;
            }
        }
        else if(introStep == IntroStep::amendData)
        {
            if(event == ftxui::Event::ArrowUp || event == ftxui::Event::Character('w'))    selectedIndex = (selectedIndex - 1 + 4) % 4;
            if(event == ftxui::Event::ArrowDown || event == ftxui::Event::Character('s'))  selectedIndex = (selectedIndex + 1) % 4;
            if(event == ftxui::Event::Character('1'))
            {
                introStep = IntroStep::InputName;
                userName = "";
                typedBuffer = "";
                selectedIndex = 0;
                input->TakeFocus();
                return;
            }
            else if(event == ftxui::Event::Character('2'))
            {
                introStep = IntroStep::SelectGender;
                userGender = false;
                selectedIndex = 0;
                return;
            }
            else if(event == ftxui::Event::Character('3'))
            {
                introStep = IntroStep::CustomClass;
                selectedIndex = 0;
                return;
            }
            else if(event == ftxui::Event::Character('4'))
            {
                introStep = IntroStep::StatPoint;
                selectedIndex = 0;
                return;
            }
        }
        
        //Enter
        if(event == ftxui::Event::Return)
        {
            prevStep = introStep;
            if(introStep == IntroStep::ConfirmName)
            {
                if(selectedIndex == 0)
                {
                    introStep = IntroStep::SelectGender;
                }
                else
                {
                    typedBuffer = "";
                    userName = "";
                    introStep = IntroStep::InputName;
                }
            }
            else if(introStep == IntroStep::SelectGender)
            {
                if(selectedIndex == 0)
                {
                    userGender = false;
                    introStep = IntroStep::personalDataConfirm;
                }
                else
                {
                    userGender = true;
                    introStep = IntroStep::personalDataConfirm;
                }
            }
            else if(introStep == IntroStep::personalDataConfirm)
            {
                if(selectedIndex == 0)
                {
                    typedBuffer = "";
                    introStep = IntroStep::CustomClass;
                    selectedIndex = 0;
                }
                else
                {
                    introStep = IntroStep::InputName;
                    userName = "";
                    typedBuffer = "";
                    userGender = false;
                }
            }
            else if(introStep == IntroStep::CustomClass)
            {
                if(selectedIndex == 0)
                {
                    typedBuffer = "";
                    introStep = IntroStep::InputCustomClass;
                    input = ftxui::Input(&typedBuffer, "원하는 직업의 이름과 특징을 입력하세요...");
                    input->TakeFocus();
                }
                else
                {
                    introStep = IntroStep::SelectClass;
                }
                selectedIndex = 0;
            }
            else if(introStep == IntroStep::SelectClass)
            {
                className = basicClass[selectedIndex];
                classDesc = basicClassDesc[selectedIndex];
                selectedIndex = 0;
                introStep = IntroStep::StatPoint;
            }
            else if(introStep == IntroStep::ConfirmClass)
            {
                if(selectedIndex == 0)
                {
                    introStep = IntroStep::StatPoint;
                    selectedIndex = 0;
                }
                else if(selectedIndex == 1)
                {
                    introStep = IntroStep::CustomClass;
                    selectedIndex = 0;
                }
            }
            else if(introStep == IntroStep::StatPoint)
            {
                if(statPoint > 0) return;
                selectedIndex = 0;
                introStep = IntroStep::ConfirmStats;
            }
            else if(introStep == IntroStep::ConfirmStats)
            {
                if(selectedIndex == 0)
                {
                    introStep = IntroStep::finalConfirm;
                }
                else if(selectedIndex == 1)
                {
                    introStep = IntroStep::StatPoint;

                }
                selectedIndex = 0;
            }
            else if(introStep == IntroStep::finalConfirm)
            {
                if(selectedIndex == 0)
                {
                    introStep = IntroStep::Purpose;
                }
                else if(selectedIndex == 1)
                {
                    introStep = IntroStep::amendData;
                }
                selectedIndex = 0;
            }
            else if(introStep == IntroStep::amendData)
            {
                if(selectedIndex == 0)
                {
                    introStep = IntroStep::InputName;
                    userName = "";
                    typedBuffer = "";
                    selectedIndex = 0;
                    input->TakeFocus();
                    return;
                }
                else if(selectedIndex == 1)
                {
                    introStep = IntroStep::SelectGender;
                    userGender = false;
                    selectedIndex = 0;
                    return;
                }
                else if(selectedIndex == 2)
                {
                    introStep = IntroStep::CustomClass;
                    selectedIndex = 0;
                    return;
                }
                else if(selectedIndex == 3)
                {
                    introStep = IntroStep::StatPoint;
                    selectedIndex = 0;
                    return;
                }
            }
        }
    }
    Element IntroScene::Render()
    {
        Elements content;

        content.push_back(text("✨새로운 여정의 시작✨") | hcenter | bold);
        content.push_back(separator());

        if(introStep == IntroStep::InputName)
        {
            content.push_back(hbox({text("당신의 이름은 무엇입니까?: "), input->Render() | color(Color::Cyan) | underlined}));
        }
        if(introStep==IntroStep::ConfirmName)
        {
            if(savedName[userName] > 6)
            {
                introStep = IntroStep::SelectGender;
            }
            else
            {
                content.push_back(separator());
                if(savedName[userName] < 2)
                {
                    content.push_back(text("▶ '" + userName + "' 이(가) 당신의 이름이 맞습니까?"));
                }
                else if(savedName[userName] == 2)
                {
                    content.push_back(text("▶ " + userName +" 맞는거죠?"));
                }
                else if(savedName[userName] == 3)
                {
                    content.push_back(text("▶ ...? 아까 " + userName + "은 아니라고 하지 않았어요? 본인 이름 맞아요?"));
                }
                else if(savedName[userName] == 4)
                {
                    content.push_back(text("▶ 장난하는거에요? " + userName + "이 이름 맞아요?"));
                }
                else if(savedName[userName] == 5)
                {
                    content.push_back(text("▶ 재미 없거든요... " + userName + " 확실한거죠?"));
                }
                else if(savedName[userName] == 6)
                {
                    content.push_back(text("▶ 이제 더 이상 안 받아줄거에요.. " + userName + " 맞는지 마지막으로 확인합니다."));
                }
                content.push_back(hbox({text("네") | (selectedIndex == 0 ? bold : nothing),
                                            text(" / "),
                                            text("아니오") | (selectedIndex == 1 ? bold : nothing)
                }));
            } 
        }
        if(introStep==IntroStep::SelectGender)
        {
            content.push_back(separator());
            content.push_back(text("당신의 성별은 무엇입니까?"));
            content.push_back(hbox({text("남자") | (selectedIndex == 0 ? bold : nothing),
                text(" / "),
                text("여자") | (selectedIndex == 1 ? bold : nothing)
            }));
        }
        if(introStep == IntroStep::personalDataConfirm)
        {
            content.push_back(hbox({text(std::string("당신의 이름은")),
                                    text(userName) | color(Color::Green),
                                    text(std::string("입니다. \n 당신의 성별은")),
                                    text(userGender ? "여자" : "남자") | color(Color::Green),
                                    text(std::string("입니다."))
            }));
            content.push_back(separator());
            content.push_back(hbox({
                    text("예") | (selectedIndex == 0 ? bold : nothing),
                    text(" / "),
                    text("아니오") | (selectedIndex == 1 ? bold : nothing)
                }));
        }
        if(introStep == IntroStep::CustomClass)
        {
            content.push_back(vbox({text("사용자 정의 직업(클래스)를 사용하시겠습니까?"), 
                hbox({text("네") | (selectedIndex == 0 ? bold : nothing),
                            text(" / "),
                            text("아니오") | (selectedIndex == 1 ? bold : nothing)
                })
            }));
        }
        if(introStep == IntroStep::SelectClass)
        {
            content.push_back(vbox({
                text("클래스를 선택해주세요."),
                vbox({
                    text(basicClass[0] + ": " + basicClassDesc[0]) | color(Color::Red) | (selectedIndex == 0 ? bold : nothing),
                    text(basicClass[1] + ": " + basicClassDesc[1]) | color(Color::NavyBlue) | (selectedIndex == 1 ? bold : nothing),
                    text(basicClass[2] + ": " + basicClassDesc[2]) | color(Color::Green) | (selectedIndex == 2 ? bold : nothing),
                    text(basicClass[3] + ": " + basicClassDesc[3]) | color(Color::Purple) | (selectedIndex == 3 ? bold : nothing),
                    text(basicClass[4] + ": " + basicClassDesc[4]) | color(Color::Gold1) | (selectedIndex == 4 ? bold : nothing)
                })
            }));
        }
        if(introStep == IntroStep::InputCustomClass)
        {
            content.push_back(vbox({
                hbox({text("사용자 정의 클래스를 입력해주세요. 직업의 특징도 같이 입력한다면 완성도가 올라갈 수 있습니다."), text("예시) 전사: 단단한 신체와 강력한 힘으로 전선의 선봉에 서는 자.") | italic}),
                input->Render() | color(Color::Cyan) | underlined,
            }));
        }
        if(introStep == IntroStep::ProcessingWriteClass)
        {
            content.push_back(
                hbox({
                    text("⚙ 설정에 맞는 사용자 정의 클래스를 생성 중입니다..."), 
                    text(spinner[spinnerIndex])
                })
            );
        }
        if(introStep == IntroStep::ConfirmClass)
        {
            content.push_back(
            {
                vbox({
                    text("사용자 정의 클래스") | bold | color(Color::Yellow),
                    separator(),
                    text(customClass) | bold,
                    paragraph(customClassDesc) | size(WIDTH, LESS_THAN, 80) | color(Color::GrayLight),
                    separator(),
                    text("해당 클래스로 결정하시겠습니까?"),
                    hbox({
                        text("네") | (selectedIndex == 0 ? bold : nothing),
                        text(" / "), 
                        text("아니오") | (selectedIndex == 1 ? bold : nothing)
                    })
                })
            });
        }
        if(introStep == IntroStep::StatPoint)
        {
            std::string strBar = "";
            std::string dexBar = "";
            std::string intBar = "";
            std::string wisBar = "";
            std::string chaBar = "";
            std::string defBar = "";
            std::string spdBar = "";

            for(int i = 0; i < 10; i++)
            {
                strBar += statPoints[0] > i ? "■" : "□";
                dexBar += statPoints[1] > i ? "■" : "□";                
                intBar += statPoints[2] > i ? "■" : "□";            
                wisBar += statPoints[3] > i ? "■" : "□";                
                chaBar += statPoints[4] > i ? "■" : "□";                
                defBar += statPoints[5] > i ? "■" : "□";                
                spdBar += statPoints[6] > i ? "■" : "□";
                
            }
            content.push_back(
                vbox({
                    text(std::string("능력치 포인트 투자 (") + std::to_string(statPoint) + std::string("p)")) | bold,
                    separator(),
                    text("STR: " + strBar + " (" + std::to_string(statPoints[0]) + ")") | (selectedIndex == 0 ? bold : nothing),
                    text("DEX: " + dexBar + " (" + std::to_string(statPoints[1]) + ")") | (selectedIndex == 1 ? bold : nothing),
                    text("INT: " + intBar + " (" + std::to_string(statPoints[2]) + ")") | (selectedIndex == 2 ? bold : nothing),
                    text("WIS: " + wisBar + " (" + std::to_string(statPoints[3]) + ")") | (selectedIndex == 3 ? bold : nothing),
                    text("CHA: " + chaBar + " (" + std::to_string(statPoints[4]) + ")") | (selectedIndex == 4 ? bold : nothing),
                    text("DEF: " + defBar + " (" + std::to_string(statPoints[5]) + ")") | (selectedIndex == 5 ? bold : nothing),
                    text("SPD: " + spdBar + " (" + std::to_string(statPoints[6]) + ")") | (selectedIndex == 6 ? bold : nothing)
                })
            );
        }
        if(introStep == IntroStep::ConfirmStats)
        {
            content.push_back(
                vbox({
                    text("당신의 스탯을 확정하겠습니까?"),
                    separator(),
                    text("STR: " + std::to_string(statPoints[0])),
                    text("DEX: " + std::to_string(statPoints[1])),
                    text("INT: " + std::to_string(statPoints[2])),
                    text("WIS: " + std::to_string(statPoints[3])),
                    text("CHA: " + std::to_string(statPoints[4])),
                    text("DEF: " + std::to_string(statPoints[5])),
                    text("SPD: " + std::to_string(statPoints[6])),
                    hbox({
                        text("네") | (selectedIndex == 0 ? bold : nothing), text(" / "), text("아니오") | (selectedIndex == 1 ? bold : nothing)
                    })
                })
            );
        }
        if(introStep == IntroStep::finalConfirm)
        {
            content.push_back({
                vbox({
                    text("이름: " + userName),
                    text("성별: " + std::string(userGender ? "여자" : "남자")),
                    text("클래스: " + className),
                    text("[스탯]\n"
                        + std::string("STR: ") + std::to_string(statPoints[0]) 
                        + " | DEX: " + std::to_string(statPoints[2]) 
                        + " | INT: " + std::to_string(statPoints[3]) 
                        + " | WIS: " + std::to_string(statPoints[4]) 
                        + " | CHA: " + std::to_string(statPoints[5]) 
                        + " | SPD: " + std::to_string(statPoints[6])),
                separator(),
                    text("이 정보가 맞습니까?"),
                    hbox({
                        text("네") | (selectedIndex == 0 ? bold : nothing), text(" / "), text("아니오") | (selectedIndex == 1 ? bold : nothing)
                    })
                })
            });
        }
        if(introStep == IntroStep::amendData)
        {
            content.push_back(
            {
                vbox({
                    text("이동을 원하는 번호를 선택하세요."),
                    separator(),
                    text("1. 이름") | color(Color::Aquamarine1) | (selectedIndex == 0 ? bold : nothing),
                    text("2. 성별") | color(Color::Yellow) | (selectedIndex == 1 ? bold : nothing),
                    text("3. 클래스") | color(Color::Violet) | (selectedIndex == 2 ? bold : nothing),
                    text("4. 스탯") | color(Color::Orchid1) | (selectedIndex == 3 ? bold : nothing)
                })
            });
        }
        if(introStep == IntroStep::Purpose)
        {
            content.push_back(
                vbox({
                    context.imageRenderer.ImageRender("Assets/Images/IntroImage.png"),
                    text("\n\n"),
                    paragraph("이곳은 세상 모든 이들의 갈망을 먹고 자라나는 거대한 탑.\n진정으로 무언가를 원하는 자에게만 탑은 문을 열어줍니다."),
                    paragraph("당신이 탑을 찾아온 이유는 무엇인가요?") | bold,
                    separator(),
                    input->Render() | color(Color::Cyan) | underlined,
                })
            );
        }
        return vbox(std::move(content)) | border;
    }
}