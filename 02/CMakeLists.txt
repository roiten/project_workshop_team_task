cmake_minimum_required(VERSION 4.1 FATAL_ERROR)

project(dbllb)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

find_package(SFML 3 COMPONENTS Graphics Window System)

add_executable(02 main.cpp)
target_compile_features(02 PUBLIC cxx_std_17)
target_link_libraries(02 PRIVATE SFML::Graphics SFML::Window SFML::System)
