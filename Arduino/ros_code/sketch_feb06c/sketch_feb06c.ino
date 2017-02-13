/*

Will move the servo back and forth

Motor:
Connect to h-bridge
Brown, A-
Orange, A+


Potentiometer:
Connect to arduino. 
White, 3.3V NOTE FOR THIS SHIT TO WORK, otherwise change readValue limits
Black, GND
Red, A8.
*/



#include <ros.h>
#include <std_msgs/UInt16.h>
#include <std_msgs/String.h>


int servo_speed_pwm_pin = 3; // Channel A speed
int servo_direction_pin0 = 4; // Channel A direction 0
int servo_direction_pin1 = 5; // Channel A direction 1
int servo_dir[] = {1,0};
int readValue = 0;
int speed = 30;
int servo_position_pin = A8;
int servo_value = 512;

ros::NodeHandle nh;
std_msgs::String str_msg;
ros::Publisher info_pub("info", &str_msg);

void setServoPosition(int pos, int speed){
  int current_pos = analogRead(servo_position_pin);
  if(pos < current_pos-5){        // We need to turn right
    servo_dir[0] = 1;
    servo_dir[1] = 0;
  }else if(pos > current_pos + 5){  //Need to turn left
    servo_dir[0] = 0;
    servo_dir[1] = 1;
    
  }else{                        //At position, send speed 0 and return
    analogWrite(servo_speed_pwm_pin, 0); 
    return;
  }
  digitalWrite(servo_direction_pin0, servo_dir[0]);
  digitalWrite(servo_direction_pin1, servo_dir[1]);
  analogWrite(servo_speed_pwm_pin, speed);
}

void servo_cb( const std_msgs::UInt16& cmd_msg){
  servo_value = cmd_msg.data;
  
}


ros::Subscriber<std_msgs::UInt16> servo_sub("servo", servo_cb);


void setup(){
  
  nh.initNode();
  nh.subscribe(servo_sub);
  nh.advertise(info_pub);
  
  pinMode(servo_position_pin,INPUT); 
  pinMode(servo_speed_pwm_pin, OUTPUT); // Set control pins to be outputs
  pinMode(servo_direction_pin0, OUTPUT);
  pinMode(servo_direction_pin1, OUTPUT);
   
}


void loop(){
  nh.spinOnce();
  setServoPosition(servo_value, 30);
  delay(1);
}
