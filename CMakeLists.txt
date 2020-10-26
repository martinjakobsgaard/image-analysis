cmake_minimum_required(VERSION 2.8)
project(image-analysis)

find_package( OpenCV REQUIRED )
set(DEPENDENCIES ${OpenCV_LIBS})

add_executable(blur "blur.cpp")
target_link_libraries(blur ${DEPENDENCIES})

add_executable(segmentation "segmentation.cpp")
target_link_libraries(segmentation ${DEPENDENCIES})
