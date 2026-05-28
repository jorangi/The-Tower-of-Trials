#include "Utility/SoundManager.h"
#include <cassert>
#include <iostream>
#include <windows.h>

using namespace std;
using TTOT::Utilities::SoundManager;

void InitSoundManagerTest()
{
    cout << "============================================" << endl;
    cout << "        SoundManager 초기화 테스트            " << endl;
    cout << "============================================" << endl;
    SoundManager& sm = SoundManager::Inst();
    Sleep(1000); // 1초간 대기
}
void PlayBGMTest()
{
    cout << "============================================" << endl;
    cout << "        SoundManager BGM 재생 테스트          " << endl;
    cout << "============================================" << endl;
    SoundManager& sm = SoundManager::Inst();
    sm.PlayBGM("BGM_Title");
    Sleep(5000); // 3초간 대기
}
void PlaySFXTest()
{
    cout << "============================================" << endl;
    cout << "        SoundManager SFX 재생 테스트          " << endl;
    cout << "============================================" << endl;
    SoundManager& sm = SoundManager::Inst();
    for(int i = 0; i < 3; i++)
    {
        sm.PlaySFX("SFX_Hit");
        Sleep(300); // 0.3초간 대기
    }
    Sleep(1000); // 1초간 대기
}
void VolumeControlTest()
{
    cout << "============================================" << endl;
    cout << "        SoundManager 볼륨 조절 테스트          " << endl;
    cout << "============================================" << endl;
    SoundManager& sm = SoundManager::Inst();
    cout << "마스터 볼륨 50%" << endl;
    sm.SetMasterVolume(0.5f);
    Sleep(2000); // 2초간 대기
    cout << "BGM 볼륨 70%" << endl;
    sm.SetBGMVolume(0.7f);
    Sleep(2000); // 2초간 대기
    cout << "SFX 볼륨 40%" << endl;
    sm.PlaySFX("SFX_Hit");
    sm.SetSFXVolume(0.4f);
    Sleep(5000); // 5초간 대기
    cout << "마스터 볼륨 100%" << endl;
    sm.SetMasterVolume(1.0f);
    sm.PlaySFX("SFX_Hit");
    Sleep(2000); // 2초간 대기
}
void StopBGMTest()
{
    cout << "============================================" << endl;
    cout << "        SoundManager BGM 정지 테스트          " << endl;
    cout << "============================================" << endl;
    SoundManager& sm = SoundManager::Inst();
    sm.StopBGM();
}
void ErrorTest()
{
    cout << "============================================" << endl;
    cout << "        SoundManager 에러 처리 테스트          " << endl;
    cout << "============================================" << endl;
    SoundManager& sm = SoundManager::Inst();
    cout << "존재하지 않는 BGM 재생 시도" << endl;
    sm.PlayBGM("BGM_NonExistent");
}
int main()
{
    InitSoundManagerTest();
    PlayBGMTest();
    PlaySFXTest();
    VolumeControlTest();
    StopBGMTest();
    ErrorTest();
}