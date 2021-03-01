/********************************************//** 
(c) 2021 branpie.com <branpiehelp@gmail.com>
Rapple Bot
************************************************/
#include <SoftwareSerial.h>
#include "RappleBot.h"
#define LEG_LEFT_P    2
#define LEG_RIGHT_P   3
#define FOOT_LEFT_P   4
#define FOOT_RIGHT_P  5
#define US_TRIG_P 8
#define US_ECHO_P 9
#define BT_RX_P 10
#define BT_TX_P 11
#define BZ_P 13
SoftwareSerial bt(BT_TX_P, BT_RX_P);
RappleBot Rapple;

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
  Rapple.action(act, 2);
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
  Rapple.action(act, 2);
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
  Rapple.action(act, 2);
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
  Rapple.action(act, 2); 
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
  Rapple.action(act, 2);
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
  Rapple.action(90,-1,35,-1, 3);//ll lr fl fr dt
  Rapple.action(90,-1,35,125, 2);//ll lr fl fr dt
  Rapple.action(90,-1,90,125, 2);//ll lr fl fr dt
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
  Rapple.action(-1,-1,90-vall,-1, dt);//ll lr fl fr dt
  Rapple.action(-1,-1,-1,90+valr, dt);//ll lr fl fr dt
  if(d=='f'){
    Rapple.action(90+vall,-1,-1,-1, dt);//ll lr fl fr dt
    Rapple.action(-1,90-valr,-1,-1, dt);//ll lr fl fr dt    
  }
  else if(d=='b'){
    Rapple.action(90-vall,-1,-1,-1, dt);//ll lr fl fr dt    
    Rapple.action(-1,90+valr,-1,-1, dt);//ll lr fl fr dt    
  }
  Rapple.action(-1,-1,90,90, dt);//ll lr fl fr dt   
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
  Rapple.action(-1,-1,90+vall,-1, dt);//ll lr fl fr dt  
  Rapple.action(-1,-1,-1,90-valr, dt);//ll lr fl fr dt  
  if(d=='f'){
    Rapple.action(90-vall,-1,-1,-1, dt);//ll lr fl fr dt  
    Rapple.action(-1,90+valr,-1,-1, dt);//ll lr fl fr dt  
  }
  else if(d=='b'){
    Rapple.action(90+vall,-1,-1,-1, dt);//ll lr fl fr dt  
    Rapple.action(-1,90-valr,-1,-1, dt);//ll lr fl fr dt  
  }
  Rapple.action(-1,-1,90,90, dt);//ll lr fl fr dt     
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
void RBOT_music(int val, int tempo=1)
{
  if(val==1){
    Rapple.sound_note(C,300/tempo);
    Rapple.sound_note(E,300/tempo);
    Rapple.sound_note(G,300/tempo);
    Rapple.sound_note(REST,1000/tempo);
    Rapple.sound_note(C2,300/tempo);
  }  
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);//Serial Default Baud Rate
  Serial.println("Hello World!");
  bt.begin(38400);//Bluetooth 4.0 BLE Default Baud Rate
  bt.println("AT");
  
  //Rapple Object init
  Rapple.init(LEG_LEFT_P,LEG_RIGHT_P,FOOT_LEFT_P,FOOT_RIGHT_P);
  Rapple.sound_init(BZ_P);
  Rapple.eye_init(US_TRIG_P,US_ECHO_P);
  //Start
  RBOT_music(1);
  RBOT_stand();
  RBOT_tstand();
  delay(1000);
  RBOT_stand();
}
String myCmd="";
long int counter=0;
void loop() {
  if(counter++%30000==0){ //Time tick about 1sec
    long tmp=Rapple.eye_update();
    if(tmp<10 && tmp>0){ //10cm check
      Serial.println("Siren");
      Rapple.sound_note(C2,300);
    }
  }
  // put your main codehere, to run repeatedly:
  while(bt.available()){
    char tmp;
    tmp=(char)bt.read();
    myCmd+=tmp;
    Serial.write(tmp);
    delay(2);
  }
  while(Serial.available()){
    char tmp;
    tmp=(char)Serial.read();    
    myCmd+=tmp;
    bt.write(tmp);
    delay(2);
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
      else if(cmd.equals("angle")){
        int sel, val;
        sscanf(myCmd.c_str(),"angle %d %d",&sel,&val);
        Serial.println("angle OK");
        Rapple.servo_write(sel, val);
        Rapple.commit();
      }
  }
  myCmd=""; 
}
