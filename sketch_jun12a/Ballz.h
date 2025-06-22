#include <Arduino.h>

class Ballz {
  private:
    static const int SIZE = 10;
    float buffer[SIZE] = {0};
    int index = 0;
    float sum;
  public:
    int potpin;
    float offset=-19.;
    Ballz(int pin);

    float get_pos_raw();
    float get_pos_avr();
    float get_average(float newpos);
};