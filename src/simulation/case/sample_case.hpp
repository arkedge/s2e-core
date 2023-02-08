/**
 * @file sample_case.hpp
 * @brief Example of user defined simulation case
 */

#ifndef S2E_SIMULATION_CASE_SAMPLE_CASE_HPP_
#define S2E_SIMULATION_CASE_SAMPLE_CASE_HPP_

#include "../ground_station/sample_ground_station/sample_ground_station.hpp"
#include "../spacecraft/sample_spacecraft/sample_spacecraft.hpp"
#include "./simulation_case.hpp"

/**
 * @class SampleCase
 * @brief An example of user side spacecraft class
 */
class SampleCase : public SimulationCase {
 public:
  /**
   * @fn SampleCase
   * @brief Constructor
   */
  SampleCase(std::string ini_base);

  /**
   * @fn ~SampleCase
   * @brief Destructor
   */
  virtual ~SampleCase();

  /**
   * @fn Initialize
   * @brief Override function of Initialize in SimulationCase
   */
  void Initialize();

  /**
   * @fn Main
   * @brief Override function of Main in SimulationCase
   */
  void Main();

  /**
   * @fn GetLogHeader
   * @brief Override function of GetLogHeader
   */
  virtual std::string GetLogHeader() const;
  /**
   * @fn GetLogValue
   * @brief Override function of GetLogValue
   */
  virtual std::string GetLogValue() const;

 private:
  SampleSat* sample_sat_;  //!< Instance of spacecraft
  SampleGS* sample_gs_;    //!< Instance of ground station
};

#endif  // S2E_SIMULATION_CASE_SAMPLE_CASE_HPP_
