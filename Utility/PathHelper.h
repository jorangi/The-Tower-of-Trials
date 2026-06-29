#pragma once
#include <filesystem>

#ifdef _WIN32
#include <windows.h>
#undef RGB
#endif

namespace TTOT::Utilities {

inline std::filesystem::path GetAssetPath(const std::filesystem::path& relativePath) {
    namespace fs = std::filesystem;
    
    // 1. Try relative to CWD (Current Working Directory)
    if (fs::exists(relativePath)) {
        return relativePath;
    }
    
    // 2. Get Exe Directory
#ifdef _WIN32
    wchar_t buffer[MAX_PATH];
    GetModuleFileNameW(NULL, buffer, MAX_PATH);
    fs::path exeDir = fs::path(buffer).parent_path();
#else
    fs::path exeDir = fs::current_path();
#endif

    // Try relative to Exe
    fs::path path1 = exeDir / relativePath;
    if (fs::exists(path1)) {
        return path1;
    }
    
    // Try relative to Exe's parent (for running from build directory while assets are in root)
    fs::path path2 = exeDir.parent_path() / relativePath;
    if (fs::exists(path2)) {
        return path2;
    }
    
    return relativePath; // Fallback
}

inline std::filesystem::path GetSavePath(const std::string& filename) {
    namespace fs = std::filesystem;
#ifdef _WIN32
    wchar_t buffer[MAX_PATH];
    GetModuleFileNameW(NULL, buffer, MAX_PATH);
    fs::path exeDir = fs::path(buffer).parent_path();
#else
    fs::path exeDir = fs::current_path();
#endif
    return exeDir / filename;
}

} // namespace TTOT::Utilities
