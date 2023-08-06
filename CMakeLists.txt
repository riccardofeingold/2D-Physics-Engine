cmake_minimum_required(VERSION 3.21)
project(NoGravityGameWS LANGUAGES CXX)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.x)
FetchContent_MakeAvailable(SFML)


add_executable(NoGravityGameWS src/main.cpp src/src/player.cpp)
target_include_directories(NoGravityGameWS PUBLIC include/no_gravity_game_ws)
target_link_libraries(NoGravityGameWS PRIVATE sfml-graphics)
target_compile_features(NoGravityGameWS PRIVATE cxx_std_17)

install(TARGETS NoGravityGameWS)