cmake_minimum_required(VERSION 3.14)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(VCPKG_TARGET_TRIPLET "x64-mingw-dynamic" CACHE STRING "")

find_program(CCACHE_PROGRAM ccache)
if(CCACHE_PROGRAM)
    set(CMAKE_CXX_COMPILER_LAUNCHER ${CCACHE_PROGRAM})
endif()

project(TowerOfTrials)

add_compile_definitions(NOMINMAX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(cpr CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(ftxui REQUIRED)

aux_source_directory(Core CORE_SRC)
aux_source_directory(Core/Scene SCENE_SRC)
aux_source_directory(Core/Scene/IntroSceneState INTRO_STATE_SRC)
aux_source_directory(Network NETWORK_SRC)
aux_source_directory(Datas DATAS_SRC)
aux_source_directory(Class CLASS_SRC)
aux_source_directory(Entity ENTITY_SRC)
aux_source_directory(Stat STAT_SRC)
aux_source_directory(Component COMPONENT_SRC)
aux_source_directory(Utility UTILITY_SRC)
aux_source_directory(UI UI_SRC)
aux_source_directory(Test TEST_SRC)

add_library(TTOTLibs STATIC
    ${CORE_SRC}
    ${SCENE_SRC}
    ${INTRO_STATE_SRC}
    ${NETWORK_SRC}
    ${DATAS_SRC}
    ${CLASS_SRC}
    ${ENTITY_SRC}
    ${STAT_SRC}
    ${COMPONENT_SRC}
    ${UTILITY_SRC}
    ${UI_SRC}
    )

target_link_libraries(TTOTLibs PUBLIC 
    cpr::cpr 
    nlohmann_json::nlohmann_json 
    ftxui::screen
    ftxui::dom
    ftxui::component
)
target_include_directories(TTOTLibs PUBLIC ${_SOURCE_DIR}/include)

add_executable(Test Test/SoundManagerTest.cpp)
target_link_libraries(Test PRIVATE TTOTLibs)

add_executable(TheTowerOfTrials main.cpp)
target_link_libraries(TheTowerOfTrials PRIVATE TTOTLibs)

foreach(target TheTowerOfTrials Test TTOTLibs)
    target_include_directories(${target} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
endforeach()

file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/Assets"
    DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")