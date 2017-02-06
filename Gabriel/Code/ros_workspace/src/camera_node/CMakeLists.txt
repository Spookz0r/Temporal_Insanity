cmake_minimum_required(VERSION 2.8.3)
project(camera_node)

## Find catkin macros and libraries
## if COMPONENTS list like find_package(catkin REQUIRED COMPONENTS xyz)
## is used, also find other catkin packages
find_package(catkin REQUIRED COMPONENTS
  roscpp
  rospy
  sensor_msgs
  cv_bridge
  std_msgs
)


set(CMAKE_PREFIX_PATH "/home/opencv-3.1.0")
find_package(OpenCV 3.1 REQUIRED)
set(OpenCV_INCLUDE_DIRS "/home/opencv-3.1.0/include")
set(CMAKE_CXX_FLAGS "-std=c++11 ${CMAKE_CXX_FLAGS}")

###########
## Build ##
###########

## Specify additional locations of header files
## Your package locations should be listed before other locations
# include_directories(include)
include_directories(
  ${catkin_INCLUDE_DIRS} ${OpenCV_INCLUDE_DIRS}
)
catkin_package()

catkin_python_setup()

add_executable(camera_pub_cpp src/camera_pub_cpp.cpp)
target_link_libraries(camera_pub_cpp ${OpenCV_LIBRARIES} ${catkin_LIBRARIES})
