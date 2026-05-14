cmake_minimum_required(VERSION 3.8)
project(meshcat_bridge)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(tf2 REQUIRED)
# 1. Find Eigen3
find_package(Eigen3 REQUIRED)

add_executable(meshcat_provider src/lqr_node.cpp)

# 2. Include the Eigen directories
target_include_directories(meshcat_provider PUBLIC
  ${EIGEN3_INCLUDE_DIRS}
)

# 3. Link dependencies to the executable
target_link_libraries(meshcat_provider PUBLIC
  ${geometry_msgs_TARGETS}
  rclcpp::rclcpp
  tf2::tf2
  Eigen3::Eigen
)

# Install the executable so 'ros2 run' can find it
install(TARGETS
  meshcat_provider
  DESTINATION lib/${PROJECT_NAME})

ament_package()