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


int pwm_a = 3; // Channel A speed
int dir_a0 = 4; // Channel A direction 0
int dir_a1 = 5; // Channel A direction 1
int servoDir[] = {1,0};
int readValue = 0;
void setup() {
    pinMode(A8,INPUT); 
    Serial.begin(9600);          
    pinMode(pwm_a, OUTPUT); // Set control pins to be outputs
    pinMode(dir_a0, OUTPUT);
    pinMode(dir_a1, OUTPUT);
}

void loop() {
  int readValue = analogRead(A8);
  Serial.println(String(readValue));
  delay(10);
  if(readValue > 620){
    servoDir[0] = 1;
    servoDir[1] = 0;
    Serial.println("Left");
  }else if(readValue < 410){
    servoDir[0] = 0;
    servoDir[1] = 1;
    Serial.println("Right");
  }
  //digitalWrite(dir_a0, servoDir[0]);
  //digitalWrite(dir_a1, servoDir[1]);
  PORTG ^= (-servoDir[0] ^ PORTG) & (1 << 5); //sets the PG5 bit to servo_dir[0] value
  PORTE ^= (-servoDir[1] ^ PORTE) & (1 << 3); //sets the PE3 bit to servo_dir[1] value
  analogWrite(pwm_a, 30);
}
