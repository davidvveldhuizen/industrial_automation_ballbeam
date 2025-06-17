#include "AccelStepper.h"
#include "beamcontrol.h"
#include "Ballz.h"
#include "PID.h"
#include "string.h"

BeamControl beam(2, 3, A0);
Ballz ballz(A1);
PID pid(0.2, 0, 0);

void setup()
{
  Serial.begin(115200);
  beam.set_angle(-30);
  pid.set_setpoint(15);
}

unsigned long t = micros();
unsigned long pt = t;
unsigned long timer;
unsigned long interval = 10000;

int debug_index;
int debug_interval= 20;

void liston_for_commands();

void loop()
{
  t = micros();
  float dt = (t - pt)*0.001;
  pt = t;

  if (t > timer)
  {
    timer += interval;
    float pos = ballz.get_pos();
    float output = pid.get_pid_output(pos, dt);

    //output = min(max(-30,output),30);
    beam.set_angle(output);
    //beam.set_angle(15);

    debug_index ++;
    if (debug_index > debug_interval){
      debug_index = 0;

      liston_for_commands();

      //Serial.print(beam.current_angle);
      //Serial.print(",-30,30,");
      //Serial.print(",-5,40,");
      //Serial.print(", ballz pos: ");
      //Serial.print(",");
      //Serial.print(pos);
      //Serial.print(",");
      //Serial.print(", output: ");
      //Serial.print(", ");

      //Serial.print(dt);
      //Serial.print(", ");
      //int a[3] = {1,2,3};
      //Serial.print(a);
      Serial.println(analogRead(ballz.potpin));
    }
  }

  beam.update(dt);
}

void liston_for_commands(){
  if (Serial.available() > 0) {

    String data = Serial.readString();
    
    char type = data.charAt(0);
    float value = data.substring(1).toFloat();
    
    switch (type) {
      case 'P': pid.P = value; break;
      case 'I': pid.I = value; break;
      case 'D': pid.D = value; break;
      case 'O': beam.angle_offset += value; break;
      case '?':
        Serial.print("P: ");
        Serial.print(pid.P);
        Serial.print(" I: ");
        Serial.print(pid.I);
        Serial.print(" D: ");
        Serial.print(pid.D);
        Serial.print(" angle offset: ");
        Serial.println(beam.angle_offset);
      default: Serial.println("Invalid input"); return;
    }
  }
}
