/*
 Connect servo to A, motor to B. 
 Servo's potentiometer required 3.3v for these numbers to be correct 

 Arduino-python communication may sometimes not start.
  
 */

int servo_speed_pwm_pin = 3;  // Channel A speed
int servo_direction_pin0 = 4; // Channel A direction 0
int servo_direction_pin1 = 5; // Channel A direction 1
int motor_speed_pwm_pin = 6;
int motor_direction_pin0 = 7;
int motor_direction_pin1 = 8;

int servo_dir[] = {1,0};
int motor_dir[] = {0,1};
int servo_position_pin = A8;

char incomingByte=0;


void setServoPosition(int pos, int speed){
  int current_pos = analogRead(servo_position_pin);
  if(pos < current_pos){        // We need to turn right
    servo_dir[0] = 1;
    servo_dir[1] = 0;
  }else if(pos > current_pos){  //Need to turn left
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
void forward(int speed){
  motor_dir[0] = 1;
  motor_dir[1] = 0;
  digitalWrite(motor_direction_pin0, motor_dir[0]);
  digitalWrite(motor_direction_pin1, motor_dir[1]);
  analogWrite(motor_speed_pwm_pin, speed);
}


void backwards(int speed){
  motor_dir[0] = 0;
  motor_dir[1] = 1;
  digitalWrite(motor_direction_pin0, motor_dir[0]);
  digitalWrite(motor_direction_pin1, motor_dir[1]);
  analogWrite(motor_speed_pwm_pin, speed);
}

void setup(){
  Serial.begin(9600);
  pinMode(motor_speed_pwm_pin, OUTPUT); // Set control pins to be outputs
  pinMode(motor_direction_pin0, OUTPUT);
  pinMode(motor_direction_pin1, OUTPUT);
  pinMode(servo_speed_pwm_pin, OUTPUT); // Set control pins to be outputs
  pinMode(servo_direction_pin0, OUTPUT);
  pinMode(servo_direction_pin1, OUTPUT);
}


void loop(){

  if(Serial.available() > 0){
    incomingByte = Serial.read();
    // Echo what you got.
    //Serial.print("I got: ");
    
    Serial.write(incomingByte);
    int nextval = 0;
    switch(incomingByte){
      case 119: //w
        nextval = Serial.read();
        forward(nextval);
      break;
      case 115://s
        nextval = Serial.read();
        backwards(nextval);
      break;
      case 113: //q
        backwards(0);
      break;
      case 97: //a
        setServoPosition(400, 30);
      break;
      case 100: //d
        setServoPosition(300, 30);
      break;
    }
   
    while (Serial.available()) { //is there anything to read?
        int getData = Serial.read();  //if yes, read it
    }
  }
    
}
