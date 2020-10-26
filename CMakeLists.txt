cmake_minimum_required(VERSION 3.1)
project(image-analysis)

find_package( OpenCV REQUIRED )
set(DEPENDENCIES ${OpenCV_LIBS})

add_executable(image-analysis "main.cpp")
target_link_libraries(image-analysis stdc++fs ${DEPENDENCIES})
