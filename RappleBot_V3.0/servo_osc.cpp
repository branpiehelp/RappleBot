/********************************************//** 
(c) 2021 branpie.com <branpiehelp@gmail.com>
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
servo_osc::servo_osc(signed char trim,int rev) 
{
  	this->trim=trim; 
  	this->reverse=rev;
  	this->c_angle=90;
  	this->t_angle=90;
};
void servo_osc::set_trim(signed char a)
{
	this->trim=a;
}
signed char servo_osc::get_trim()
{
	return this->trim;
}
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
      this->t_angle=180-angle;
      if(abs(this->trim)<MAX_TRIM_ANGLE){//Maximum trim Angle
        this->t_angle-=this->trim;
      }
    }
    else{
      this->t_angle=angle;
      if(abs(this->trim)<MAX_TRIM_ANGLE){//Maximum trim Angle
        this->t_angle+=this->trim;
      }
    }

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
