#include <SoftwareSerial.h>
#include <Servo.h>
#include "servo_osc.h"
//#define NO_SLEEP_MODE
//#define DEBUG_MODE
#define MAX_SERVO_NUM 4
#define LEG_RIGHT_P   3
#define LEG_LEFT_P    2
#define FOOT_RIGHT_P  5
#define FOOT_LEFT_P   4

#define BZ_P 13
#define BT_TX_P 6
#define BT_RX_P 7
int arr_pin[MAX_SERVO_NUM]={LEG_RIGHT_P,LEG_LEFT_P,FOOT_RIGHT_P,FOOT_LEFT_P};
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
servo_osc arr_servo[MAX_SERVO_NUM];
//Servo s1, s2, s3, s4;
SoftwareSerial bt(BT_TX_P, BT_RX_P);
void drv_buzzer_init(void)
{
  pinMode(BZ_P, OUTPUT);
}
void drv_servo_init(void)
{
  arr_servo[0].attach(LEG_RIGHT_P);
  arr_servo[1].attach(LEG_LEFT_P);
  arr_servo[2].attach(FOOT_RIGHT_P);
  arr_servo[3].attach(FOOT_LEFT_P);
} 
void drv_servo_write(int sel, int a) //서보모터선택, 각도
{
  if(sel==LEG_RIGHT_P){
    arr_servo[0].write(a);
  }
  else if(sel==LEG_LEFT_P){
    arr_servo[1].write(180-a);
  }
  else if(sel==FOOT_RIGHT_P){
    arr_servo[2].write(a);
  }
  else if(sel==FOOT_LEFT_P){
    arr_servo[3].write(180-a);
  }
}
void otto_legr(int a){
  drv_servo_write(LEG_RIGHT_P,a);
}
void otto_legl(int a){
  drv_servo_write(LEG_LEFT_P,a);
}
void otto_footr(int a){
  drv_servo_write(FOOT_RIGHT_P,a);  
}
void otto_footl(int a){
  drv_servo_write(FOOT_LEFT_P,a);    
}
void otto_sleep()
{
  for(int i=0;i<MAX_SERVO_NUM;i++){
    arr_servo[i].detach();
  }
}
void otto_wakeup()
{
  for(int i=0;i<MAX_SERVO_NUM;i++){
    arr_servo[i].attach(arr_pin[i]);
  }  
}
void otto_q(int dt=10)
{
  int cnt;
  otto_wakeup();
  do{
    cnt=0;
    for(int i=0;i<MAX_SERVO_NUM;i++){
      cnt+=arr_servo[i].refresh(dt);
    }
  }while(cnt!=0);
#ifdef DEBUG_MODE
  delay(1000);//debug
#endif
#ifndef NO_SLEEP_MODE
  otto_sleep();
#endif
}
void otto_action(int lr=-1, int ll=-1, int fr=-1, int fl=-1, int dt=10)
{
  if(lr!=-1)
    otto_legr(lr);
  if(ll!=-1)
    otto_legl(ll);
  if(fr!=-1)
    otto_footr(fr);
  if(fl!=-1)
    otto_footl(fl);
  otto_q(dt);
}
void otto_action(int act[MAX_SERVO_NUM], int dt=10)
{
  if(act[0]!=-1)
    otto_legr(act[0]);
  if(act[1]!=-1)
    otto_legl(act[1]);
  if(act[2]!=-1)
    otto_footr(act[2]);
  if(act[3]!=-1)
    otto_footl(act[3]);
  otto_q(dt);
}
//////////////////APPLICATION
/********************************************//** 
@function void otto_stand(void)
@brief 
      ____
     |O  O|
      |  |
     -    -
************************************************/
void otto_stand(void){
  int act[4]={90,90,90,90};
  otto_action(act, 10);
}
/********************************************//** 
@function void otto_vstand(void)
@brief 
      ____
     |O  O|
      |  |
     \    /
************************************************/
void otto_vstand(void){//V stand
  int act[4]={90,90,160,160};
  otto_action(act, 5);
}
/********************************************//** 
@function void otto_tsand(void)
@brief 
      ____
     |O  O|
      |  |
     |    |
************************************************/
void otto_tstand(void){//Toe stand
  int act[4]={90,90,30,30};//lr ll fr fl
  otto_action(act, 5);
}
/********************************************//** 
@function void otto_ctand(void)
@brief 
      ____
     |O  O|
       ||
      -  -
************************************************/
void otto_cstand(void){//Close stand
  otto_legr(45);
  otto_legl(45);
  otto_footr(90);
  otto_footl(90);
  otto_q();  
}
/********************************************//** 
@function void otto_otand(void)
@brief 
      ____
     |O  O|
     |    |
    -      -
************************************************/
void otto_ostand(void){//Open stand

}
/********************************************//** 
@function void otto_rtand(void)
@brief 
      ____
     |O  O|
      |  \/
     -  
************************************************/
void otto_rstand(void){//Right stand
  otto_legl(45);
  otto_footr(45);
  otto_footl(135);
  otto_q();
  otto_legl(90);
  otto_footr(90);
  otto_q();
}
/********************************************//** 
@function void otto_ltand(void)
@brief 
      ____
     |O  O|
     \|  |
          -
************************************************/
void otto_lstand(void){//Left stand

}
/********************************************//** 
@function void otto_rstep()
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
void otto_rstep(char d, int vall, int valr, int dt){
  //Right step
  otto_action(-1,-1,-1,90-vall, dt);//lr ll fr fl dt
  otto_action(-1,-1,90+valr,-1, dt);//lr ll fr fl dt
  if(d=='f'){
    otto_action(-1,90+vall,-1,-1, dt);//lr ll fr fl dt
    otto_action(90-vall,-1,-1,-1, dt);//lr ll fr fl dt    
  }
  else if(d=='b'){
    otto_action(-1,90-vall,-1,-1, dt);//lr ll fr fl dt    
    otto_action(90+valr,-1,-1,-1, dt);//lr ll fr fl dt    
  }
  otto_action(-1,-1,90,90, dt);//lr ll fr fl dt   
}
/********************************************//** 
@function void otto_lstep()
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
void otto_lstep(char d, int vall, int valr, int dt){
  //Left step
  otto_action(-1,-1,90-valr,-1, dt);//lr ll fr fl dt
  otto_action(-1,-1,-1,90+vall, dt);//lr ll fr fl dt
  if(d=='f'){
    otto_action(90+valr,-1,-1,-1, dt);//lr ll fr fl dt
    otto_action(-1,90-vall,-1,-1, dt);//lr ll fr fl dt 
  }
  else if(d=='b'){
    otto_action(90-valr,-1,-1,-1, dt);//lr ll fr fl dt
    otto_action(-1,90+vall,-1,-1, dt);//lr ll fr fl dt 
  }
  otto_action(-1,-1,90,90, dt);//lr ll fr fl dt     
}
/********************************************//** 
@function void otto_fwalk()
int vall : left footstep angle 
int valr : right footstep angle
int dt : delay time
@brief 
      ____     ____     ____     ____
     |O  O|   |O  O|   |O  O|   |O  O|
     \|  |     |  |     |  |/    |  |
          -   -    -   -        -    -
************************************************/
void otto_fwalk(int valL, int valR, int dt){//forward walk
  otto_rstep('f',valL,valR, dt);
  otto_lstep('f',valL,valR, dt);
}
/********************************************//** 
@function void otto_bwalk()
int vall : left footstep angle 
int valr : right footstep angle
int dt : delay time
@brief 
      ____     ____     ____     ____
     |O  O|   |O  O|   |O  O|   |O  O|
     \|  |     |  |     |  |/    |  |
          -   -    -   -        -    -
************************************************/
void otto_bwalk(int dt=10){//Back walk
  otto_rstep('b',25,25, dt);
  otto_lstep('b',25,25, dt);  
}
void otto_rwalk(void){//Right walk
  otto_legr(90);
  otto_legl(90);
  otto_footr(30);
  otto_footl(60);   
  delay(500);
  otto_stand();
}
void otto_melody(int tn, int d, int tempo=1)
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
void otto_music(int val, int tempo=1)
{
  if(val==1){
    otto_melody(C, 1000, tempo);
    otto_melody(E, 1000, tempo);
    otto_melody(G, 1000, tempo);
    otto_melody(REST,2000, tempo);
    otto_melody(C2, 1000, tempo);
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
  drv_buzzer_init();
  otto_music(1, 4);
  otto_tstand();
  delay(1000);
  otto_stand();
}
String myCmd="";
void loop() {
  // put your main code here, to run repeatedly:
  while(bt.available()){
    //char cmd=(char)bt.read();
    //Serial.write(cmd);
    myCmd+=(char)bt.read();
    delay(5);
  }
  while(Serial.available()){
    //char cmd=(char)Serial.read();
    //bt.write(cmd);
    myCmd+=(char)Serial.read();
    delay(5);
  }
  if(!myCmd.equals("")){
      myCmd.replace("\n","");
      myCmd.replace("\r","");    
      Serial.println(">>>"+myCmd);
      String cmd=myCmd.substring(0, myCmd.indexOf(" "));
      int val, num;
      int vall, valr;
      if(cmd.equals("stand")){
        otto_stand();
      }
      else if(cmd.equals("tstand")){
        otto_tstand();
      }
      else if(cmd.equals("vstand")){
        otto_vstand();
      }
      else if(cmd.equals("cstand")){
        otto_cstand();
      }
      else if(cmd.equals("rstand")){
        otto_rstand();
      }
      else if(cmd.equals("fw")){
        sscanf(myCmd.c_str(),"fw %d %d",&vall,&valr);
        otto_fwalk(vall, valr, 10);
        otto_fwalk(vall, valr, 10);
        otto_fwalk(vall, valr, 10);
      }
      else if(cmd.equals("bw")){
        otto_bwalk();
        otto_bwalk();
        otto_bwalk();
      }
      else if(cmd.equals("rwalk")){
        otto_rwalk();
        otto_rwalk();
        otto_rwalk();
      }
      else if(cmd.equals("info")){
        disp_servo_info();
      }
      else if(cmd.equals("angle")){
        sscanf(myCmd.c_str(),"angle %d %d",&num,&val);
        drv_servo_write(num, val);
        otto_q();
      }
  }
  myCmd="";  
}
