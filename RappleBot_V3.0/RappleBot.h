/********************************************//** 
(c) 2021 branpie.com <branpiehelp@gmail.com>
Rapple Bot
************************************************/
#ifndef RappleBot_h
#define RappleBot_h
#include <EEPROM.h>
#include <Servo.h>
#include "servo_osc.h"
#include "Melody.h"
#include "Ultrasonic_handler.h"
//Servo Angle Direction
#define N_ANGLE 0
#define R_ANGLE 1
//Servo Sleep mode
#define SLEEP_MODE 1
#define WAKEUP_MODE 0
#define CAL_UPDATE 0xDD
//Servo_osc array index
#define	LEFT_LEG 0
#define RIGHT_LEG 1
#define	LEFT_FOOT 2
#define	RIGHT_FOOT 3
#define	MAX_SERVO_NUM 4 

enum RappleStand{
	ATTENTION=1,
	HEEL,
	TOE,
	CLOSE,
	OPEN,
	RSTAND,
	LSTAND,
};
enum RappleWalk{
	GOFORWARD=1,
	GOBACKWARD,	
	TURNLEFT,
	TURNRIGHT,
};
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
	void set_servo_trim(int sel, signed char a);	
	signed char get_servo_trim(int sel);
  void load_trim_all();    
  void clear_trim_all();		
	void servo_write(int sel, int a);
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
	void sound_note(enum NOTE tn, int d);
	//Ultrasonic
  void eye_init(int trig_p, int echo_p);
  long eye_update(void);
  
  //APPLICATION
  //Stand action
  void stand(int dt=2);
  void vstand(int dt=2);//Heel stand
  void tstand(int dt=2);//Toe stand
  void cstand(int dt=2);//Close stand
  void ostand(int dt=2);//Open stand
  void rstand(int dt=2);//Right stand
  void lstand(int dt=2);//Left stand
  void stand(enum RappleStand sel, int dt=2);
  //walk action
  void rstep(char d, int val, int dt=2);
  void lstep(char d, int val, int dt=2);
  void fwalk(int valL, int valR, int dt=2);//forward walk
  void bwalk(int valL, int valR, int dt=2);//Back walk
	void turnRight(int val, int dt=2);//turnRight
	void turnLeft(int val, int dt=2);//turnLeft
	void walk(enum RappleWalk sel, int val, int dt=1);//All walk action
	//Sound
  void alert(enum A_SOUND val, int tempo=1);
};
#endif
