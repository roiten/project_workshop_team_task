cmake_minimum_required(VERSION 3.10)
project(13)

add_executable(13 main.cpp)

find_package(SFML 2.6 COMPONENTS graphics window system REQUIRED)

target_compile_features(13 PUBLIC cxx_std_17)

target_link_libraries(03
        sfml-graphics
        sfml-window
        sfml-system
)