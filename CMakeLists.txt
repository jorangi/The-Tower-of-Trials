cmake_minimum_required(VERSION 3.14)

set(VCPKG_TARGET_TRIPLET "x64-mingw-dynamic" CACHE STRING "")

project(TowerOfTrials)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(cpr CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)

aux_source_directory(Core CORE_SRC)
aux_source_directory(Network NETWORK_SRC)
aux_source_directory(Datas DATAS_SRC)
aux_source_directory(Entity ENTITY_SRC)
aux_source_directory(Stat STAT_SRC)
aux_source_directory(Component COMPONENT_SRC)
aux_source_directory(Utility UTILITY_SRC)
aux_source_directory(Test TEST_SRC)

add_library(TTOTLibs STATIC
    ${CORE_SRC}
    ${NETWORK_SRC}
    ${DATAS_SRC}
    ${ENTITY_SRC}
    ${STAT_SRC}
    ${COMPONENT_SRC}
    ${UTILITY_SRC}
)

target_link_libraries(TTOTLibs PUBLIC cpr::cpr nlohmann_json::nlohmann_json)
target_include_directories(TTOTLibs PUBLIC ${_SOURCE_DIR}/include)

add_executable(Test Test/ConnGeminiTest.cpp)
target_link_libraries(Test PRIVATE TTOTLibs cpr::cpr nlohmann_json::nlohmann_json)

add_executable(TheTowerOfTrials main.cpp)
target_link_libraries(TheTowerOfTrials PRIVATE TTOTLibs cpr::cpr nlohmann_json::nlohmann_json)

foreach(target TheTowerOfTrials Test TTOTLibs)
    target_include_directories(${target} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
endforeach()