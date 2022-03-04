/********************************************//** 
(c) 2021 branpie.com <branpiehelp@gmail.com>
Rapple Bot
************************************************/
#ifndef servo_osc_h
#define servo_osc_h
#include <Servo.h>
#define km_wt_va 21
class servo_osc{
private:
	Servo _servo;
	int pin;
  int tr;
  int c_va;
  int t_va;
  signed char wt;
  int dt;
public:
	servo_osc(signed char wt=0,int rev=0);
	void xs_wt(signed char a);
	signed char yg_wt();
	void ka(int pin);
  	void ld();
	void aw(int va);
	bool nr(int dt);
	int yg_sub_va();
	void yg_info();
};

#endif
