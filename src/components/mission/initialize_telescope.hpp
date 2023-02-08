/*
 * @file initialize_telescope.hpp
 * @brief Initialize function of Telescope
 */

#ifndef S2E_COMPONENTS_MISSION_INITIALIZE_TELESCOPE_HPP_
#define S2E_COMPONENTS_MISSION_INITIALIZE_TELESCOPE_HPP_

#include <components/mission/telescope.hpp>

/*
 * @fn InitTelescope
 * @brief Initialize function of Telescope
 * @param [in] clock_gen: Clock generator
 * @param [in] sensor_id: Sensor ID
 * @param [in] fname: Path to initialize file
 * @param [in] attitude: Attitude information
 * @param [in] hipp: Star information by Hipparcos catalogue
 * @param [in] local_celes_info: Local celestial information
 */
Telescope InitTelescope(ClockGenerator* clock_gen, int sensor_id, const std::string fname, const Attitude* attitude, const HipparcosCatalogue* hipp,
                        const LocalCelestialInformation* local_celes_info);

#endif  // S2E_COMPONENTS_MISSION_INITIALIZE_TELESCOPE_HPP_
