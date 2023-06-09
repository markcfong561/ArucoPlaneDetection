cmake_minimum_required(VERSION 2.8)
project( ArucoPlaneDetection )
set(CMAKE_CXX_FLAGS "-std=c++11")
find_package( OpenCV REQUIRED )

include_directories("include/")

set (CMAKE_RUNTIME_OUTPUT_DIRECTORY "..")

add_executable( arucotest src/arucotest.cpp src/PlaneDetection.cpp)
target_link_libraries( arucotest
		       ${OpenCV_LIBS}
)