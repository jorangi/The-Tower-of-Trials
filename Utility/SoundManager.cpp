#define MINIAUDIO_IMPLEMENTATION
#include "Utility/SoundManager.h"
#include <string>
#include <iostream>
#include <filesystem>

namespace TTOT::Utilities
{
    SoundManager::SoundManager()
    {
        if(ma_engine_init(NULL, &s_engine) != MA_SUCCESS)
        {
            std::cerr << "[SoundManager] 엔진 초기화 실패!" << std::endl;
        }
        namespace fs = std::filesystem;
        if(!fs::exists("Assets/Sounds") || !fs::is_directory("Assets/Sounds"))
        {
            std::cerr << "[SoundManager] Assets/Sounds 폴더가 존재하지 않습니다!" << std::endl;
        }
        std::cout << "[SoundManager] 사운드 파일 등록 시작" << std::endl;
        int count = 0;
        for(const auto& entry : fs::recursive_directory_iterator("Assets/Sounds"))
        {
            if(!entry.is_regular_file()) continue;
            fs::path filePath = entry.path();
            std::string parentDirName = filePath.parent_path().filename().string();
            std::string fileName = filePath.stem().string();
            std::string generatedKey = parentDirName + "_" + fileName;
            std::string fullPath = filePath.string();
            bool isLoop = (parentDirName == "BGM");
            RegisterSound(generatedKey, fullPath, isLoop);
            count++;
        }
        std::cout << "[SoundManager] 초기화 완료. (등록된 사운드: " << count << ")" <<  std::endl;
    }
    SoundManager::~SoundManager()
    {
        for(auto& pair : sfxPoolMap)
        {
            for(auto& sfx : pair.second)
            {
                ma_sound_uninit(sfx.get());
            }
        }
        for(auto& pair : bgmMap)
        {
            ma_sound_uninit(pair.second.get());
        }
        bgmMap.clear();
        ma_engine_uninit(&s_engine);
    }
    void SoundManager::RegisterSound(const std::string& key, const std::string& filePath, bool isLoop)
    {
        if(isLoop)
        {
            auto sound = std::make_unique<ma_sound>();
            ma_result result = ma_sound_init_from_file(&s_engine, filePath.c_str(), 0, NULL, NULL, sound.get());
            if(result == MA_SUCCESS)
            {
                ma_sound_set_looping(sound.get(), MA_TRUE);
                bgmMap[key] = std::move(sound);
                std::cout << "[SoundManager] BGM 등록 성공 : " << key << " (" << filePath << ")" << std::endl;
            }
        }
        else
        {
            int count = 0;
            for(int i = 0; i < 5; i++)
            {
                auto sound = std::make_unique<ma_sound>();
                ma_result cloneResult = ma_sound_init_from_file(&s_engine, filePath.c_str(), 0, NULL, NULL, sound.get());
                if(cloneResult == MA_SUCCESS)
                {
                    sfxPoolMap[key].push_back(std::move(sound));
                    count++;
                }
            }
            std::cout << "[SoundManager] 등록 : " << key << " (" << filePath << ")" << "( 성공: " << count << ", 실패 : " << 5 - count << ")" << std::endl;
        }
    }
    void SoundManager::PlayBGM(const std::string& key)
    {
        auto it = bgmMap.find(key);
        if(it == bgmMap.end())
        {
            std::cerr << "[SoundManager] " << key << " 음악이 존재하지 않습니다." << std::endl;
            return;
        }
        if(currentBGM != nullptr)
        {
            ma_sound_stop(currentBGM);
            ma_sound_seek_to_pcm_frame(currentBGM, 0);
        }
        currentBGM = it->second.get();
        ma_sound_set_volume(currentBGM, masterVolume * bgmVolume);
        ma_sound_start(currentBGM);
    }
    void SoundManager::StopBGM()
    {
        if(currentBGM != nullptr)
        {
            ma_sound_stop(currentBGM);
            ma_sound_seek_to_pcm_frame(currentBGM, 0);
            currentBGM = nullptr;
        }
    }
    void SoundManager::PlaySFX(const std::string& key)
    {
        auto it = sfxPoolMap.find(key);
        if(it == sfxPoolMap.end())
        {
            std::cerr << "[SoundManager] " << key << " 효과음이 존재하지 않습니다." << std::endl;
            return;
        }
        for(auto& sfx : sfxPoolMap[key])
        {
            if(!ma_sound_is_playing(sfx.get()))
            {
                ma_sound_set_volume(sfx.get(), masterVolume * sfxVolume);
                ma_sound_seek_to_pcm_frame(sfx.get(), 0);
                ma_sound_start(sfx.get());
                return;
            }
        }
        auto& fallbackSFX = sfxPoolMap[key][0];
        ma_sound_set_volume(fallbackSFX.get(), masterVolume * sfxVolume);
        ma_sound_seek_to_pcm_frame(fallbackSFX.get(), 0);
        ma_sound_start(fallbackSFX.get());
    }
    void SoundManager::SetMasterVolume(float volume)
    {
        masterVolume = volume;
        ma_engine_set_volume(&s_engine, masterVolume);
    }
    void SoundManager::SetBGMVolume(float volume)
    {
        bgmVolume = volume;
        if(currentBGM != nullptr)
        {
            ma_sound_set_volume(currentBGM, masterVolume * bgmVolume);
        }
    }
    void SoundManager::SetSFXVolume(float volume)
    {
        sfxVolume = volume;
    }
}