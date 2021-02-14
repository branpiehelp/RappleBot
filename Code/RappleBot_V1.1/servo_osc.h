/********************************************//** 
BRANPIE All right reserved.
RAPPLE ROBOT
************************************************/
#ifndef servo_osc_h
#define servo_osc_h
#include <Servo.h>
class servo_osc{
private:
    Servo _servo;
    int pin;
    int reverse;
    int c_angle;
    int t_angle;
    int trim;
    int dt;
public:
    servo_osc(int trim=0,int rev=0) 
    {this->trim=trim; this->reverse=rev;};
	  void attach(int pin);
    void detach();
    void servo_osc::write(int angle);
    bool servo_osc::refresh(int dt);
    int get_sub_angle();
    void get_info();
};

#endif
