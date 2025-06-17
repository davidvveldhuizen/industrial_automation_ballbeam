#include "PID.h"
#include <Arduino.h>

PID::PID(float p, float i, float d){
  P=p;
  I=i;
  D=d;
}

void  PID::set_setpoint(float p){
  setpoint = p;
}

float PID::get_pid_output(float input, float dt){
  
  float error =  setpoint - input;
  if (abs(error) < 2) error = 0;

  error_sum += error * dt;
  error_sum = min(max(-10, error_sum), 10);

  float dir = (error-last_error)/dt;
  float output=  (error*P) + (dir*D) + (error_sum * I);
  last_error = error;
  return output;
}