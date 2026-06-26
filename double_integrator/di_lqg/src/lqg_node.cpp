#include <chrono>
#include <functional>
#include <memory>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/float64_multi_array.hpp"
#include <Eigen/Dense>

using namespace std::chrono_literals;

class LQRControllerNode : public rclcpp::Node {
  rclcpp::Publisher<std_msgs::msg::Float64MultiArray>::SharedPtr publisher_;
  rclcpp::TimerBase::SharedPtr timer_;

  Eigen::Vector2d x_;
  Eigen::Matrix2d A_, Q_;
  Eigen::Vector2d B_;
  Eigen::RowVector2d K_;

  double dt_;
  double t_;

public:
  LQRControllerNode() : Node("lqr_online_node") {
    publisher_ = this->create_publisher<std_msgs::msg::Float64MultiArray>(
        "sim_data", 10);

    // parameters
    dt_ = 0.1;
    t_ = 0.0;

    x_ << 1.0, 0.0;
    A_ << 1.0, dt_, 0.0, 1.0;
    B_ << 0.5 * dt_ * dt_, dt_;
    Q_ = Eigen::Matrix2d::Identity();
    const double R = 0.1;

    // compute LQR gain once (still optimal control)
    K_ = compute_dlqr(A_, B_, Q_, R);

    timer_ = this->create_wall_timer(100ms,
                                     std::bind(&LQRControllerNode::step, this));

    RCLCPP_INFO(get_logger(), "Online LQR simulation started.");
  }

private:
  void step() {
    if (t_ >= 10.0) {
      timer_->cancel();
      RCLCPP_INFO(get_logger(), "Simulation stopped at t = 10s");
      return;
    }

    const double u = -(K_ * x_)(0);

    // publish
    std_msgs::msg::Float64MultiArray msg;
    msg.data = {t_, x_(0), x_(1), u};
    publisher_->publish(msg);

    // dynamics update
    x_ = A_ * x_ + B_ * u;
    t_ += dt_;
  }

  Eigen::RowVector2d compute_dlqr(Eigen::Matrix2d A, Eigen::Vector2d B,
                                  Eigen::Matrix2d Q, double R) {
    Eigen::Matrix2d P = Q;
    Eigen::Matrix2d P_next;

    for (int i = 0; i < 1000; ++i) {

      double scalar_term = R + (B.transpose() * P * B).value();
      Eigen::Matrix2d gain_part = (A.transpose() * P * B) *
                                  (1.0 / scalar_term) * (B.transpose() * P * A);
      P_next = Q + A.transpose() * P * A - gain_part;

      if ((P_next - P).norm() < 1e-9) {
        P = P_next;
        break;
      }
      P = P_next;
    }

    double final_scalar = R + (B.transpose() * P * B).value();
    return (1.0 / final_scalar) * (B.transpose() * P * A);
  }
};

int main(int argc, char **argv) {
  rclcpp::init(argc, argv);

  auto node = std::make_shared<LQRControllerNode>();
  rclcpp::spin(node);

  rclcpp::shutdown();
  return 0;
}