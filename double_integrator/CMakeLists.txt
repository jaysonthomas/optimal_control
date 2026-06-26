cmake_minimum_required(VERSION 3.8)
project(double_integrator)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(tf2 REQUIRED)
find_package(Eigen3 REQUIRED)

add_executable( double_integrator_node 
                src/controllers/dlqr.cpp
                src/double_integrator.cpp
                src/main.cpp
                src/sim.cpp
                src/sim_manager_node.cpp)

target_include_directories(double_integrator_node PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)

target_link_libraries(double_integrator_node PUBLIC
  ${geometry_msgs_TARGETS}
  rclcpp::rclcpp
  tf2::tf2
  Eigen3::Eigen
)

# Install the executable so 'ros2 run' can find it
install(TARGETS
  double_integrator_node
  DESTINATION lib/${PROJECT_NAME})

ament_package()