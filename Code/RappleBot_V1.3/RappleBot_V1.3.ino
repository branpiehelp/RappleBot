/********************************************//** 
© 2021 branpie.com <branpiehelp@gmail.com>
Rapple Bot
************************************************/
#include <SoftwareSerial.h>
#include <Servo.h>
#include "servo_osc.h"
#define MAX_SERVO_NUM 4
#define LEG_LEFT_P    2
#define LEG_RIGHT_P   3
#define FOOT_LEFT_P   4
#define FOOT_RIGHT_P  5

#define N_ANGLE 0
#define R_ANGLE 1
#define BZ_P 13
#define BT_TX_P 6
#define BT_RX_P 7
int Servo_sleep=0;
int arr_pin[MAX_SERVO_NUM]={LEG_LEFT_P,LEG_RIGHT_P,
                            FOOT_LEFT_P,FOOT_RIGHT_P};
float foct[8]=
{261.6, 293.7, 329.6, 349.2, 392.0, 440.0, 493.9, 
1046.5};
enum MELODY{
  C=0,
  D,
  E,
  F,
  G,
  A,
  B,
  C2,
  REST
};
servo_osc arr_servo[MAX_SERVO_NUM] = 
{ servo_osc(0,R_ANGLE),  //LEG_LEFT_P : In 0 degree, Out: 180 degree
  servo_osc(0,N_ANGLE),  //LEG_RIGHT_P : In 0 degree, Out: 180 degree
  servo_osc(0,R_ANGLE),  //FOOT_LEFT_P : BRBOTm 0 degree, Up: 180 degree
  servo_osc(0,N_ANGLE),  //FOOT_RIGHT_P : BRBOTm 0 degree, Up: 180 degree
}; 
SoftwareSerial bt(BT_TX_P, BT_RX_P);
void drv_buzzer_init(void)
{
  pinMode(BZ_P, OUTPUT);
}
void drv_servo_init(void)
{
  arr_servo[0].attach(LEG_LEFT_P);  //PIN 번호, Reverse 각도 mode
  arr_servo[1].attach(LEG_RIGHT_P);
  arr_servo[2].attach(FOOT_LEFT_P);
  arr_servo[3].attach(FOOT_RIGHT_P);
} 
void drv_servo_write(int sel, int a) //서보모터선택, 각도
{
  Serial.println(sel);
  if(sel==LEG_LEFT_P){
    arr_servo[0].write(a);
  }
  else if(sel==LEG_RIGHT_P){
    arr_servo[1].write(a);
  }
  else if(sel==FOOT_LEFT_P){
    arr_servo[2].write(a);
  }
  else if(sel==FOOT_RIGHT_P){
    arr_servo[3].write(a);
  }
}
void RBOT_legl(int a){
  drv_servo_write(LEG_LEFT_P,a);
}
void RBOT_legr(int a){
  drv_servo_write(LEG_RIGHT_P,a);
}
void RBOT_footl(int a){
  drv_servo_write(FOOT_LEFT_P,a);    
}
void RBOT_footr(int a){
  drv_servo_write(FOOT_RIGHT_P,a);  
}

void RBOT_sleep()
{
  for(int i=0;i<MAX_SERVO_NUM;i++){
    arr_servo[i].detach();
  }
  Servo_sleep=1;
}
void RBOT_wakeup()
{
  for(int i=0;i<MAX_SERVO_NUM;i++){
    arr_servo[i].attach(arr_pin[i]);
  }  
  Servo_sleep=0;
}
void RBOT_q(int dt=1) //Servo speed 1degree/0.002second
{
  int cnt;
  //RBOT_wakeup();
  do{
    cnt=0;
    for(int i=0;i<MAX_SERVO_NUM;i++){
      cnt+=arr_servo[i].refresh(dt);
    }
  }while(cnt!=0);
  //RBOT_sleep();
}
void RBOT_action(int ll=-1, int lr=-1, int fl=-1, int fr=-1, int dt=1)
{
  if(ll!=-1)
    RBOT_legl(ll);
  if(lr!=-1)
    RBOT_legr(lr);
  if(fl!=-1)
    RBOT_footl(fl);
  if(fr!=-1)
    RBOT_footr(fr);
  RBOT_q(dt);
}
void RBOT_action(int act[MAX_SERVO_NUM], int dt=1)
{
  if(act[0]!=-1)
    RBOT_legl(act[0]);
  if(act[1]!=-1)
    RBOT_legr(act[1]);
  if(act[2]!=-1)
    RBOT_footl(act[2]);
  if(act[3]!=-1)
    RBOT_footr(act[3]);
  RBOT_q(dt);
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
void RBOT_stand(void){
  int act[4]={90,90,90,90};
  RBOT_action(act, 2);
}
/********************************************//** 
@function void RBOT_vstand(void)
@brief 
      ____
     |O  O|
      |  |
     \    /
************************************************/
void RBOT_vstand(void){//V stand
  int act[4]={90,90,160,160};
  RBOT_action(act, 2);
}
/********************************************//** 
@function void RBOT_tsand(void)
@brief 
      ____
     |O  O|
      |  |
     |    |
************************************************/
void RBOT_tstand(void){//Toe stand
  int act[4]={90,90,30,30};//lr ll fr fl
  RBOT_action(act, 2);
}
/********************************************//** 
@function void RBOT_ctand(void)
@brief 
      ____
     |O  O|
       ||
      -  -
************************************************/
void RBOT_cstand(void){//Close stand
  int act[4]={45,45,90,90};//lr ll fr fl
  RBOT_action(act, 2); 
}
/********************************************//** 
@function void RBOT_otand(void)
@brief 
      ____
     |O  O|
     |    |
    -      -
************************************************/
void RBOT_ostand(void){//Open stand
  int act[4]={135,135,90,90};//lr ll fr fl
  RBOT_action(act, 2);
}
/********************************************//** 
@function void RBOT_rtand(void)
@brief 
      ____
     |O  O|
      |  \/
     -  
************************************************/
void RBOT_rstand(void){//Right stand
  RBOT_action(90,-1,35,-1, 3);//ll lr fl fr dt
  RBOT_action(90,-1,35,125, 2);//ll lr fl fr dt
  RBOT_action(90,-1,90,125, 2);//ll lr fl fr dt
}
/********************************************//** 
@function void RBOT_ltand(void)
@brief 
      ____
     |O  O|
     \|  |
          -
************************************************/
void RBOT_lstand(void){//Left stand

}
/********************************************//** 
@function void RBOT_rstep()
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
void RBOT_rstep(char d, int vall, int valr, int dt){
  //Right step
  RBOT_action(-1,-1,90-vall,-1, dt);//ll lr fl fr dt
  RBOT_action(-1,-1,-1,90+valr, dt);//ll lr fl fr dt
  if(d=='f'){
    RBOT_action(90+vall,-1,-1,-1, dt);//ll lr fl fr dt
    RBOT_action(-1,90-valr,-1,-1, dt);//ll lr fl fr dt    
  }
  else if(d=='b'){
    RBOT_action(90-vall,-1,-1,-1, dt);//ll lr fl fr dt    
    RBOT_action(-1,90+valr,-1,-1, dt);//ll lr fl fr dt    
  }
  RBOT_action(-1,-1,90,90, dt);//ll lr fl fr dt   
}
/********************************************//** 
@function void RBOT_lstep()
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
void RBOT_lstep(char d, int vall, int valr, int dt){
  //Left step
  RBOT_action(-1,-1,90+vall,-1, dt);//ll lr fl fr dt  
  RBOT_action(-1,-1,-1,90-valr, dt);//ll lr fl fr dt  
  if(d=='f'){
    RBOT_action(90-vall,-1,-1,-1, dt);//ll lr fl fr dt  
    RBOT_action(-1,90+valr,-1,-1, dt);//ll lr fl fr dt  
  }
  else if(d=='b'){
    RBOT_action(90+vall,-1,-1,-1, dt);//ll lr fl fr dt  
    RBOT_action(-1,90-valr,-1,-1, dt);//ll lr fl fr dt  
  }
  RBOT_action(-1,-1,90,90, dt);//ll lr fl fr dt     
}
/********************************************//** 
@function void RBOT_fwalk()
int vall : left footstep angle 
int valr : right footstep angle
int dt : delay time
@brief 
      ____     ____     ____     ____
     |O  O|   |O  O|   |O  O|   |O  O|
     \|  |     |  |     |  |/    |  |
          -   -    -   -        -    -
************************************************/
void RBOT_fwalk(int valL, int valR, int dt){//forward walk
  RBOT_rstep('f',valL,valR, dt);
  RBOT_lstep('f',valL,valR, dt);
}
/********************************************//** 
@function void RBOT_bwalk()
int vall : left footstep angle 
int valr : right footstep angle
int dt : delay time
@brief 
      ____     ____     ____     ____
     |O  O|   |O  O|   |O  O|   |O  O|
     \|  |     |  |     |  |/    |  |
          -   -    -   -        -    -
************************************************/
void RBOT_bwalk(int valL, int valR, int dt=1){//Back walk
  RBOT_rstep('b',valL,valR, dt);
  RBOT_lstep('b',valL,valR, dt);  
}
void RBOT_rwalk(int dt=1){//Right walk
  RBOT_fwalk(5,20,dt);
}
void RBOT_lwalk(int dt=1){//Left walk
  RBOT_bwalk(20,5,dt);
}
void RBOT_melody(int tn, int d, int tempo=1)
{
  if(tn!=REST){
    tone(BZ_P, foct[tn]);
    delay(d/tempo);
    noTone(BZ_P);
    delay(100/tempo);
  }
  else{
    delay(d/tempo);
  }
}
void RBOT_music(int val, int tempo=1)
{
  if(val==1){
    RBOT_melody(C, 1000, tempo);
    RBOT_melody(E, 1000, tempo);
    RBOT_melody(G, 1000, tempo);
    RBOT_melody(REST,2000, tempo);
    RBOT_melody(C2, 1000, tempo);
  }  
}
void disp_servo_info()
{
  for(int i=0;i<4;i++)
    arr_servo[i].get_info();
}
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("Hello World!");
  bt.begin(9600);
  bt.println("AT");
  drv_servo_init();
  RBOT_stand();
  drv_buzzer_init();
  RBOT_music(1, 4);
  RBOT_tstand();
  delay(1000);
  RBOT_stand();
}
char bt_myCmd;
String myCmd="";
void loop() {
  // put your main code here, to run repeatedly:
  while(bt.available()){
    bt_myCmd=(char)bt.read();
  }
  if(bt_myCmd!=0){
    if(bt_myCmd=='f'){
      RBOT_fwalk(20, 20, 1);
    }
    else if(bt_myCmd=='b'){
      RBOT_bwalk(20, 20, 1);
    }
    else if(bt_myCmd=='l'){
      RBOT_lwalk(1);
    }
    else if(bt_myCmd=='r'){
      RBOT_rwalk(1);
    }
    else if(bt_myCmd=='s'){
      RBOT_stand();
    }
    else if(bt_myCmd=='v'){
      RBOT_vstand();
    }
    else if(bt_myCmd=='c'){
      RBOT_cstand();
    }
    else if(bt_myCmd=='o'){
      RBOT_ostand();
    }
    else if(bt_myCmd=='a'){
      RBOT_rstand();
    }
  }
  while(Serial.available()){
    myCmd+=(char)Serial.read();
    delay(5);
  }
  if(!myCmd.equals("")){
      myCmd.replace("\n","");
      myCmd.replace("\r","");    
      Serial.println(">>>"+myCmd);
      String cmd=myCmd.substring(0, myCmd.indexOf(" "));

      if(cmd.equals("stand")){
        RBOT_stand();
      }
      else if(cmd.equals("tstand")){
        RBOT_tstand();
      }
      else if(cmd.equals("vstand")){
        RBOT_vstand();
      }
      else if(cmd.equals("cstand")){
        RBOT_cstand();
      }
      else if(cmd.equals("ostand")){
        RBOT_ostand();
      }
      else if(cmd.equals("rstand")){
        RBOT_rstand();
      }
      else if(cmd.equals("fw")){
        int vall, valr, sp;//left angle, right angle, speed:1~5
        sscanf(myCmd.c_str(),"fw %d %d %d",&vall,&valr,&sp);
        RBOT_fwalk(vall, valr, sp);
      }
      else if(cmd.equals("bw")){
        RBOT_bwalk(25,25,2);
      }
      else if(cmd.equals("rw")){
        RBOT_rwalk(2);
      }
      else if(cmd.equals("lw")){
        RBOT_lwalk(2);
      }
      else if(cmd.equals("info")){
        disp_servo_info();
      }
      else if(cmd.equals("angle")){
        int sel, val;
        sscanf(myCmd.c_str(),"angle %d %d",&sel,&val);
        Serial.println("angle OK");
        drv_servo_write(sel, val);
        RBOT_q(10);
      }
  }
  myCmd="";
  bt_myCmd=0;  
}
