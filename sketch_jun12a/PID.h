#ifndef PID_H
#define PID_H

class PID {
  public:
  float P;
  float I;
  float D;
  float derivative;
  float setpoint;
  float last_error;
  float error_sum;
  float error;

  PID(float p, float i, float d);

  void set_setpoint(float p);

  float get_pid_output(float input, float dt);
};

#endif