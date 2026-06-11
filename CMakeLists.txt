cmake_minimum_required(VERSION 3.10)
project(MonsterGame CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Her lister vi alle dine nye .cpp filer, så underviseren kan compile det med CMake:
add_executable(MonsterGame 
    main.cpp 
    Character.cpp 
    Monster.cpp 
    Status.cpp 
    Item.cpp 
    BattleEngine.cpp
    Dungeon.cpp
)