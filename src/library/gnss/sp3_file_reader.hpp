/**
 * @file sp3_file_reader.hpp
 * @brief A class to read the SP3 (Extended Standard Product 3) format file and provide functions to access the data
 * @note Support version: SP3-d
 *       Ref: https://files.igs.org/pub/data/format/sp3d.pdf?_ga=2.115202830.823990648.1664976786-1792483726.1664976785
 */

#ifndef S2E_LIBRARY_GNSS_SP3_FILE_READER_HPP_
#define S2E_LIBRARY_GNSS_SP3_FILE_READER_HPP_

#include <library/math/vector.hpp>
#include <library/time_system/date_time_format.hpp>
#include <library/time_system/gps_time.hpp>
#include <string>
#include <vector>

/**
 * @enum Sp3Mode
 * @brief Data mode of SP3 file to define including data
 */
enum class Sp3Mode {
  kPosition,  //!< Position and clock data mode
  kVelocity,  //!< Velocity and clock rate data mode
};

/**
 * @enum Sp3OrbitType
 * @brief Orbit type defined in SP3
 */
enum class Sp3OrbitType {
  kFitted,        //!< Fitted
  kExtrapolated,  //!< Extrapolated or predicted
  kBroadcast,     //!< Broadcast
  kHelmert,       //!< Fitted after Helmert transformation
};

/**
 * @struct Sp3Header
 * @brief SP3 file header information
 */
struct Sp3Header {
  // 1st line information
  // version -> not implemented yet
  Sp3Mode mode_;                   //!< position or velocity
  DateTime start_epoch_;           //!< Time of start epoch
  size_t number_of_epoch_ = 0;     //!< Number of epoch in the SP3 file
  std::string used_data_;          //!< Used data to generate the SP3 file
  std::string coordinate_system_;  //!< Coordinate system for the position and velocity data
  Sp3OrbitType orbit_type_;        //!< Orbit type
  std::string agency_name_;        //!< Agency name who generates the SP3 file

  // 2nd line information
  GpsTime start_gps_time_;                        //!< Start time of orbit
  double epoch_interval_s_ = 1.0;                 //!< Epoch interval (0.0, 100000.0)
  size_t start_time_mjday_;                       //!< Start time of the orbit data (44244 = 6th Jan. 1980) [Modified Julian day]
  double start_time_mjday_fractional_day_ = 0.0;  //!< Fractional part of the start time [0.0, 1.0) [day]

  // 3rd line information
  size_t number_of_satellites_ = 0;  //!< Number of satellites in the SP3 file
  // Used satellite IDs (3rd to 11the line)
  std::vector<std::string> satellite_ids_;  //!< Satellite ID list

  // Accuracy of each satellite (12th to 20th line)
  std::vector<double> satellite_accuracy_;  //!< List of accuracy of each satellite

  // 21st line information
  std::string time_system_;  //!< Time system for the position and velocity data

  // 22nd line information
  // Not important

  // 23rd line information
  double base_number_position_ = 1.25;  //!< Floating point base number used for computing the standard deviations of position and velocity
  double base_number_clock_ = 1.025;    //!< Floating point base number used for computing the standard deviations of clock and clock-rate correction

  // 24th - 26th line information
  // Not important
};

/**
 * @class Sp3FileReader
 * @brief
 */
class Sp3FileReader {
 public:
  /**
   * @fn Sp3FileReader
   * @brief Constructor
   * @param[in] file_name: File name of target SP3 file with directory path
   */
  Sp3FileReader(const std::string file_name);

 private:
  Sp3Header header_;

  /**
   * @fn ReadFile
   * @brief Read SP3 file
   * @param[in] file_name: File name of target SP3 file with directory path
   * @return true: File read success, false: File read error
   */
  bool ReadFile(const std::string file_name);
  /**
   * @fn ReadHeader
   * @brief Read SP3 file
   * @param[in] sp3_file: file stream of the SP3 file
   * @return The last line of header. 0 means error is happened.
   */
  size_t ReadHeader(std::ifstream& sp3_file);
};

#endif  // S2E_LIBRARY_GNSS_SP3_FILE_READER_HPP_
