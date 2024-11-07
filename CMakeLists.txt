cmake_minimum_required(VERSION 2.8.3)
project(integrated_navigation)

## Add support for C++11, supported in ROS Kinetic and newer
add_definitions(-std=c++11)

find_package(catkin REQUIRED COMPONENTS
  message_generation
  roscpp
  std_msgs
)

## Generate messages in the 'msg' folder
 add_message_files(
   FILES
   integratedNavigationMsgx1.msg 
)


## Generate added messages and services with any dependencies listed here
 generate_messages(
   DEPENDENCIES
   std_msgs
 )

catkin_package(
  INCLUDE_DIRS include
  #LIBRARIES IntegratedNavigation
  CATKIN_DEPENDS roscpp std_msgs
  DEPENDS system_lib
  message_runtime
)
## Specify additional locations of header files
## Your package locations should be listed before other locations
# include_directories(include)
include_directories(
  ${catkin_INCLUDE_DIRS}
)

## The recommended prefix ensures that target names across packages don't collide
 add_executable(integratedNavigation_node src/main.cpp src/uart.cpp)

## same as for the library above
 add_dependencies(integratedNavigation_node ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})

## Specify libraries to link a library or executable target against
 target_link_libraries(integratedNavigation_node
   ${catkin_LIBRARIES}
 )
