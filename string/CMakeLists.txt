cmake_minimum_required(VERSION 3.8 FATAL_ERROR)

project(string LANGUAGES CXX)

add_executable(${PROJECT_NAME} main.cpp string.cpp string.h)

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)

set_target_properties(${PROJECT_NAME} PROPERTIES CXX_EXTENSIONS OFF)
