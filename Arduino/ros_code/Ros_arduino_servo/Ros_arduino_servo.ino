/*
To start ros node in terminal:
 rosrun rosserial_python serial_node.py /dev/ttyACM0
 
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


#define MAX_RIGHT 410
#define MAX_LEFT 640
#define MID_POS 525
#define SERVO_SPEED 30

int servo_speed_pwm_pin = 3; // Channel A speed
int servo_direction_pin0 = 4; // Channel A direction 0
int servo_direction_pin1 = 5; // Channel A direction 1
int servo_dir[] = {1,0};
int readValue = 0;
int servo_position_pin = A8;
int servo_value = MID_POS;

ros::NodeHandle nh;
std_msgs::String str_msg;
ros::Publisher info_pub("info", &str_msg);



void servo_cb( const std_msgs::UInt16& cmd_msg){
  servo_value = map(cmd_msg.data,0,100,MAX_RIGHT,MAX_LEFT);
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


  //Timer0, calling compareA function every 1 ms
  //OCR0A = 0xAF;
  //TIMSK0 |= _BV(OCIE0A);

  noInterrupts();
  
  //Timer3  16bit timer
  
  TCCR4A = 0;  //Sets tccr3a register to 0
  TCCR4B = 0;  //sets tccr3b register to 0
  TCNT4 = 0;   //initialize counter value to 0

    // compare match register for 100 hz increments
  OCR4A = 155;  //= (16*10 upphöjt 6) / (100*1024) - 1
  TCCR4B |= (1 << WGM42); //turn on CTC mode
  TCCR4B |= (1 << CS42) | (1 << CS40); //Set prescaler to 1024
  TIMSK4 |= (1 << OCIE4A); //Enable timer compare interrupt
  
  interrupts();
  
  pinMode(13,OUTPUT);

}

//void setServoPosition(int pos, int speed){
//SIGNAL(TIMER0_COMPA_vect)
ISR(TIMER4_COMPA_vect)
{
  //digitalWrite(13,!digitalRead(13));
  int current_pos = analogRead(servo_position_pin);
  int pos = servo_value;
  if(pos < current_pos-5){        // We need to turn right
    servo_dir[0] = 1;
    servo_dir[1] = 0;
  }
  else if(pos > current_pos + 5){  //Need to turn left
    servo_dir[0] = 0;
    servo_dir[1] = 1;

  }
  else{                        //At position, send speed 0 and return
    analogWrite(servo_speed_pwm_pin, 0); 
    return;
  }
  digitalWrite(servo_direction_pin0, servo_dir[0]);
  digitalWrite(servo_direction_pin1, servo_dir[1]);
  analogWrite(servo_speed_pwm_pin, SERVO_SPEED);
}


void loop(){
  nh.spinOnce();
  //setServoPosition(servo_value, SERVO_SPEED);
  delay(1);
}

