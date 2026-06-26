#include "sim.hpp"
#include "controllers/dlqr.hpp"

Sim::Sim(const std::string& controller_type, double dt)
    : plant(dt), controller(initialise_controller(controller_type)), dt(dt), t(0.0), u(0.0) 
{ }

double Sim::get_current_time() const { return t; }
double Sim::get_control() const { return u; }
const Eigen::Vector2d & Sim::get_state() const { return plant.get_state(); }

std::unique_ptr<Controller> Sim::initialise_controller(const std::string &type) {
  if (type == "dlqr") {
    return std::make_unique<DLQR>(plant.get_A(), plant.get_B(), 
                                  Eigen::Matrix2d::Identity(), 0.1);
  }

  throw std::runtime_error("Unknown controller type");
}

void Sim::step() {
  u = controller->compute_control(plant.get_state());
  plant.step(u);
  t += dt;
}
