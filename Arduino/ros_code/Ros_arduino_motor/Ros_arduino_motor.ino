
/* Motor connected to B, black to +, red to -
  encoder: green -> gnd
           blue -> 5V
           yellow -> 18
           white -> 19
*/           

#include <ros.h>
#include <std_msgs/Int16.h>



//Motor Setup
int motor_speed_pwm_pin = 6;
int motor_direction_pin0 = 7;
int motor_direction_pin1 = 8;
int motor_dir[] = {0,1};

void set_motor_speed_cb(const std_msgs::Int16& speed_msg){
  digitalWrite(13,!digitalRead(13));
  if(speed_msg.data > 0){
    motor_dir[0] = 1;
    motor_dir[1] = 0;
  }else{
    motor_dir[0] = 0;
    motor_dir[1] = 1;
  }
  digitalWrite(motor_direction_pin0, motor_dir[0]);
  digitalWrite(motor_direction_pin1, motor_dir[1]);
  analogWrite(motor_speed_pwm_pin, abs(speed_msg.data));


}
// ROS Stuff
ros::NodeHandle nh;
ros::Subscriber<std_msgs::Int16> motor_sub("motor", set_motor_speed_cb);

void setup(){
  
  nh.initNode();
  nh.subscribe(motor_sub);
  
  int motor_dir[] = {0,1};
  pinMode(motor_speed_pwm_pin, OUTPUT); // Set control pins to be outputs
  pinMode(motor_direction_pin0, OUTPUT);
  pinMode(motor_direction_pin1, OUTPUT);
  pinMode(13,OUTPUT);
}



void loop(){
  nh.spinOnce();
  delay(1);

}
