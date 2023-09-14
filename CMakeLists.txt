cmake_minimum_required(VERSION 3.21)
project(NoGravityGameWS LANGUAGES CXX)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.x)
FetchContent_MakeAvailable(SFML)


add_executable(NoGravityGameWS 
               src/main.cpp 
               src/src/game.cpp 
               src/src/Physics2D/window.cpp
               src/src/Physics2D/world.cpp
               src/src/Physics2D/ray.cpp
               src/src/Physics2D/vector2d.cpp
               src/src/Physics2D/vector2dconverter.cpp
               src/src/Physics2D/rigidbody.cpp
               src/src/Physics2D/math2d.cpp
               src/src/Physics2D/collision2d.cpp
               src/src/Physics2D/transform2d.cpp
               src/src/Physics2D/aabb.cpp
               src/src/Physics2D/collisionManifold.cpp
               src/src/Physics2D/entity2d.cpp
               src/src/Physics2D/point2d.cpp
               src/src/Physics2D/text2d.cpp
)
target_include_directories(NoGravityGameWS PUBLIC include/no_gravity_game_ws)
target_link_libraries(NoGravityGameWS PRIVATE sfml-graphics)
target_compile_features(NoGravityGameWS PRIVATE cxx_std_17)

install(TARGETS NoGravityGameWS)