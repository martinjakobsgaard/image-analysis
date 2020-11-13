cmake_minimum_required(VERSION 3.1)
project(data-analysis)

find_package( OpenCV REQUIRED )
set(DEPENDENCIES ${OpenCV_LIBS})

# Add image analysis binary
add_executable(image-analysis "image-analysis.cpp")
target_link_libraries(image-analysis stdc++fs ${DEPENDENCIES})

# Add pointcloud analysis binary
add_executable(pointcloud-analysis "pointcloud-analysis.cpp" "segment.cpp")
target_link_libraries(pointcloud-analysis stdc++fs ${DEPENDENCIES})
