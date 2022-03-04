/********************************************//** 
(c) 2021 branpie.com <branpiehelp@gmail.com>
Rapple Bot
************************************************/
#include <SoftwareSerial.h>
#include "IRremote.h"
#include "RappleBot.h"
#define LEG_LEFT_P    2
#define LEG_RIGHT_P   3
#define FOOT_LEFT_P   4
#define FOOT_RIGHT_P  5
#define IR_PIN A5
#define US_TRIG_P 8
#define US_ECHO_P 9
#define BT_RX_P 10
#define BT_TX_P 11
#define BZ_P 13


//Grobal variables
SoftwareSerial bt(BT_TX_P, BT_RX_P);
IRrecv irrecv(IR_PIN);
RappleBot Rapple;
String myCmd="";
unsigned long int counter=0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);//Serial Default Baud Rate  
  Serial.println("Hello World!");
  bt.begin(38400);//Bluetooth 4.0 bt Default Baud Rate
  bt.println("AT");
  //IR 
  irrecv.enableIRIn();
  
  //Rapple Object init
  Rapple.init(LEG_LEFT_P,LEG_RIGHT_P,FOOT_LEFT_P,FOOT_RIGHT_P);
  Rapple.sound_init(BZ_P);
  Rapple.eye_init(US_TRIG_P,US_ECHO_P);
  
  //Adjust
  //Rapple.set_servo_trim(LEFT_FOOT,-10);
  //Rapple.set_servo_trim(RIGHT_FOOT,-7);
  Rapple.load_trim_all();
  
  //Start
  Rapple.alert(CEG_C,1);
  Rapple.tstand(); 
  Rapple.stand();    
}
void _loop() {
  // put your main codehere, to run repeatedly:
}
void timer_loop() {
  if(counter%(2000)==0){//2 seconds
    long tmp=Rapple.eye_update();
    if(tmp<10 && tmp>0){ //10cm check
      Serial.println("Siren");
      Rapple.alert(CE,1);
    }
    delay(1);
  }
  counter=millis();
}
void cmd_handler(){
  myCmd="";  
  while(bt.available()){
    char tmp;
    tmp=(char)bt.read();
    myCmd+=tmp;
    Serial.write(tmp);
    delay(5);
  }
  while(Serial.available()){
    char tmp;
    tmp=(char)Serial.read();    
    myCmd+=tmp;
    bt.write(tmp);
    delay(5);
  }

  if(!myCmd.equals("")){
      myCmd.replace("\n","");
      myCmd.replace("\r","");    
      Serial.println(">>>"+myCmd);
      String cmd=myCmd.substring(0, myCmd.indexOf(" "));

      if(cmd.equals("stand")){
        Rapple.stand();
      }
      else if(cmd.equals("tstand")){
        Rapple.tstand();
      }
      else if(cmd.equals("vstand")){
        Rapple.vstand();
      }
      else if(cmd.equals("cstand")){
        Rapple.cstand();
      }
      else if(cmd.equals("ostand")){
        Rapple.ostand();
      }
      else if(cmd.equals("rstand")){
        Rapple.rstand();
      }
      else if(cmd.equals("fw")){
        int vall, valr, sp;//left angle, right angle, speed:1~5
        sscanf(myCmd.c_str(),"fw %d %d %d",&vall,&valr,&sp);
        Rapple.fwalk(vall, valr, sp);
      }
      else if(cmd.equals("run")){
        int vall, valr, sp;//left angle, right angle, speed:1~5
        sscanf(myCmd.c_str(),"run %d %d %d",&vall,&valr,&sp);
        for(int i=0;i<10;i++){
          Rapple.fwalk(vall, valr, sp);
        }        
      }
      else if(cmd.equals("bw")){
        Rapple.bwalk(25,25,2);
      }
      else if(cmd.equals("rw")){
        Rapple.turnRight(20,2);
      }
      else if(cmd.equals("lw")){
        Rapple.turnLeft(20,2);
      }
      else if(cmd.equals("angle")){
        int sel, val;
        sscanf(myCmd.c_str(),"angle %d %d",&sel,&val);
        Serial.println("angle OK");
        Rapple.servo_write(sel, val);
        Rapple.commit();
      }
      else if(cmd.equals("ctrim")){
        Rapple.clear_trim_all();
        Serial.println("clear trim All OK");        
      }
      else if(cmd.equals("strim")){
        int sel, val;
        sscanf(myCmd.c_str(),"strim %d %d",&sel,&val);
        Rapple.set_servo_trim(sel, val);        
        Serial.print(sel);   
        Serial.println(":set trim OK");        
      }
      else if(cmd.equals("gtrim")){
        int sel, val;
        sscanf(myCmd.c_str(),"gtrim %d",&sel);
        Serial.print(Rapple.get_servo_trim(sel));   
        Serial.println(":get trim OK");        
      }
      else if(cmd.equals("cmdtest")){
        int val;
        sscanf(myCmd.c_str(),(String("cmdtest")+String(" %d")).c_str(),&val);
        Serial.println(val);
      }
  } 
}
void cmd_handler_IR(){
  if(irrecv.decode()){
    long long int cmd=irrecv.decodedIRData.decodedRawData;
    if(cmd!=0){
      Serial.println((unsigned long)cmd);
      bt.println((unsigned long)cmd);
    }
    if(cmd==3125149440){ //1
      for(int i=0;i<5;i++)
        Rapple.fwalk(10, 10, 2);
    }
    else if(cmd==3108437760){  //2
      for(int i=0;i<5;i++)
        Rapple.fwalk(20, 20, 2);
    }
    else if(cmd==3091726080){ //3
      for(int i=0;i<5;i++)
        Rapple.fwalk(30, 30, 2);
    }
    else if(cmd==3927310080){ //8
      for(int i=0;i<5;i++)
        Rapple.bwalk(20, 20, 2);
    }
    else if(cmd==3141861120){ //4
      for(int i=0;i<5;i++)
        Rapple.fwalk(0, 30, 2);
    }
    else if(cmd==3158572800){ //6
      for(int i=0;i<5;i++)
        Rapple.fwalk(30, 0, 2);
    }
    else if(cmd==3208707840 || cmd==3810328320 || cmd==3860463360){ //5, OK, 0
      Rapple.stand();
      Rapple.sleep();
    }
    else if(cmd==3910598400){ //*
      Rapple.tstand();
    }         
    else if(cmd==4061003520){ //#
      Rapple.vstand();
    }         
    else if(cmd==4144561920){ //LEFT
      Rapple.lstand();
    }    
    else if(cmd==2774204160){ //RIGHT
      Rapple.rstand();
    }
    else if(cmd==3877175040){ //up
      for(int i=0;i<5;i++)
        Rapple.fwalk(40, 40, 2);
    } 
    irrecv.resume();
  }
}
void loop() {
  _loop();
  timer_loop();
  cmd_handler();
  cmd_handler_IR();
}
