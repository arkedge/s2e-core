#pragma once
#include "../../../Library/math/Vector.hpp"
#include "../../../Dynamics/Dynamics.h"
#include "../../../Simulation/Spacecraft/Structure/Structure.h"
#include "../../../Component/Power/PCU.h"
#include "../../../Component/CDH/OBC.h"
#include "../../../Component/AOCS/Gyro.h"

using libra::Vector;
class OBC;
class PCU;
class Gyro;

class SampleComponents
{
public:
  SampleComponents(const Dynamics* dynamics, const Structure* structure, const SimulationConfig* config, ClockGenerator* clock_gen, const int sat_id);
  ~SampleComponents();
  Vector<3> GenerateForce_b();
  Vector<3> GenerateTorque_b();
  void CompoLogSetUp(Logger& logger);
private:
  PCU* pcu_;
  OBC* obc_;
  Gyro* gyro_;
  const SimulationConfig* config_;
  const Dynamics* dynamics_;
  const Structure* structure_;
};
