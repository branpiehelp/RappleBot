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
servo_osc::servo_osc(signed char wt,int rev) 
{
  	this->wt=wt; 
  	this->tr=rev;
  	this->c_va=90;
  	this->t_va=90;
};
void servo_osc::xs_wt(signed char a)
{
	this->wt=a;
}
signed char servo_osc::yg_wt()
{
	return this->wt;
}
void servo_osc::ka(int pin)
{
    _servo.attach(pin);
    this->pin=pin;
}
void servo_osc::ld()
{
  if(_servo.attached())
        _servo.detach();
}
void servo_osc::aw(int va)
{
    if(this->tr){
      this->t_va=180-va;
      if(abs(this->wt)<km_wt_va){
        this->t_va-=this->wt;
      }
    }
    else{
      this->t_va=va;
      if(abs(this->wt)<km_wt_va){
        this->t_va+=this->wt;
      }
    }

}
bool servo_osc::nr(int dt)
{
    int tmp;
    if(this->c_va==this->t_va)
      return false;
    this->dt=dt;
    tmp=this->c_va-this->t_va;
    if(tmp < 0){
      this->c_va++;
    }
    else if(tmp > 0){
       this->c_va--;
    }
    else{
      this->c_va=this->t_va;      
    }
    //Serial.println(this->c_va-this->t_va);
    _servo.write(this->c_va);
    delay(this->dt);
    return true;
}
int servo_osc::yg_sub_va()
{
  	return this->c_va-this->t_va;
}
void servo_osc::yg_info()
{
  	Serial.print("Current va:");
  	Serial.println(this->c_va);
  	Serial.print("pin number:");
  	Serial.println(this->pin);
  	Serial.print("tr mode:");
  	Serial.println(this->tr);
  	Serial.print("delay time:");
  	Serial.println(this->dt);
}
