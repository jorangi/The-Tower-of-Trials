cmake_minimum_required(VERSION 3.10)

project(TowerOfTrials)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# file(GLOB_RECURSE SRC_FILES "*.cpp")

file(GLOB_RECURSE SRC_FILES 
    "Entity/*.cpp"
    "Stat/*.cpp"
    "Component/*.cpp"
    "Utility/*.cpp"
    "Test/*.cpp"
)

add_executable(TestApp ${SRC_FILES})

target_include_directories(TestApp PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})