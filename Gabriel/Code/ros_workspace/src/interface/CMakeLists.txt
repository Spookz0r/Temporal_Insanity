cmake_minimum_required(VERSION 2.8.3)
project(gui)

## Find catkin and any catkin packages
find_package(catkin REQUIRED COMPONENTS roscpp rospy std_msgs genmsg image_transport sensor_msgs cv_bridge)
find_package(PkgConfig REQUIRED)
pkg_check_modules(GTKMM REQUIRED gtkmm-3.0)

find_package(OpenCV)

include_directories(include ${OpenCV_INCLUDE_DIRS})
## FLAGS

link_directories(${GTKMM_LIBRARY_DIRS})
include_directories(include ${GTKMM_INCLUDE_DIRS})
set(CMAKE_CXX_FLAGS "-std=c++11 ${CMAKE_CXX_FLAGS}")

################################################
## Declare ROS messages, services and actions ##
################################################
##Declare ros messages and services
##add_message_files(DIRECTORY msg FILES Num.msg)
##add_service_files(DIRECTORY srv FILES AddTwoInts.srv)

##generate added messages and services

## Declare a catkin package
catkin_package()



##Build talker and listener
include_directories(include ${catkin_INCLUDE_DIRS})



add_executable(gui src/main.cpp src/gui.cpp src/button.cpp src/image_show.cpp src/icon.cpp)
target_link_libraries(gui ${catkin_LIBRARIES} ${GTKMM_LIBRARIES}  ${OpenCV_LIBRARIES})
add_dependencies(gui beginner_tutorials_generate_messages_cpp)


