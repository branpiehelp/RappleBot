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
#include "RappleBot.h"
RappleBot::RappleBot()
{
}
RappleBot::RappleBot(int leftleg_p, int rightleg_p, int leftfoot_p, int rightfoot_p)
{
	RappleBot::init(leftleg_p, rightleg_p, leftfoot_p, rightfoot_p);
}
void RappleBot::init(int leftleg_p, int rightleg_p, int leftfoot_p, int rightfoot_p)
{
	this->servo_sleep=SLEEP_MODE;
	this->servo_pin[LEFT_LEG]=leftleg_p;
	this->servo_pin[RIGHT_LEG]=rightleg_p;
	this->servo_pin[LEFT_FOOT]=leftfoot_p;
	this->servo_pin[RIGHT_FOOT]=rightfoot_p;
	this->servo_obj[LEFT_LEG]=servo_osc(0,R_ANGLE);  //LEG_LEFT_P : In 0 degree, Out: 180 degree
    this->servo_obj[RIGHT_LEG]=servo_osc(0,N_ANGLE);  //LEG_RIGHT_P : In 0 degree, Out: 180 degree
    this->servo_obj[LEFT_FOOT]=servo_osc(0,R_ANGLE);  //FOOT_LEFT_P : Bottom 0 degree, Up: 180 degree
    this->servo_obj[RIGHT_FOOT]=servo_osc(0,N_ANGLE);  //FOOT_RIGHT_P : Bottom 0 degree, Up: 180 degree	
	RappleBot::wakeup();
}

void RappleBot::eye_init(int trig_p, int echo_p)
{
	this->us_trig_pin=trig_p;
	this->us_echo_pin=echo_p;
  	Ultrasonic_handler(this->us_trig_pin,this->us_echo_pin);
}
void RappleBot::sound_init(int buzzer_pin)
{
	this->buzzer_pin=buzzer_pin;
  	sound.init(this->buzzer_pin);
}
void RappleBot::servo_write(int pin, int a)
{
  if(pin==this->servo_pin[LEFT_LEG]){
    this->servo_obj[LEFT_LEG].write(a);
  }
  else if(pin==this->servo_pin[RIGHT_LEG]){
    this->servo_obj[RIGHT_LEG].write(a);
  }
  else if(pin==this->servo_pin[LEFT_FOOT]){
    this->servo_obj[LEFT_FOOT].write(a);
  }
  else if(pin==this->servo_pin[RIGHT_FOOT]){
    this->servo_obj[RIGHT_FOOT].write(a);
  }
}
void RappleBot::legl_angle(int a){
  RappleBot::servo_write(this->servo_pin[LEFT_LEG],a);
}
void RappleBot::legr_angle(int a){
  RappleBot::servo_write(this->servo_pin[RIGHT_LEG],a);
}
void RappleBot::footl_angle(int a){
  RappleBot::servo_write(this->servo_pin[LEFT_FOOT],a);    
}
void RappleBot::footr_angle(int a){
  RappleBot::servo_write(this->servo_pin[RIGHT_FOOT],a);  
}

void RappleBot::sleep()
{
  for(int i=0;i<MAX_SERVO_NUM;i++){
    this->servo_obj[i].detach();
  }
  this->servo_sleep=SLEEP_MODE;
}
void RappleBot::wakeup()
{
  for(int i=0;i<MAX_SERVO_NUM;i++){
    this->servo_obj[i].attach(servo_pin[i]);
  }  
  this->servo_sleep=WAKEUP_MODE;
}
void RappleBot::commit(int dt) //Servo speed 1degree/0.002second
{
  int cnt;
  //RBOT_wakeup();
  do{
    cnt=0;
    for(int i=0;i<MAX_SERVO_NUM;i++){
      cnt+=this->servo_obj[i].refresh(dt);
    }
  }while(cnt!=0);
  //RBOT_sleep();
}
void RappleBot::action(int ll, int lr, int fl, int fr, int dt)
{
  if(ll!=-1)
    RappleBot::legl_angle(ll);
  if(lr!=-1)
    RappleBot::legr_angle(lr);
  if(fl!=-1)
    RappleBot::footl_angle(fl);
  if(fr!=-1)
    RappleBot::footr_angle(fr);
  RappleBot::commit(dt);
}
void RappleBot::action(int act[MAX_SERVO_NUM], int dt)
{
  if(act[LEFT_LEG]!=-1)
    RappleBot::legl_angle(act[LEFT_LEG]);
  if(act[RIGHT_LEG]!=-1)
    RappleBot::legr_angle(act[RIGHT_LEG]);
  if(act[LEFT_FOOT]!=-1)
    RappleBot::footl_angle(act[LEFT_FOOT]);
  if(act[RIGHT_FOOT]!=-1)
    RappleBot::footr_angle(act[RIGHT_FOOT]);
  RappleBot::commit(dt);
}
