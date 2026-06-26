#include <chrono>

#include "sim_manager_node.hpp"
#include "controllers/dlqr.hpp"

using namespace std::chrono_literals;

SimManagerNode::SimManagerNode() : Node("DoubleIntegratorNode"), sim("dlqr", 0.1) {
  publisher = create_publisher<std_msgs::msg::Float64MultiArray>("sim_data", 10);
  timer = create_wall_timer(100ms, std::bind(&SimManagerNode::timer_callback, this));
}

void SimManagerNode::timer_callback() {
  if (sim.get_current_time() < 10) {
    sim.step();
  }
  
  auto x = sim.get_state();
  std_msgs::msg::Float64MultiArray msg;
  msg.data = {sim.get_current_time(), x(0), x(1), sim.get_control()};
  publisher->publish(msg);
}