
int motor_speed_pwm_pin = 6;
int motor_direction_pin0 = 7;
int motor_direction_pin1 = 8;
int counter = 0;
void setup() {
  pinMode(motor_speed_pwm_pin, OUTPUT); // Set control pins to be outputs
  pinMode(motor_direction_pin0, OUTPUT);
  pinMode(motor_direction_pin1, OUTPUT);
  pinMode(18, INPUT);
  pinMode(19, INPUT);
  Serial.begin(9600);
 
  attachInterrupt(digitalPinToInterrupt(18), bip, FALLING );
  attachInterrupt(digitalPinToInterrupt(19), bip, FALLING );
}

int motor_dir[] = {0,1};
int num = 100;
void loop() {
  digitalWrite(motor_direction_pin0, motor_dir[0]);
  analogWrite(5,100);   
  digitalWrite(motor_direction_pin1, motor_dir[1]);
  analogWrite(motor_speed_pwm_pin, num);
  Serial.println(String(counter));
  delay(100);
}

void forward(int speed){
  motor_dir[0] = 1;
  motor_dir[1] = 0;
  analogWrite(motor_speed_pwm_pin, speed);
}


void backwards(int speed){
  motor_dir[0] = 0;
  motor_dir[1] = 1;
  analogWrite(motor_speed_pwm_pin, speed);
}

void bip(){
  counter++;
}

