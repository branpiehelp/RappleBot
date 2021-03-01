/********************************************//** 
(c) 2021 branpie.com <branpiehelp@gmail.com>
Rapple Bot
************************************************/
#ifndef RappleBot_h
#define RappleBot_h
#include <Servo.h>
#include "servo_osc.h"
#include "Melody.h"
#include "Ultrasonic_handler.h"
//Servo_osc array index
#define MAX_SERVO_NUM 4
#define LEFT_LEG   0
#define RIGHT_LEG  1
#define LEFT_FOOT  2
#define RIGHT_FOOT 3
//Servo Angle Direction
#define N_ANGLE 0
#define R_ANGLE 1
//Servo Sleep mode
#define SLEEP_MODE 1
#define WAKEUP_MODE 0
class RappleBot{
private:
	int servo_pin[MAX_SERVO_NUM];
	int us_trig_pin;
	int us_echo_pin;
	int buzzer_pin;
	int servo_sleep;
	servo_osc servo_obj[MAX_SERVO_NUM]; 
  	Ultrasonic_handler eye;
	Melody sound;
public:
	RappleBot();
	RappleBot(int leftleg_p, int rightleg_p, int leftfoot_p, int rightfoot_p);
	void init(int leftleg_p, int rightleg_p, int leftfoot_p, int rightfoot_p);
	//Servo motor
	void servo_write(int pin, int a);
	void legl_angle(int a);
	void legr_angle(int a);
	void footl_angle(int a);
	void footr_angle(int a);
	void sleep();
	void wakeup();
	void commit(int dt=1); //Servo speed 1degree/0.002second
	void action(int ll=-1, int lr=-1, int fl=-1, int fr=-1, int dt=1);
	void action(int act[MAX_SERVO_NUM], int dt=1);

	//Melody
  	void sound_init(int buzzer_pin); 
  	void sound_note(int tn, int d){
    	sound.note(tn,d);
  	}
  	//Ultrasonic
  	void eye_init(int trig_p, int echo_p);
  	long eye_update(void){
    	return eye.update();
  	}
};
#endif
