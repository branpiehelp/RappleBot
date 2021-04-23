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
  this->servo_obj[LEFT_LEG]=servo_osc(0,R_ANGLE);  
  this->servo_obj[RIGHT_LEG]=servo_osc(0,N_ANGLE);  
  this->servo_obj[LEFT_FOOT]=servo_osc(0,R_ANGLE);  
  this->servo_obj[RIGHT_FOOT]=servo_osc(0,N_ANGLE);   
	RappleBot::wakeup();
}

void RappleBot::eye_init(int trig_p, int echo_p)
{
	this->us_trig_pin=trig_p;
	this->us_echo_pin=echo_p;
  this->eye.init(this->us_trig_pin,this->us_echo_pin);
}
void RappleBot::sound_init(int buzzer_pin)
{
	this->buzzer_pin=buzzer_pin;
	sound.init(this->buzzer_pin);
}
signed char RappleBot::get_servo_trim(int sel)
{
	this->servo_obj[sel].set_trim((signed char)EEPROM.read(sel));
	return this->servo_obj[sel].get_trim();
}
void RappleBot::set_servo_trim(int sel, signed char a)
{
	this->servo_obj[sel].set_trim(a);
	EEPROM.write(sel, a);
}
void RappleBot::load_trim_all(){
  byte trim_v;
  //LEG_LEFT_P : In 0 degree, Out: 180 degree 
  trim_v=RappleBot::get_servo_trim(LEFT_LEG);
  this->servo_obj[LEFT_LEG].set_trim(trim_v);  
  //LEG_RIGHT_P : In 0 degree, Out: 180 degree
  trim_v=RappleBot::get_servo_trim(RIGHT_LEG);
  this->servo_obj[RIGHT_LEG].set_trim(trim_v);  
  //FOOT_LEFT_P : Bottom 0 degree, Up: 180 degree 
  trim_v=RappleBot::get_servo_trim(LEFT_FOOT);
  this->servo_obj[LEFT_FOOT].set_trim(trim_v);  
  //FOOT_RIGHT_P : Bottom 0 degree, Up: 180 degree  
  trim_v=RappleBot::get_servo_trim(RIGHT_FOOT);
  this->servo_obj[RIGHT_FOOT].set_trim(trim_v);   
}
void RappleBot::clear_trim_all()
{
  RappleBot::set_servo_trim(LEFT_LEG, 0x00);
  RappleBot::set_servo_trim(RIGHT_LEG, 0x00);
  RappleBot::set_servo_trim(LEFT_FOOT, 0x00);
  RappleBot::set_servo_trim(RIGHT_FOOT, 0x00);
}
void RappleBot::servo_write(int sel, int a)
{
	this->servo_obj[sel].write(a);
}
void RappleBot::legl_angle(int a){
	RappleBot::servo_write(LEFT_LEG,a);
}
void RappleBot::legr_angle(int a){
	RappleBot::servo_write(RIGHT_LEG,a);
}
void RappleBot::footl_angle(int a){
	RappleBot::servo_write(LEFT_FOOT,a);    
}
void RappleBot::footr_angle(int a){
	RappleBot::servo_write(RIGHT_FOOT,a);  
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
	//wakeup();
	do{
		cnt=0;
    	for(int i=0;i<MAX_SERVO_NUM;i++){
			cnt+=this->servo_obj[i].refresh(dt);
		}
	}while(cnt!=0);
	//sleep();
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



//////////////////APPLICATION
/********************************************//** 
@function void RBOT_stand(void)
@brief 
      ____
     |O  O|
      |  |
     -    -
************************************************/
void RappleBot::stand(int dt){
	int act[4]={90,90,90,90};
	RappleBot::action(act, dt);
}
/********************************************//** 
@function void RBOT_vstand(void)
@brief 
      ____
     |O  O|
      |  |
     \    /
************************************************/
void RappleBot::vstand(int dt){//V stand
	int act[4]={90,90,160,160};
	RappleBot::action(act, dt);
}
/********************************************//** 
@function void RBOT_tsand(void)
@brief 
      ____
     |O  O|
      |  |
     |    |
************************************************/
void RappleBot::tstand(int dt){//Toe stand
	int act[4]={90,90,30,30};//lr ll fr fl
	RappleBot::action(act, dt);
}
/********************************************//** 
@function void RBOT_ctand(void)
@brief 
      ____
     |O  O|
       ||
      -  -
************************************************/
void RappleBot::cstand(int dt){//Close stand
	int act[4]={45,45,90,90};//lr ll fr fl
	RappleBot::action(act, dt); 
}
/********************************************//** 
@function void RBOT_otand(void)
@brief 
      ____
     |O  O|
     |    |
    -      -
************************************************/
void RappleBot::ostand(int dt){//Open stand
	int act[4]={135,135,90,90};//lr ll fr fl
	RappleBot::action(act, 2);
}
/********************************************//** 
@function void RBOT_rtand(void)
@brief 
      ____
     |O  O|
      |  \/
     -  
************************************************/
void RappleBot::rstand(int dt){//Right stand
	RappleBot::action(90,-1,35,-1, dt+1);//ll lr fl fr dt
	RappleBot::action(90,-1,35,125, dt);//ll lr fl fr dt
	RappleBot::action(90,-1,90,125, dt);//ll lr fl fr dt
}
/********************************************//** 
@function void RBOT_ltand(void)
@brief 
      ____
     |O  O|
     \|  |
          -
************************************************/
void RappleBot::lstand(int dt){//Left stand
	RappleBot::action(-1,90,-1,35, dt+1);//ll lr fl fr dt
	RappleBot::action(-1,90,125,35, dt);//ll lr fl fr dt
	RappleBot::action(-1,90,125,90, dt);//ll lr fl fr dt
}
/********************************************//** 
@function void stand(void)
@brief 
      ____
     |O  O|
      |  |
     -    -
************************************************/
void RappleBot::stand(enum RappleStand sel, int dt){//All stand action
	if(sel==ATTENTION){
		RappleBot::stand(dt);
	}
	else if(sel==HEEL){
		RappleBot::vstand(dt);    
	}
	else if(sel==TOE){
		RappleBot::tstand(dt);    
	}
	else if(sel==OPEN){
		RappleBot::ostand(dt);    
	}  
	else if(sel==CLOSE){
		RappleBot::cstand(dt);    
	} 
	else if(sel==RSTAND){
		RappleBot::rstand(dt);    
	} 
	else if(sel==LSTAND){
		RappleBot::lstand(dt);    
	} 
}
    
/********************************************//** 
@function void rstep()
char d : direction 'f' forward 'b' back
int vall : left footstep angle 
int valr : right footstep angle
int dt : delay time
@brief 
      ____     ____
     |O  O|   |O  O|
     \|  |     |  |
          -   -    -
************************************************/
void RappleBot::rstep(char d, int val, int dt){
	//Right step
	RappleBot::action(-1,-1,90-val,-1, dt);//ll lr fl fr dt
	RappleBot::action(-1,-1,-1,90+val, dt);//ll lr fl fr dt
	if(d=='f'){
		RappleBot::action(90+val,-1,-1,-1, dt);//ll lr fl fr dt
		RappleBot::action(-1,90-val,-1,-1, dt);//ll lr fl fr dt    
	}
	else if(d=='b'){
		RappleBot::action(90-val,-1,-1,-1, dt);//ll lr fl fr dt    
		RappleBot::action(-1,90+val,-1,-1, dt);//ll lr fl fr dt    
	}
	RappleBot::action(-1,-1,90,90, dt);//ll lr fl fr dt   
}
/********************************************//** 
@function void lstep()
char d : direction 'f' forward 'b' back
int vall : left footstep angle 
int valr : right footstep angle
int dt : delay time
@brief 
      ____     ____
     |O  O|   |O  O|
      |  |/    |  |
     -        -    -
************************************************/
void RappleBot::lstep(char d, int val, int dt){
	//Left step
	RappleBot::action(-1,-1,90+val,-1, dt);//ll lr fl fr dt  
	RappleBot::action(-1,-1,-1,90-val, dt);//ll lr fl fr dt  
	if(d=='f'){
		RappleBot::action(90-val,-1,-1,-1, dt);//ll lr fl fr dt  
		RappleBot::action(-1,90+val,-1,-1, dt);//ll lr fl fr dt  
	}
	else if(d=='b'){
		RappleBot::action(90+val,-1,-1,-1, dt);//ll lr fl fr dt  
		RappleBot::action(-1,90-val,-1,-1, dt);//ll lr fl fr dt  
	}
	RappleBot::action(-1,-1,90,90, dt);//ll lr fl fr dt     
}
/********************************************//** 
@function void fwalk()
int vall : left footstep angle 
int valr : right footstep angle
int dt : delay time
@brief 
      ____     ____     ____     ____
     |O  O|   |O  O|   |O  O|   |O  O|
     \|  |     |  |     |  |/    |  |
          -   -    -   -        -    -
************************************************/
void RappleBot::fwalk(int valL, int valR, int dt){//forward walk
	RappleBot::lstep('f',valL, dt);
	RappleBot::rstep('f',valR, dt);
}
/********************************************//** 
@function void bwalk()
int vall : left footstep angle 
int valr : right footstep angle
int dt : delay time
@brief 
      ____     ____     ____     ____
     |O  O|   |O  O|   |O  O|   |O  O|
     \|  |     |  |     |  |/    |  |
          -   -    -   -        -    -
************************************************/
void RappleBot::bwalk(int valL, int valR, int dt){//Back walk
	RappleBot::lstep('b',valL, dt);  
	RappleBot::rstep('b',valR, dt);
}
void RappleBot::turnRight(int val, int dt){//turnRight
	RappleBot::fwalk(val/2,val,dt);
}
void RappleBot::turnLeft(int val, int dt){//turnLeft
	RappleBot::fwalk(val,val/2,dt);
}
/********************************************//** 
@function void walk()
int vall : left footstep angle 
int valr : right footstep angle
int dt : delay time
@brief 
      ____     ____     ____     ____
     |O  O|   |O  O|   |O  O|   |O  O|
     \|  |     |  |     |  |/    |  |
          -   -    -   -        -    -
************************************************/
void RappleBot::walk(enum RappleWalk sel, int val, int dt){//All walk action
	if(sel==GOFORWARD){
		RappleBot::fwalk(val,val,dt);
	}
	else if(sel==GOBACKWARD){
		RappleBot::bwalk(val,val,dt); 
	}
	else if(sel==TURNLEFT){
		RappleBot::turnLeft(val,dt);    
	}  
	else if(sel==TURNRIGHT){
		RappleBot::turnRight(val,dt); 
	}
}
/********************************************//** 
@function void eye_update()
return : distance of things 
@brief 
************************************************/
long RappleBot::eye_update(void){
   	return this->eye.update();
}
/********************************************//** 
@function void sound_note()
int val : note
int tempo : duration of play
@brief 
************************************************/
void RappleBot::sound_note(enum NOTE tn, int d){
	this->sound.note(tn,d);
}  	
/********************************************//** 
@function void Melody()
int val : Kind of melody 
int tempo : Speed of play
@brief 
************************************************/
void RappleBot::alert(enum A_SOUND val, int tempo)
{
  RappleBot::sound.asound(val,tempo);
}
//End of file
