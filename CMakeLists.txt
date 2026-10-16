cmake_minimum_required(VERSION 3.16)
project(MiniPac LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(MiniPac
    main.cpp
    Game.cpp
    Level.cpp
    LevelLoader.cpp
    Player.cpp
    NPC.cpp
    Input.cpp
    Renderer.cpp
)

if(MSVC)
    target_compile_options(MiniPac PRIVATE /W4 /utf-8)
else()
    target_compile_options(MiniPac PRIVATE -Wall -Wextra -pedantic)
endif()
