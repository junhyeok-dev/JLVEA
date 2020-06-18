cmake_minimum_required(VERSION 3.15)
project(JLVEA-cpp)

set(CMAKE_CXX_STANDARD 14)
find_package(OpenCV REQUIRED)

add_executable(JLVEA-cpp main.cpp)

target_link_libraries(JLVEA-cpp ${OpenCV_LIBS})