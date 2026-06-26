#pragma once

#include <rclcpp/rclcpp.hpp>
#include <std_msgs/msg/float64_multi_array.hpp>

#include "sim.hpp"

class SimManagerNode : public rclcpp::Node {
public:
  SimManagerNode();

private:
  void timer_callback();

  Sim sim;
  rclcpp::Publisher<std_msgs::msg::Float64MultiArray>::SharedPtr publisher;
  rclcpp::TimerBase::SharedPtr timer;
};