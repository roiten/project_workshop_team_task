cmake_minimum_required(VERSION 3.20 FATAL_ERROR)
project(dbllb)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

set(SFML_STATIC_LIBRARIES TRUE)

find_package(SFML 3 REQUIRED COMPONENTS Graphics Window System)

add_executable(04 main.cpp)
target_compile_features(04 PUBLIC cxx_std_17)
target_link_libraries(04 PRIVATE SFML::Graphics SFML::Window SFML::System)