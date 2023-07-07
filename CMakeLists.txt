cmake_minimum_required(VERSION 2.8)
project( ArucoPlaneDetection )
set(CMAKE_CXX_FLAGS "-std=c++11")
SET("OpenCV_DIR" "/usr/share/OpenCV")
find_package(OpenCV REQUIRED)
find_package(Pylon QUIET)
find_package(Eigen3 REQUIRED)
if (NOT ${Pylon_FOUND})
    include("${CMAKE_CURRENT_SOURCE_DIR}/cmake/FindPylon.cmake")
endif()
find_package(Qt5 REQUIRED COMPONENTS Core Gui Widgets Test Concurrent)

link_directories(${OpenCV_LIBRARY_DIRS})

set (CMAKE_RUNTIME_OUTPUT_DIRECTORY "..")

include_directories( include
		     		${OpenCV_INCLUDE_DIRS}
		     		${Pylon_INCLUDE_DIRS}
	             	${pylon_camera_INCLUDE_DIRS}
)

add_executable( arucotest src/arucotest.cpp src/PlaneDetection.cpp)
target_link_libraries( arucotest
					   ${OpenCV_LIBS}
					   ${Pylon_LIBRARIES}
)