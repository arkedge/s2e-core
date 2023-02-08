/**
 * @file simulation_object.h
 * @brief Class to manage randomization of variables for Monte-Carlo simulation
 */

#ifndef S2E_SIMULATION_MONTE_CARLO_SIMULATION_SIMULATION_OBJECT_HPP_
#define S2E_SIMULATION_MONTE_CARLO_SIMULATION_SIMULATION_OBJECT_HPP_

#include <library/math/quaternion.hpp>
#include <library/math/vector.hpp>
#include <functional>
#include <map>
#include <string>
#include <vector>

#include "initialize_monte_carlo_parameters.hpp"
#include "monte_carlo_simulation_executor.hpp"

using libra::Vector;

/**
 * @class SimulationObject
 * @brief Class to manage randomization of variables for Monte-Carlo simulation
 */
class SimulationObject {
 public:
  /**
   * @fn SimulationObject
   * @brief Constructor
   */
  explicit SimulationObject(std::string name);

  /**
   * @fn ~SimulationObject
   * @brief Destructor
   */
  virtual ~SimulationObject();

  /**
   * @fn GetInitParameterVec
   * @brief Get randomized vector value and store it in dst_vec
   */
  template <size_t NumElement>
  void GetInitParameterVec(const MCSimExecutor& mc_sim, std::string ip_name, Vector<NumElement>& dst_vec) const;

  /**
   * @fn GetInitParameterDouble
   * @brief Get randomized value and store it in dst
   */
  void GetInitParameterDouble(const MCSimExecutor& mc_sim, std::string ip_name, double& dst) const;

  /**
   * @fn GetInitParameterQuaternion
   * @brief Get randomized quaternion and store it in dst_quat
   */
  void GetInitParameterQuaternion(const MCSimExecutor& mc_sim, std::string ip_name, Quaternion& dst_quat) const;

  /**
   * @fn SetParameters
   * @brief Virtual function to set the randomized results to target variables
   */
  virtual void SetParameters(const MCSimExecutor& mc_sim) = 0;

  /**
   * @fn SetAllParameters
   * @brief Execute all SetParameter function for all SimulationObject instance
   */
  static void SetAllParameters(const MCSimExecutor& mc_sim);

 private:
  std::string name_;  //!< Name to distinguish the target variable in initialize file for Monte-Carlo simulation
  static std::map<std::string, SimulationObject*> so_list_;  //!< list of objects with simulation parameters
};

/**
 * @fn GetInitParameterVec
 * @brief Return initialized parameters for vector
 */
template <size_t NumElement>
void SimulationObject::GetInitParameterVec(const MCSimExecutor& mc_sim, std::string ip_name, Vector<NumElement>& dst_vec) const {
  mc_sim.GetInitParameterVec(name_, ip_name, dst_vec);
}

#endif  // S2E_SIMULATION_MONTE_CARLO_SIMULATION_SIMULATION_OBJECT_HPP_
