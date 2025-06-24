#include "AccelStepper.h"
#include "beamcontrol.h"
#include "Ballz.h"
#include "PID.h"
#include "string.h"

#include <SPI.h>
#include <Ethernet.h>

#include <ArduinoRS485.h> // ArduinoModbus depends on the ArduinoRS485 library
#include <ArduinoModbus.h>

BeamControl beam(13, 21, A11, 16);
Ballz ballz(A13);
PID pid(0.3, 0, 8);


byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
};
IPAddress ip(192, 168, 1, 120);

EthernetServer ethServer(502);

ModbusTCPServer modbusTCPServer;

bool run;

void modbus_setup(){
  Serial.println("Ethernet Modbus TCP Example");

  // start the Ethernet connection and the server:
  Ethernet.begin(mac, ip);

  // Check for Ethernet hardware present
  if (Ethernet.hardwareStatus() == EthernetNoHardware) {
    Serial.println("Ethernet shield was not found.  Sorry, can't run without hardware. :(");
    while (true) {
      delay(1); // do nothing, no point running without Ethernet hardware
    }
  }
  if (Ethernet.linkStatus() == LinkOFF) {
    Serial.println("Ethernet cable is not connected.");
  }

  // start the server
  ethServer.begin();
  
  // start the Modbus TCP server
  if (!modbusTCPServer.begin()) {
    Serial.println("Failed to start Modbus TCP Server!");
    while (1);
  }

  // configure a single coil at address 0x00
  modbusTCPServer.configureCoils(0x00, 22);

  EthernetClient client = ethServer.available();
  
  if (client) {
    // a new client connected
    Serial.println("new client");

    // let the Modbus TCP accept the connection 
    modbusTCPServer.accept(client);
  }
}

void setup()
{
  Serial.begin(115200);
  beam.set_angle(-30);
  pid.set_setpoint(15);
  pinMode(17, OUTPUT);
  digitalWrite(17, 1);
  modbus_setup();
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
  EthernetClient client = ethServer.available();
  
  if (client) {
    // a new client connected
    Serial.println("new client");

    // let the Modbus TCP accept the connection 
    modbusTCPServer.accept(client);

    while (client.connected()) {
      // poll for Modbus TCP requests, while client connected
      //delay(1000);
      t = micros();
      float dt = (t - pt)*0.001;
      pt = t;

      if (t > timer)
      {
        timer += interval;

        modbusTCPServer.poll();
        uint8_t value = 0;

        for (int i=9;i<17;i++){
          bool bit = modbusTCPServer.coilRead(i);
          value |= (bit<<i-9);
        }
        uint8_t type = 0;
        for (int i=17;i<20;i++){
          bool bit = modbusTCPServer.coilRead(i);
          type |= (bit<<i-17);
        }

        switch (type){
          case 0: pid.P = ((float)value)/10.; break;
          case 1: pid.I = (float)value/10; break;          
          case 2: pid.D = (float)value/10; break;
          case 3: pid.set_setpoint(value); break;
          case 4: run = false; break;
          case 5: run = true; digitalWrite(17, 0); break;
          case 7: run = false; digitalWrite(17, 1); break;
        }
        
        //Serial.println(modbusTCPServer.holdingRegisterWrite(0, 100));
        //delay(10);
        //Serial.println(modbusTCPServer.coilWrite(0,1));
        //delay(10);

        float pos = ballz.get_pos_avr();
        float output = pid.get_pid_output(pos, dt);

        //output = min(max(-30,output),30);
        if (run){
          beam.set_angle(output);
        }else{
          beam.set_angle(0);
        }
        
        //beam.set_angle(0);
    
        debug_index ++;
        if (debug_index > debug_interval){
          debug_index = 0;

          liston_for_commands();

          Serial.print(beam.get_angle());
          
          Serial.print(",");
          Serial.print(output);
          Serial.print(",");
          
          Serial.print(pos);
          Serial.print(",");

          Serial.println(ballz.get_pos_raw());
        }
      }
      beam.update(dt);
    }

    Serial.println("client disconnected");
  }

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
      case 'o': ballz.offset += value; break;
      case 'S': pid.set_setpoint(value); break;
      case '?':
        Serial.print("P: ");
        Serial.print(pid.P);
        Serial.print(" I: ");
        Serial.print(pid.I);
        Serial.print(" D: ");
        Serial.print(pid.D);
        Serial.print(" angle offset: ");
        Serial.print(beam.angle_offset);
        Serial.print(" pos offset: ");
        Serial.print(ballz.offset);
        Serial.print(" setpoint: ");
        Serial.println(pid.setpoint);
      default: Serial.println("Invalid input"); return;
    }
  }
}
