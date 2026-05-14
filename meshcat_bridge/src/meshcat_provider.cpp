#include <chrono>
#include <memory>
#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/transform_stamped.hpp"
#include <tf2/LinearMath/Quaternion.h>

using namespace std::chrono_literals;

class MeshcatProvider : public rclcpp::Node {
public:
  MeshcatProvider() : Node("meshcat_provider"), count_(0) {
    publisher_ = this->create_publisher<geometry_msgs::msg::TransformStamped>("/meshcat_data", 10);
    timer_ = this->create_wall_timer(50ms, std::bind(&MeshcatProvider::timer_callback, this));
  }

private:
  void timer_callback() {
    auto message = geometry_msgs::msg::TransformStamped();
    
    message.header.stamp = this->get_clock()->now();
    message.header.frame_id = "world";

    // Create a simple rotation based on time
    tf2::Quaternion q;
    double angle = count_ * 0.1;
    q.setRPY(angle, angle * 0.5, 0);

    message.transform.translation.x = 0.5 * cos(angle);
    message.transform.translation.y = 0.5 * sin(angle);
    message.transform.translation.z = 0.0;
    
    message.transform.rotation.x = q.x();
    message.transform.rotation.y = q.y();
    message.transform.rotation.z = q.z();
    message.transform.rotation.w = q.w();

    publisher_->publish(message);
    count_++;
  }
  rclcpp::TimerBase::SharedPtr timer_;
  rclcpp::Publisher<geometry_msgs::msg::TransformStamped>::SharedPtr publisher_;
  size_t count_;
};

int main(int argc, char * argv[]) {
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<MeshcatProvider>());
  rclcpp::shutdown();
  return 0;
}