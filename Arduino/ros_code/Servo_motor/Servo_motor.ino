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
/* Motor connected to B, black to +, red to -
  encoder: green -> gnd
           blue -> 5V
           yellow -> 18
           white -> 19
*/           

#include <ros.h>
#include <std_msgs/UInt16.h>
#include <std_msgs/Int16.h>
#include <std_msgs/Float32.h>
#include <std_msgs/String.h>


#define MAX_RIGHT 410
#define MAX_LEFT 640
#define MID_POS 525
#define SERVO_SPEED 30
#define ENCODER_A_PIN 18
#define ENCODER_B_PIN 19
#define PULSES_PER_REVOLUTION 680
#define WHEEL_RADIUS_MM 30
#define VELOCITY_PUBLISH_HZ 20
#define SERVO_UPDATE_HZ 100

int servo_speed_pwm_pin = 3; // Channel A speed
int servo_direction_pin0 = 4; // Channel A direction 0
int servo_direction_pin1 = 5; // Channel A direction 1
int servo_dir[] = {1,0};
int readValue = 0;
int servo_position_pin = A8;
int servo_value = MID_POS;

//Motor Setup
int motor_speed_pwm_pin = 6;
int motor_direction_pin0 = 7;
int motor_direction_pin1 = 8;
int motor_dir[] = {0,1};

//Encoder
volatile long encoder_pos = 0;
long encoder_pos_new = 0;
long encoder_pos_old = 0;
unsigned long new_time = 0;
unsigned long old_time = 0;
long temp_vel = 0;
int rpm = 0;
float velocity_ms = 0;

ros::NodeHandle nh;
std_msgs::String str_msg;
std_msgs::Float32 velocity_msg;
ros::Publisher info_pub("info", &str_msg);
ros::Publisher velocity_pub("velocity", &velocity_msg);


void servo_cb( const std_msgs::UInt16& cmd_msg){
  servo_value = map(cmd_msg.data,0,100,MAX_RIGHT,MAX_LEFT);
  if(servo_value > 100) servo_value = 100;
  if(servo_value < 0) servo_value = 0;
}

void set_motor_speed_cb(const std_msgs::Int16& speed_msg){
  if(speed_msg.data > 0){
    motor_dir[0] = 1;
    motor_dir[1] = 0;
  }else{
    motor_dir[0] = 0;
    motor_dir[1] = 1;
  }
  //digitalWrite(motor_direction_pin0, motor_dir[0]); // 7
 // digitalWrite(motor_direction_pin1, motor_dir[1]); // 8
  PORTH ^= (-motor_dir[0] ^ PORTH) & (1 << 4); //sets the PH4 bit to servo_dir[0] value
  PORTH ^= (-motor_dir[1] ^ PORTH) & (1 << 5); //sets the PH5 bit to servo_dir[1] value
  analogWrite(motor_speed_pwm_pin, abs(speed_msg.data));
}


ros::Subscriber<std_msgs::UInt16> servo_sub("servo", servo_cb);
ros::Subscriber<std_msgs::Int16> motor_sub("motor", set_motor_speed_cb);

void doEncoder(){
  //if(digitalRead(ENCODER_A_PIN) == digitalRead(ENCODER_B_PIN)){
    //encoder_pos++;
  //} else {
    //encoder_pos++; //--
  //}
  encoder_pos++;
}

void setup(){

  nh.initNode();
  nh.subscribe(motor_sub);
  nh.subscribe(servo_sub);
  nh.advertise(info_pub);
  nh.advertise(velocity_pub);
  
  pinMode(ENCODER_A_PIN, INPUT);
  pinMode(ENCODER_B_PIN, INPUT);
  attachInterrupt(digitalPinToInterrupt(ENCODER_A_PIN), doEncoder, FALLING ); //A
  attachInterrupt(digitalPinToInterrupt(ENCODER_B_PIN), doEncoder, FALLING ); //B
// EASY MODE
/*
  pinMode(servo_position_pin,INPUT); 
  pinMode(servo_speed_pwm_pin, OUTPUT); // Set control pins to be outputs
  pinMode(servo_direction_pin0, OUTPUT);
  pinMode(servo_direction_pin1, OUTPUT);
  
  
  pinMode(motor_speed_pwm_pin, OUTPUT); // Set control pins to be outputs
  pinMode(motor_direction_pin0, OUTPUT);
  pinMode(motor_direction_pin1, OUTPUT);
*/
/* PRO MODE 

  servo: pin3 = PE5, pin4 =PG5 , pin5 = PE3, pin A8 = PK0  
  Motor: pin6 = PH3, pin7 =PH4, pin8 = PH5 
  */
  DDRK = B00000000; //sets PK0 as input
  DDRE = DDRE | B00101000; //sets PE3 PE5 as output 
  DDRG = DDRG | B00100000; //sets PG5 as output 
  DDRH = DDRH | B00111000; //sets Ph3, Ph4 PH5 as output

  noInterrupts();

  //Timer3 16bit timer VELOCITY_PUBLISH_HZ hz for publish velocity
  TCCR3A = 0;  //Sets tccr3a register to 0
  TCCR3B = 0;  //sets tccr3b register to 0
  TCNT3 = 0;   //initialize counter value to 0

    // compare match register for 100 hz increments
  OCR3A = long(16000000)/(long(VELOCITY_PUBLISH_HZ)*1024) - 1;  //= (16*10 upphöjt 6) / (100*1024) - 1
  TCCR3B |= (1 << WGM32); //turn on CTC mode
  TCCR3B |= (1 << CS32) | (1 << CS40); //Set prescaler to 1024
  TIMSK3 |= (1 << OCIE3A); //Enable timer compare interrupt

  
  //Timer4  16bit timer 100 hz for servo update
  
  TCCR4A = 0;  //Sets tccr3a register to 0
  TCCR4B = 0;  //sets tccr3b register to 0
  TCNT4 = 0;   //initialize counter value to 0

    // compare match register for 100 hz increments
  OCR4A = long(16000000)/(long(SERVO_UPDATE_HZ)*1024) - 1;// 155;  //= (16*10 upphöjt 6) / (100*1024) - 1
  TCCR4B |= (1 << WGM42); //turn on CTC mode
  TCCR4B |= (1 << CS42) | (1 << CS40); //Set prescaler to 1024
  TIMSK4 |= (1 << OCIE4A); //Enable timer compare interrupt


  
  interrupts();

}

ISR(TIMER3_COMPA_vect){
  
  encoder_pos_new = encoder_pos;
  new_time = millis();
  temp_vel = (encoder_pos_new-encoder_pos_old)*1000 / (new_time-old_time);  //number of pulses *1000 / timestep, *1000 for millis
  rpm = (temp_vel*60*1000)/((new_time-old_time)*PULSES_PER_REVOLUTION); // gives revolutions per minute
  velocity_ms = (float(WHEEL_RADIUS_MM)/1000) * rpm * 0.10472;  //0.10472 = 2*pi/60
  encoder_pos_old = encoder_pos_new;
  old_time = new_time;
  
  velocity_msg.data = velocity_ms;
  velocity_pub.publish(&velocity_msg);
}

ISR(TIMER4_COMPA_vect)
{
  String message = "Hello";
  
  char charBuf[message.length()+1];
  message.toCharArray(charBuf,message.length()+1);
  str_msg.data = charBuf;
  info_pub.publish(&str_msg);
  
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
    analogWrite(servo_speed_pwm_pin, 0); //pin 3
    
    return;
  }
//  digitalWrite(servo_direction_pin0, servo_dir[0]); //pin4
  //digitalWrite(servo_direction_pin1, servo_dir[1]); //pin 5
  PORTG ^= (-servo_dir[0] ^ PORTG) & (1 << 5); //sets the PG5 bit to servo_dir[0] value
  PORTE ^= (-servo_dir[1] ^ PORTE) & (1 << 3); //sets the PE3 bit to servo_dir[1] value
  analogWrite(servo_speed_pwm_pin, SERVO_SPEED);
}

void loop(){
  nh.spinOnce();
  delay(2);
}

