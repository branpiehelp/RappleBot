/********************************************//** 
© 2021 branpie.com <branpiehelp@gmail.com>
Rapple Bot
************************************************/
#if defined(ARDUINO) && ARDUINO >= 100
  #include "Arduino.h"
#else
  #include "WProgram.h"
  #include <pins_arduino.h>
#endif
#include "servo_osc.h"
#include <Servo.h>
void servo_osc::attach(int pin)
{
    _servo.attach(pin);
    this->pin=pin;
}
void servo_osc::detach()
{
  if(_servo.attached())
        _servo.detach();
}
void servo_osc::write(int angle)
{
    if(this->reverse){
      angle=180-angle;
    }
    this->t_angle=angle;
}
bool servo_osc::refresh(int dt)
{
    int tmp;
    if(this->c_angle==this->t_angle)
      return false;
    this->dt=dt;
    tmp=this->c_angle-this->t_angle;
    if(tmp < 0){
      this->c_angle++;
    }
    else if(tmp > 0){
       this->c_angle--;
    }
    else{
      this->c_angle=this->t_angle;      
    }
    //Serial.println(this->c_angle-this->t_angle);
    _servo.write(this->c_angle);
    delay(this->dt);
    return true;
}
int servo_osc::get_sub_angle()
{
  return this->c_angle-this->t_angle;
}
void servo_osc::get_info()
{
  Serial.print("Current Angle:");
  Serial.println(this->c_angle);
  Serial.print("pin number:");
  Serial.println(this->pin);
  Serial.print("reverse mode:");
  Serial.println(this->reverse);
  Serial.print("delay time:");
  Serial.println(this->dt);
}
