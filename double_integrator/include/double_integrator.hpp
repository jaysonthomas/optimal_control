#pragma once
#include <Eigen/Dense>

class DoubleIntegrator {
public:
  using State = Eigen::Vector2d;

  explicit DoubleIntegrator(const double dt);
  void step(const double u);

  const State & get_state() const;
  const Eigen::Matrix2d & get_A() const;
  const Eigen::Vector2d & get_B() const;

private:
  State x;
  Eigen::Matrix2d A;
  Eigen::Vector2d B;
};