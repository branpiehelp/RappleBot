/********************************************//** 
(c) 2021 branpie.com <branpiehelp@gmail.com>
Rapple Bot
************************************************/
#ifndef servo_osc_h
#define servo_osc_h
#include <Servo.h>
#define MAX_TRIM_ANGLE 20
class servo_osc{
private:
	Servo _servo;
	int pin;
  int reverse;
  int c_angle;
  int t_angle;
  signed char trim;
  int dt;
public:
	servo_osc(signed char trim=0,int rev=0);
	void set_trim(signed char a);
	signed char get_trim();
	void attach(int pin);
  void detach();
	void write(int angle);
	bool refresh(int dt);
	int get_sub_angle();
	void get_info();
};

#endif
