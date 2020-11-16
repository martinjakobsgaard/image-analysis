cmake_minimum_required(VERSION 3.1)
project(data-analysis)

# OpenCV
find_package( OpenCV REQUIRED )

# PCL
find_package(PCL 1.2 REQUIRED)
include_directories(${PCL_INCLUDE_DIRS})
link_directories(${PCL_LIBRARY_DIRS})
add_definitions(${PCL_DEFINITIONS})

# Set dependencies
set(DEPENDENCIES ${OpenCV_LIBS} ${PCL_LIBRARIES})

# Add image analysis binary
add_executable(image-analysis "image-analysis.cpp")
target_link_libraries(image-analysis stdc++fs ${DEPENDENCIES})

# Add pointcloud analysis binary
add_executable(pointcloud-analysis "pointcloud-analysis.cpp")
target_link_libraries(pointcloud-analysis stdc++fs ${DEPENDENCIES})
