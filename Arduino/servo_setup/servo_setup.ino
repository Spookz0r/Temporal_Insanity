int servo_speed_pwm_pin = 3;  // Channel A speed
int servo_direction_pin0 = 4; // Channel A direction 0
int servo_direction_pin1 = 5; // Channel A direction 1
int read_value = 0;
void setup() {
    Serial.begin(9600);
    pinMode(A8,INPUT); 
    pinMode(servo_speed_pwm_pin, OUTPUT); // Set control pins to be outputs
    pinMode(servo_direction_pin0, OUTPUT);
    pinMode(servo_direction_pin1, OUTPUT);
}



int servo_dir[] = {1,0};
int servo_max = 430; //Value when max turned to the right
int servo_min = 275; //Value when max turned to the left
int servo_position_pin = A8;
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

void loop() {
  int read_value = analogRead(A8);
  Serial.println(String(read_value));
  delay(10);
  if(read_value > 430){
    setServoPosition(270, 40);
    Serial.println("Left");
  }else if(read_value < 275){
    setServoPosition(435, 40);
    Serial.println("Right");
  }

}


