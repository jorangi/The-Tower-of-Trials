#include "Utility/SoundManager.h"
#include <cassert>
#include <iostream>
#include <windows.h>
#include <memory>

using namespace std;
using TTOT::Utilities::SoundManager;

unique_ptr<SoundManager> InitSoundManagerTest()
{
    cout << "============================================" << endl;
    cout << "        SoundManager 초기화 테스트            " << endl;
    cout << "============================================" << endl;
    unique_ptr<SoundManager> sm = make_unique<SoundManager>();
    Sleep(1000); // 1초간 대기
    return std::move(sm);
}
void PlayBGMTest(unique_ptr<SoundManager>& sm)
{
    cout << "============================================" << endl;
    cout << "        SoundManager BGM 재생 테스트          " << endl;
    cout << "============================================" << endl;
    sm->PlayBGM("BGM_Title");
    Sleep(5000); // 3초간 대기
}
void PlaySFXTest(unique_ptr<SoundManager>& sm)
{
    cout << "============================================" << endl;
    cout << "        SoundManager SFX 재생 테스트          " << endl;
    cout << "============================================" << endl;
    for(int i = 0; i < 3; i++)
    {
        sm->PlaySFX("SFX_Hit");
        Sleep(50); // 0.05초간 대기
    }
    Sleep(1000); // 1초간 대기
}
void VolumeControlTest(unique_ptr<SoundManager>& sm)
{
    cout << "============================================" << endl;
    cout << "        SoundManager 볼륨 조절 테스트          " << endl;
    cout << "============================================" << endl;
    cout << "마스터 볼륨 50%" << endl;
    sm->SetMasterVolume(0.5f);
    Sleep(2000); // 2초간 대기
    cout << "BGM 볼륨 70%" << endl;
    sm->SetBGMVolume(0.7f);
    Sleep(2000); // 2초간 대기
    cout << "SFX 볼륨 40%" << endl;
    sm->PlaySFX("SFX_Hit");
    sm->SetSFXVolume(0.4f);
    Sleep(5000); // 5초간 대기
    cout << "마스터 볼륨 100%" << endl;
    sm->SetMasterVolume(1.0f);
    sm->PlaySFX("SFX_Hit");
    Sleep(2000); // 2초간 대기
}
void StopBGMTest(unique_ptr<SoundManager>& sm)
{
    cout << "============================================" << endl;
    cout << "        SoundManager BGM 정지 테스트          " << endl;
    cout << "============================================" << endl;
    sm->StopBGM();
}
void ErrorTest(unique_ptr<SoundManager>& sm)
{
    cout << "============================================" << endl;
    cout << "        SoundManager 에러 처리 테스트          " << endl;
    cout << "============================================" << endl;
    cout << "존재하지 않는 BGM 재생 시도" << endl;
    sm->PlayBGM("BGM_NonExistent");
}
int main()
{
    auto sm = InitSoundManagerTest();
    PlayBGMTest(sm);
    PlaySFXTest(sm);
    VolumeControlTest(sm);
    StopBGMTest(sm);
    ErrorTest(sm);
}