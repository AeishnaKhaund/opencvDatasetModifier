cmake_minimum_required(VERSION 3.10)

project(OpencvDatasetCreator)
set(CMAKE_CXX_STANDARD 17)
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
include_directories(src/include/)

add_executable(test_ex src/main.cpp src/folderParser.cpp src/imageOverlayAndAugmentor.cpp src/include/imageOverlayAndAugmentor.h)
target_link_libraries(test_ex ${OpenCV_LIBS})
#target_link_libraries(test_ex stdc++fs)
