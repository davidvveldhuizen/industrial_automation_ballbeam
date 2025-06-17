#include "Ballz.h"

Ballz::Ballz(int pin){
  potpin = pin;
  pinMode(pin, INPUT);
}

float Ballz::get_average(float newpos){

  sum -= buffer[index];
  buffer[index] = newpos;
  sum += newpos;

  index = (index + 1) % SIZE;

  return sum/SIZE;
}

float Ballz::get_pos(){
  float pos = 0.07*analogRead(potpin)-24.;
  
  if (pos > -3 || pos < 35){
    if (pos < 0) pos = 0;
    return get_average(pos);
  }else{
    return buffer[index];
  }
}