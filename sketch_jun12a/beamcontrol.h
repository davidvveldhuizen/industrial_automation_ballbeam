#include "AccelStepper.h"
#include "PID.h"

class BeamControl{
private:
  int steps = 200;
public:
  float current_angle;
  float target_angle;

  int steppin;
  int dirpin;
  int potpin;
  int enablepin;
  AccelStepper stepper;

  int error;

  float angle_offset = 277.;
  PID pid;

  BeamControl(int _steppin, int _dirpin, int _potpin, int _enablepin);
  void set_angle(int angle);
  float get_angle();
  void update(float dt);
  void enable();
  void disable();
};