#include "AccelStepper.h"
#include "PID.h"

class BeamControl{
private:
  static const int SIZE = 3;
  float buffer[SIZE] = {0};
  int index = 0;
  float sum;
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
  float get_avr_angle();
  void update(float dt);
  void enable();
  void disable();
};