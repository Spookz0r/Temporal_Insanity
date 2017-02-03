char incomingByte=0;

void setup() {
  // Open serial connection.
  Serial.begin(9600);
}

void loop() {
  if (Serial.available() > 0) {
    // Read the incoming byte.
    incomingByte = Serial.read();
    // Echo what you got.
    
    Serial.write(incomingByte);
    if(incomingByte == 119){
      int nextval = Serial.read();
      //Serial.write(nextval);
      digitalWrite(13, HIGH);
    }else if(incomingByte == 115){
      digitalWrite(13, LOW);
    }
    
    while (Serial.available()) { //is there anything to read?
        int getData = Serial.read();  //if yes, read it
    }
    
  }
}
