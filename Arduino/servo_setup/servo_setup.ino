
int readValue = 0;
void setup() {
    pinMode(A8,INPUT); 
    Serial.begin(9600);          

    
}

void loop() {
  int readValue = analogRead(A8);
  
  Serial.println(String(readValue));
  delay(10);
  
}
