/**
 * @file runge_kutta.cpp
 * @brief Class for General Runge-Kutta method
 */
#ifndef S2E_LIBRARY_NUMERICAL_INTEGRATION_RUNGE_KUTTA_TEMPLATE_HPP_
#define S2E_LIBRARY_NUMERICAL_INTEGRATION_RUNGE_KUTTA_TEMPLATE_HPP_

#include "./runge_kutta.hpp"

namespace libra {

template <size_t N>
void RungeKutta<N>::Integrate() {
  CalcSlope();
  for (size_t i = 0; i < stage_; i++) {
    current_state_ = current_state_ + b_[i] * step_width_s_ * k_[i];
  }
}

template <size_t N>
void RungeKutta<N>::CalcSlope() {
  k_[0] = DerivativeFunction(current_time_s_, current_state_);
  for (size_t i = 1; i < stage_; i++) {
    Vector<N> state = current_state_;
    for (size_t j = 0; j < i; j++) {
      state = state + a_[i][j] * step_width_s_ * k_[j];
    }
    double time_s = current_time_s_ + c_[i] * step_width_s_;
    k_[i] = DerivativeFunction(time_s, state);
  }
}

}  // namespace libra

#endif  // S2E_LIBRARY_NUMERICAL_INTEGRATION_RUNGE_KUTTA_TEMPLATE_HPP_
