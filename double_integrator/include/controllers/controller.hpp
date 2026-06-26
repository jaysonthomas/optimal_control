#pragma once

#include <Eigen/Dense>

class Controller {
public:
  using State = Eigen::Vector2d;

  virtual ~Controller() = default;

  virtual double compute_control(const State &x) = 0;
};