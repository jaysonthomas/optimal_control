#pragma once

#include <memory>
#include "controllers/controller.hpp"
#include "double_integrator.hpp"

class Sim {
public:
  Sim(const std::string& controller_type, double dt);
  double get_current_time() const;
  double get_control() const;
  const Eigen::Vector2d & get_state() const;  
  void step();

private:
  std::unique_ptr<Controller> initialise_controller(const std::string &type);
  DoubleIntegrator plant;
  std::unique_ptr<Controller> controller;
  double dt;
  double t;
  double u;
};