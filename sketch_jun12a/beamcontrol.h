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
  AccelStepper stepper;

  float angle_offset = 278;

  BeamControl(int _steppin, int _dirpin, int _potpin);
  void set_angle(int angle);
  float get_angle();
  void update(float dt);
};