cmake_minimum_required(VERSION 3.8)
project(motor_control)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c11")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20")

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)
find_package(geometry_msgs REQUIRED)
add_executable(motor_make src/motor_make.cpp)
add_executable(motor_pid src/pid_control.cpp)

target_include_directories(motor_make PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_include_directories(motor_pid PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/include
  ${CMAKE_CURRENT_SOURCE_DIR}/include/pid
)

ament_target_dependencies(motor_make rclcpp std_msgs geometry_msgs)
ament_target_dependencies(motor_pid rclcpp std_msgs geometry_msgs)

install(TARGETS
  motor_make
  motor_pid
  DESTINATION lib/${PROJECT_NAME}
)

install(DIRECTORY
  include/
  DESTINATION include
)

ament_package()
