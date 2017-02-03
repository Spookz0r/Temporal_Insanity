int incomingByte = 0;   // for incoming serial data

void setup() {
        Serial.begin(9600);     // opens serial port, sets data rate to 9600 bps
}
char byteRead;
void loop() {
    if (Serial.available()) {
        byteRead = Serial.read();
        Serial.write(byteRead);
    }
        
}
 
