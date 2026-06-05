#include "Core/Scene/IntroScene.h"
#include "Core/Event/SceneChangeEvent.h"
#include <chrono>

using namespace ftxui;
using namespace TTOT::Core::Scenes;

namespace TTOT::Core::Scenes
{
    IntroScene::IntroScene(TTOT::Core::GameContext& context) : SceneBase(context) 
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

        }
    }
    void IntroScene::HandleInput(const ftxui::Event& event)
    {
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
        }
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
            }
            input->OnEvent(event);
            return;
        }
        else if(event == ftxui::Event::ArrowLeft || event == ftxui::Event::Character('a') || event == ftxui::Event::ArrowRight || event == ftxui::Event::Character('d'))
        {
            selectedIndex = (selectedIndex + 1) % 2;
        }
        
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
                    //TODO: 설정 저장
                }
                else
                {
                    introStep = IntroStep::InputName;
                    userName = "";
                    typedBuffer = "";
                    userGender = false;
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
        else
        {
            content.push_back(text("이름: " + userName) | color(Color::Green));
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
        if(introStep>=IntroStep::SelectGender)
        {
            if(introStep == IntroStep::SelectGender)
            {
                content.push_back(separator());
                content.push_back(text("당신의 성별은 무엇입니까?"));
                content.push_back(hbox({text("남자") | (selectedIndex == 0 ? bold : nothing),
                    text(" / "),
                    text("여자") | (selectedIndex == 1 ? bold : nothing)
                }));
            }
            else
            {
                content.push_back(text(std::string("성별: ") + (userGender ? "여자" : "남자")) | color(Color::Green));
            }
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
                    text("전사: 단단한 신체와 강력한 힘으로 전선의 선봉에 서는 자.") | color(Color::Red) | (selectedIndex == 0 ? bold : nothing),
                    text("도적: 어둠 속에 숨어 적의 허점을 찌르고 유유히 사라지는 암살자.") | color(Color::NavyBlue) | (selectedIndex == 1 ? bold : nothing),
                    text("궁수: 거리를 유지한 채 치명적인 화살로 적을 숨막히게 하는 저격수.") | color(Color::Green) | (selectedIndex == 2 ? bold : nothing),
                    text("마법사: 세계의 원소를 다루어 광역 파괴를 불러오는 유리 대포.") | color(Color::Purple) | (selectedIndex == 3 ? bold : nothing),
                    text("성직자: 신성한 힘으로 아군을 치유하고 불사의 존재를 정화하는 인도자.") | color(Color::Gold1) | (selectedIndex == 4 ? bold : nothing)
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
                text("설정에 맞는 사용자 정의 클래스를 생성 중입니다.")
            );
        }
        if(introStep == IntroStep::ConfirmClass)
        {
            
        }
        if(introStep == IntroStep::StatPoint)
        {

        }
        if(introStep == IntroStep::ConfirmStats)
        {

        }
        if(introStep == IntroStep::finalConfirm)
        {

        }
        return vbox(std::move(content)) | border;
    }
}