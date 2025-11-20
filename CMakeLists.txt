cmake_minimum_required(VERSION 3.16)
project(FishGame VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets Gui Core Multimedia) # Multimedia用于音效

include_directories(
    src
    src/core
    src/entities
    src/scenes
    src/ui
    src/utils
)

# 包含你列出的所有文件
add_executable(FishGame
    src/main.cpp
    src/resources.qrc
    
    # Core
    src/core/GameEngine.cpp
    src/core/GameEngine.h
    src/core/Constants.h
    
    # Entities
    src/entities/Entity.cpp
    src/entities/Entity.h
    src/entities/Player.cpp
    src/entities/Player.h
    src/entities/Enemy.cpp
    src/entities/Enemy.h
    
    # Scenes
    src/scenes/MainScene.cpp
    src/scenes/MainScene.h
    src/scenes/LevelManager.cpp
    src/scenes/LevelManager.h
    
    # UI
    src/ui/MainWindow.cpp
    src/ui/MainWindow.h
    src/ui/StartScreen.cpp
    src/ui/StartScreen.h
    src/ui/GameHud.cpp
    src/ui/GameHud.h
    src/ui/PauseDialog.cpp
    src/ui/PauseDialog.h
    src/ui/LevelInfoDialog.cpp
    src/ui/LevelInfoDialog.h
    
    # Utils
    src/utils/AudioManager.cpp
    src/utils/AudioManager.h
)

target_link_libraries(FishGame PRIVATE Qt6::Widgets Qt6::Gui Qt6::Core Qt6::Multimedia)