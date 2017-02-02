import processing.serial.*;

String[] portName = Serial.list(); //change the 0 to a 1 or 2 etc. to match your port
Serial myPort;
String val;
void setup(){
  println(portName[2]);
  myPort = new Serial(this, portName[2], 9600);
}


void draw(){
  if(myPort.available() > 0){
    val = myPort.readStringUntil('\n');
    println(val);
  }
  
}