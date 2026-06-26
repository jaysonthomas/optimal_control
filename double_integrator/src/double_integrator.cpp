#include "double_integrator.hpp"

DoubleIntegrator::DoubleIntegrator(const double dt) {
  x << 1.0, 0.0;
  A << 1.0, dt, 0.0, 1.0;
  B << 0.5 * dt * dt, dt;
}

const DoubleIntegrator::State &DoubleIntegrator::get_state() const { return x; }
const Eigen::Matrix2d &DoubleIntegrator::get_A() const { return A; }
const Eigen::Vector2d &DoubleIntegrator::get_B() const { return B; }

void DoubleIntegrator::step(const double u) { 
  x = A * x + B * u; 
}
