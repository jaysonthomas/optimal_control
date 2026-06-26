#include "controllers/dlqr.hpp"

DLQR::DLQR(const AMatrix &A, const BMatrix &B, const QMatrix &Q, const double R) {
  K = solve_dlqr(A, B, Q, R);
}

double DLQR::compute_control(const State &x) { return -(K * x)(0); }

DLQR::GainMatrix DLQR::compute_K( const AMatrix &A, const BMatrix &B, 
                                  const Eigen::Matrix2d P, const double R) {
  double scalar = R + (B.transpose() * P * B).value();
  return (1.0 / scalar) * B.transpose() * P * A;
}

DLQR::GainMatrix DLQR::solve_dlqr(const AMatrix &A, const BMatrix &B,
                                  const QMatrix &Q, const double R) {
  Eigen::Matrix2d P = Q;
  Eigen::Matrix2d P_next;
                                    
  for (int i = 0; i < 1000; ++i) {
    DLQR::GainMatrix K = compute_K(A, B, P, R);
    P_next = Q + A.transpose() * P * A - (A.transpose() * P * B * K);

    if ((P_next - P).norm() < 1e-9) {
      P = P_next;
      break;
    }
    P = P_next;
  }
  return compute_K(A, B, P, R);
}