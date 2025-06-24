#include "AccelStepper.h"
#include "beamcontrol.h"
#include "PID.h"

BeamControl::BeamControl(int _steppin, int _dirpin, int _potpin, int _enablepin): pid(0,0,0){
  steppin = _steppin;
  dirpin = _dirpin;
  potpin = _potpin;
  enablepin = _enablepin;
  pinMode(potpin, INPUT);
  pinMode(enablepin, OUTPUT);

  stepper = AccelStepper(AccelStepper::DRIVER, steppin, dirpin);
  stepper.setCurrentPosition(get_angle());
  stepper.setMaxSpeed(400);
  stepper.setAcceleration(4000);
  stepper.setCurrentPosition(get_angle());
  
  //pid = PID(1,1);
}

void BeamControl::set_angle(int angle)
  {
    target_angle = angle;
    //stepper.moveTo((angle*100)+angle_offset);
    //pid.set_setpoint(angle);
  }

float BeamControl::get_angle()
  {
    return -0.35 * (float)analogRead(potpin) + angle_offset;
  }

void BeamControl::update(float dt)
  {
    current_angle = get_angle();
    error = (target_angle-current_angle);
    if (abs(error) < 1) error = 0;
    int output =  error * 70;

    stepper.setSpeed(output);
    stepper.runSpeed();
    //stepper.run();
  }

void BeamControl::enable(){
  digitalWrite(enablepin, 0);
}

void BeamControl::disable(){
  digitalWrite(enablepin, 1);
}