cmake_minimum_required(VERSION 3.8)
project(di_lqg)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(tf2 REQUIRED)
find_package(Eigen3 REQUIRED)

add_executable(di_lqg_node src/lqg_node.cpp)

target_link_libraries(di_lqg_node PUBLIC
  ${geometry_msgs_TARGETS}
  rclcpp::rclcpp
  tf2::tf2
  Eigen3::Eigen
)

# Install the executable so 'ros2 run' can find it
install(TARGETS
  di_lqg_node
  DESTINATION lib/${PROJECT_NAME})

ament_package()