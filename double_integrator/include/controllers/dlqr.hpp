#pragma once

#include "controllers/controller.hpp"

class DLQR : public Controller {
public:
  using AMatrix = Eigen::Matrix2d;
  using BMatrix = Eigen::Vector2d;
  using QMatrix = Eigen::Matrix2d;
  using GainMatrix = Eigen::RowVector2d;

  DLQR(const AMatrix &A, const BMatrix &B, const QMatrix &Q, const double R);
  double compute_control(const State &x) override;

private:
  GainMatrix compute_K(const AMatrix &A, const BMatrix &B, const Eigen::Matrix2d P, const double R);
  GainMatrix solve_dlqr(const AMatrix &A, const BMatrix &B, const QMatrix &Q, const double R);

  GainMatrix K;
};