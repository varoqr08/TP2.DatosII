cmake_minimum_required(VERSION 3.8)
project(CVTest)

set(CMAKE_CXX_STANDARD 11)

find_package(OpenCV REQUIRED)

set(SOURCE_FILES main.cpp)
add_executable(CVTest ${SOURCE_FILES})
include_directories(${OpenCV_INCLUDE_DIRS})
target_link_libraries(CVTest ${OpenCV_LIBS})