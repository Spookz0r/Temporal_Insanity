cmake_minimum_required(VERSION 2.8.3)
project(gpio_test)

## Find catkin macros and libraries
## if COMPONENTS list like find_package(catkin REQUIRED COMPONENTS xyz)
## is used, also find other catkin packages
find_package(catkin REQUIRED COMPONENTS
  roscpp
  rospy
  sensor_msgs
  std_msgs
)


#FIND_LIBRARY(WIRINGPI_LIBRARY wiringPi /usr/lib)
include_directories(
  ${catkin_INCLUDE_DIRS}
)
catkin_package()

set(CMAKE_CXX_FLAGS "-std=c++11 ${CMAKE_CXX_FLAGS}")

## System dependencies are found with CMake's conventions
# find_package(Boost REQUIRED COMPONENTS system)

add_executable(gpio_test src/gpio_test.cpp)
target_link_libraries(gpio_test ${CATKIN_LIBRARIES})
##add_dependencies(gpio_test wiringPi)



# ${WIRINGPI_LIBRARY}
