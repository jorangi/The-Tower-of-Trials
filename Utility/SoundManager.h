#pragma once
#include "Utility/miniaudio.h"
#include <unordered_map>
#include <string>
#include <iostream>
#include <memory>

namespace TTOT::Utility
{
    class SoundManager
    {
        private:
            ma_engine s_engine;
            std::unordered_map<std::string, std::unique_ptr<ma_sound>> soundMap;
            ma_sound* currentBGM = nullptr;

            float masterVolume = 1.0f;
            float bgmVolume = 1.0f;
            float sfxVolume = 1.0f;
            
            SoundManager();
            SoundManager(const SoundManager&) = delete;
            void operator=(const SoundManager&) = delete;
        public:
            static SoundManager& Inst();
            ~SoundManager();

            void RegisterSound(const std::string& key, const std::string& filePath, bool isLoop = false);
            void PlayBGM(const std::string& key);
            void StopBGM();
            void PlaySFX(const std::string& key);
            
            void SetMasterVolume(float volume);
            void SetBGMVolume(float volume);
            void SetSFXVolume(float volume);
    };
}