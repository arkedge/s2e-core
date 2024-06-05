/**
 * @file ode_attitude.hpp
 * @brief Class to implement Ordinary Differential Equations for Attitude
 */

#ifndef S2E_DYNAMICS_ATTITUDE_ODE_ATTITUDE_HPP_
#define S2E_DYNAMICS_ATTITUDE_ODE_ATTITUDE_HPP_

#include <math_physics/numerical_integration/interface_ode.hpp>
#include <utilities/macros.hpp>

#include "attitude.hpp"

namespace libra::numerical_integration {
/**
 * @class AttitudeWithCantileverVibrationOde
 * @brief Class to implement Ordinary Differential Equations for Attitude with Cantilever Vibration
 */
class AttitudeWithCantileverVibrationOde : public InterfaceOde<13> {
 public:
  Vector<13> DerivativeFunction(const double time_s, const Vector<13>& state) const override {
    UNUSED(time_s);

    libra::Vector<13> output;

    libra::Vector<3> omega_b_rad_s;
    for (int i = 0; i < 3; i++) {
      omega_b_rad_s[i] = state[i];
    }
    libra::Vector<3> omega_cantilever_rad_s;
    for (int i = 0; i < 3; i++) {
      omega_cantilever_rad_s[i] = state[i + 3];
    }

    libra::Vector<3> euler_angle_cantilever_rad;
    for (int i = 0; i < 3; i++) {
      euler_angle_cantilever_rad[i] = state[i + 10];
    }

    libra::Vector<3> angular_momentum_total_b_Nms = (previous_inertia_tensor_kgm2_ * omega_b_rad_s) + angular_momentum_reaction_wheel_b_Nms_;
    libra::Vector<3> net_torque_b_Nm = torque_b_Nm_ - libra::OuterProduct(omega_b_rad_s, angular_momentum_total_b_Nms) - torque_inertia_tensor_b_Nm_;

    libra::Vector<3> angular_accelaration_cantilever_rad_s2 =
        -(inverse_equivalent_inertia_tensor_cantilever_ *
          (attenuation_coefficient_ * omega_cantilever_rad_s + spring_coefficient_ * euler_angle_cantilever_rad)) -
        inverse_inertia_tensor_ * net_torque_b_Nm;

    libra::Vector<3> rhs = inverse_inertia_tensor_ * (net_torque_b_Nm - inertia_tensor_cantilever_kgm2_ * angular_accelaration_cantilever_rad_s2);

    for (int i = 0; i < 3; ++i) {
      output[i] = rhs[i];
    }

    for (int i = 0; i < 3; i++) {
      output[i + 3] = angular_accelaration_cantilever_rad_s2[i];
    }

    libra::Vector<4> quaternion_i2b;
    for (int i = 0; i < 4; i++) {
      quaternion_i2b[i] = state[i + 6];
    }

    libra::Vector<4> d_quaternion = 0.5 * CalcAngularVelocityMatrix(omega_b_rad_s) * quaternion_i2b;

    for (int i = 0; i < 4; i++) {
      output[i + 6] = d_quaternion[i];
    }

    for (int i = 0; i < 3; i++) {
      output[i + 10] = omega_cantilever_rad_s[i];
    }

    return output;
  }

  // Setter
  /**
   * @fn SetAttenuationCoefficient
   * @brief Set attenuation coefficient
   */
  inline void SetAttenuationCoefficient(const double attenuation_coefficient) { attenuation_coefficient_ = attenuation_coefficient; }
  /**
   * @fn SetSpringCoefficient
   * @brief Set spring coefficient
   */
  inline void SetSpringCoefficient(const double spring_coefficient) { spring_coefficient_ = spring_coefficient; }
  /**
   * @fn SetTorque_b_Nm
   * @brief Set torque acting on the spacecraft on the body fixed frame [Nm]
   */
  inline void SetTorque_b_Nm(const libra::Vector<3> torque_b_Nm) { torque_b_Nm_ = torque_b_Nm; }
  /**
   * @fn SetTorqueInertiaTensor_b_Nm
   * @brief Set torque generated by inertia tensor [Nm]
   */
  inline void SetTorqueInertiaTensor_b_Nm(const libra::Vector<3> torque_inertia_tensor_b_Nm) {
    torque_inertia_tensor_b_Nm_ = torque_inertia_tensor_b_Nm;
  }
  /**
   * @fn SetAngularMomentumReactionWheel_b_Nms
   * @brief Set angular momentum of reaction wheel in the body fixed frame [Nms]
   */
  inline void SetAngularMomentumReactionWheel_b_Nms(const libra::Vector<3> angular_momentum_reaction_wheel_b_Nms) {
    angular_momentum_reaction_wheel_b_Nms_ = angular_momentum_reaction_wheel_b_Nms;
  }
  /**
   * @fn SetInverseInertiaTensor
   * @brief Set inverse of inertia tensor
   */
  inline void SetInverseInertiaTensor(const libra::Matrix<3, 3> inverse_inertia_tensor) { inverse_inertia_tensor_ = inverse_inertia_tensor; }
  /**
   * @fn SetPreviousInertiaTensor_kgm2
   * @brief Set previous inertia tensor [kgm2]
   */
  inline void SetPreviousInertiaTensor_kgm2(const libra::Matrix<3, 3> previous_inertia_tensor_kgm2) {
    previous_inertia_tensor_kgm2_ = previous_inertia_tensor_kgm2;
  }
  /**
   * @fn SetInertiaTensorCantilever_kgm2
   * @brief Set inertia tensor of the cantilever [kgm2]
   */
  inline void SetInertiaTensorCantilever_kgm2(const libra::Matrix<3, 3> inertia_tensor_cantilever_kgm2) {
    inertia_tensor_cantilever_kgm2_ = inertia_tensor_cantilever_kgm2;
  }
  /**
   * @fn SetInverseEquivalentInertiaTensorCantilever
   * @brief Set inverse of inertia tensor of the cantilever
   */
  inline void SetInverseEquivalentInertiaTensorCantilever(const libra::Matrix<3, 3> inverse_equivalent_inertia_tensor_cantilever) {
    inverse_equivalent_inertia_tensor_cantilever_ = inverse_equivalent_inertia_tensor_cantilever;
  }

 protected:
  double attenuation_coefficient_ = 0.0;                                   //!< Attenuation coefficient
  double spring_coefficient_ = 0.0;                                        //!< Spring coefficient
  libra::Vector<3> torque_b_Nm_{0.0};                                      //!< Torque in the body fixed frame [Nm]
  libra::Vector<3> torque_inertia_tensor_b_Nm_{0.0};                       //!< Torque generated by inertia tensor [Nm]
  libra::Vector<3> angular_momentum_reaction_wheel_b_Nms_{0.0};            //!< Angular momentum of reaction wheel in the body fixed frame [Nms]
  libra::Matrix<3, 3> inverse_inertia_tensor_{0.0};                        //!< Inverse of inertia tensor
  libra::Matrix<3, 3> previous_inertia_tensor_kgm2_{0.0};                  //!< Previous inertia tensor [kgm2]
  libra::Matrix<3, 3> inertia_tensor_cantilever_kgm2_{0.0};                //!< Inertia tensor of the cantilever [kgm2]
  libra::Matrix<3, 3> inverse_equivalent_inertia_tensor_cantilever_{0.0};  //!< Inverse of inertia tensor of the cantilever
};

}  // namespace libra::numerical_integration

#endif  // S2E_DYNAMICS_ATTITUDE_ODE_ATTITUDE_HPP_
