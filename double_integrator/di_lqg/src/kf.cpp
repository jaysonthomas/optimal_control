#include <kf.hpp>
#include <stdafx.hpp>

KalmanFilter::KalmanFilter(const Eigen::MatrixXd &A, const Eigen::MatrixXd &B,
                           const Eigen::MatrixXd &C, const Eigen::MatrixXd &P,
                           const Eigen::MatrixXd &V, const Eigen::MatrixXd &W)
    : A(A), B(B), C(C), P(P), P0(P), V(V), W(W),
      I(this->A.rows(), this->A.rows()), x_hat(this->A.rows()) {
  I.setIdentity();
}

KalmanFilter::~KalmanFilter() {}

void KalmanFilter::init() {
  P = P0;
  x_hat.setZero();
}

void KalmanFilter::init(const Eigen::VectorXd &x) {
  init();
  x_hat = x;
}

void KalmanFilter::predict(const Eigen::VectorXd &u) {
  x_hat = A * x_hat + B * u;
  P = A * P * A.transpose() + W;
}

void KalmanFilter::update(const Eigen::VectorXd &y) {
  // First, compute innovation, innovation covariance and kalman gain.
  Eigen::MatrixXd z = y - C * x_hat;
  Eigen::MatrixXd S = C * P * C.transpose() + V; 

  // 2. Compute Kalman Gain using LDLT decomposition (Avoids .inverse())
    // We want: m_L = m_P * m_C.T * S^-1  =>  m_L * S = m_P * m_C.T
    // Taking transpose: S * m_L.T = m_C * m_P
    m_L = S.ldlt().solve(m_C * m_P).transpose();

  L = S.ldlt().solve(P * C.transpose());
  
  x_hat += (L * z);
  P = (I - L * C) * P * (I - L * C).transpose() + L * V * L.transpose();
}

Eigen::VectorXd KalmanFilter::get_state() { return x_hat; }