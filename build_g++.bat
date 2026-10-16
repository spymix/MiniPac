@echo off
setlocal

g++ -std=c++17 -Wall -Wextra -pedantic ^
    main.cpp Game.cpp Level.cpp LevelLoader.cpp Player.cpp NPC.cpp Input.cpp Renderer.cpp ^
    -o MiniPac.exe

if errorlevel 1 (
    echo.
    echo BUILD FAILED
    exit /b 1
)

echo.
echo BUILD OK
MiniPac.exe
