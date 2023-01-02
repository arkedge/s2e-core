/*
 * @file GScalculator.cpp
 * @brief Emulation of analysis and calculation for Ground Stations
 */

#include "GScalculator.h"

#include <Environment/Global/PhysicalConstants.hpp>
#include <Library/math/Constant.hpp>

GScalculator::GScalculator(const double loss_polarization, const double loss_atmosphere, const double loss_rainfall, const double loss_others,
                           const double EbN0, const double hardware_deterioration, const double coding_gain, const double margin_req)
    : loss_polarization_(loss_polarization),
      loss_atmosphere_(loss_atmosphere),
      loss_rainfall_(loss_rainfall),
      loss_others_(loss_others),
      EbN0_(EbN0),
      hardware_deterioration_(hardware_deterioration),
      coding_gain_(coding_gain),
      margin_req_(margin_req) {
  max_bitrate_ = 0.0f;
}

GScalculator::~GScalculator() {}

void GScalculator::Update(const Spacecraft& spacecraft, const Antenna& sc_ant, const GroundStation& groundstation, const Antenna& gs_ant) {
  bool is_visible = groundstation.IsVisible(spacecraft.GetSatID());
  if (is_visible) {
    max_bitrate_ = CalcMaxBitrate(spacecraft.GetDynamics(), sc_ant, groundstation, gs_ant);
  } else {
    max_bitrate_ = 0.0f;
  }
}

double GScalculator::CalcMaxBitrate(const Dynamics& dynamics, const Antenna& sc_ant, const GroundStation& groundstation, const Antenna& gs_ant) {
  if (!sc_ant.IsTransmitter() || !gs_ant.IsReceiver()) {
    // Check compatibility of transmitter and receiver
    return 0.0f;
  }

  // TODO: Consider Inertial frame of ECEF frame?
  Vector<3> sc_pos_i = dynamics.GetOrbit().GetSatPosition_i();
  Vector<3> gs_pos_i = groundstation.GetGSPosition_i();
  double dist_sc_gs_km = norm(sc_pos_i - gs_pos_i) / 1000;
  double loss_space_dB = -20 * log10(4 * libra::pi * dist_sc_gs_km / (300 / sc_ant.GetFrequency() / 1000));

  // TODO: Calculate the boresight angle from position of the satellite
  double sc_boresight_angle_rad = 0;
  // Ref.  // double theta = angle(q_b2i.frame_conv(axis_b), rel_pos);

  double gs_boresight_angle_rad = 0;  // Assume the GS antenna completely track the satellite

  double cn0_dBHz = sc_ant.CalcTxEIRP(sc_boresight_angle_rad) + loss_space_dB + loss_polarization_ + loss_atmosphere_ + loss_rainfall_ +
                    loss_others_ + gs_ant.CalcRxGT(gs_boresight_angle_rad) - 10 * log10(environment::boltzmann_constant_J_K);

  double margin_for_bitrate_dB = cn0_dBHz - (EbN0_ + hardware_deterioration_ + coding_gain_) - margin_req_;

  if (margin_for_bitrate_dB > 0) {
    return pow(10, margin_for_bitrate_dB / 10.) / 1000000.;  //[MHz]? TODO: Need to check the correctness...
  } else {
    return 0.0;
  }
}

double GScalculator::CalcCn0OnGs(const Dynamics& dynamics, const Antenna& sc_tx_ant, const GroundStation& ground_station, const Antenna& gs_rx_ant) {
  if (!sc_tx_ant.IsTransmitter() || !gs_rx_ant.IsReceiver()) {
    // Check compatibility of transmitter and receiver
    return 0.0f;
  }

  // Free space path loss
  Vector<3> sc_pos_i = dynamics.GetOrbit().GetSatPosition_i();
  Vector<3> gs_pos_i = ground_station.GetGSPosition_i();
  double dist_sc_gs_km = norm(sc_pos_i - gs_pos_i) / 1000;
  double loss_space_dB = -20 * log10(4 * libra::pi * dist_sc_gs_km / (300 / sc_tx_ant.GetFrequency() / 1000));

  // GS direction on SC TX antenna frame
  Vector<3> sc_to_gs_i = gs_pos_i - sc_pos_i;
  sc_to_gs_i = libra::normalize(sc_to_gs_i);
  Quaternion q_i_to_sc_ant = sc_tx_ant.GetQuaternion_b2c() * dynamics.GetAttitude().GetQuaternion_i2b();
  Vector<3> gs_direction_on_sc_frame = q_i_to_sc_ant.frame_conv(sc_to_gs_i);
  double theta_on_sc_ant_rad = acos(gs_direction_on_sc_frame[2]);
  double phi_on_sc_ant_rad = acos(gs_direction_on_sc_frame[0] / sin(theta_on_sc_ant_rad));

  // SC direction on GS RX antenna frame
  Vector<3> gs_to_sc_ecef = dynamics.GetOrbit().GetSatPosition_ecef() - ground_station.GetGSPosition_ecef();
  gs_to_sc_ecef = libra::normalize(gs_to_sc_ecef);
  Quaternion q_ecef_to_gs_ant = gs_rx_ant.GetQuaternion_b2c() * ground_station.GetGSPosition_geo().GetQuaternionXcxfToLtc();
  Vector<3> sc_direction_on_gs_frame = q_ecef_to_gs_ant.frame_conv(gs_to_sc_ecef);
  double theta_on_gs_ant_rad = acos(sc_direction_on_gs_frame[2]);
  double phi_on_gs_ant_rad = acos(sc_direction_on_gs_frame[0] / sin(theta_on_gs_ant_rad));

  // Calc CN0
  double cn0_dBHz = sc_tx_ant.CalcTxEIRP(theta_on_sc_ant_rad, phi_on_sc_ant_rad) + loss_space_dB + loss_polarization_ + loss_atmosphere_ +
                    loss_rainfall_ + loss_others_ + gs_rx_ant.CalcRxGT(theta_on_gs_ant_rad, phi_on_gs_ant_rad) -
                    10 * log10(environment::boltzmann_constant_J_K);
  return cn0_dBHz;
}

std::string GScalculator::GetLogHeader() const {
  std::string str_tmp = "";

  str_tmp += WriteScalar("max bitrate[Mbps]");

  return str_tmp;
}

std::string GScalculator::GetLogValue() const {
  std::string str_tmp = "";

  str_tmp += WriteScalar(max_bitrate_);

  return str_tmp;
}
