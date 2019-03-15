#include "Servo.h"

//Interrupt Stuff
int interruptPin = 2;

//Serial Stuff
int hardwareBaudRate = 9600;
int softwareBaudRate = 9600;

//Motor Stuff
Servo motor;
int motorPWMPin = 5;
int motorEnablePin = 6;
int onMicroseconds = 800;
int offMicroseconds = 1600;

//Slider Stuff
int minimumOnPos = 0;
int maximumOffPos = 0;
int slideResistorPin = 1;
int slideResistorPos = 0;

//Switch Stuff
bool switchOn = true;

void setup() {
  Serial.begin(9600);

  motor.attach(motorPWMPin);
  pinMode(motorEnablePin, OUTPUT);
  digitalWrite(motorEnablePin, HIGH);

  pinMode(interruptPin, INPUT);
  attachInterrupt(0, processInterrupt, RISING);

}

void processInterrupt(){
  Serial.println("Interrupted");
  if(switchOn){
    turnOff();
  }else{
    turnOn();
  }
}

void loop() {
  /*
  slideResistorPos = analogRead(slideResistorPin); 
  Serial.println(slideResistorPos);
  delay(10);
  // put your main code here, to run repeatedly:
  */
      

}

void turnOn(){
  motor.writeMicroseconds(onMicroseconds);
  switchOn = true;
}

void turnOff(){
  motor.writeMicroseconds(offMicroseconds);
  switchOn = false;
}

//True for up/on, false for down/off
bool checkPos(bool on){
  if(on){
    return slideResistorPos > minimumOnPos;
  }else{
    return slideResistorPos < maximumOffPos;
  }
}


void printWifiErrors(){
/*
      //ESP Connection errors
      case 1: Serial.println(F("Wrong protocol")); break;
      case 2: Serial.println(F("ID rejected")); break;
      case 3: Serial.println(F("Server unavail")); break;
      case 4: Serial.println(F("Bad user/pass")); break;
      case 5: Serial.println(F("Not authed")); break;
      case 6: Serial.println(F("Failed to subscribe")); break;
      default: Serial.println(F("Connection failed")); break;
      */
}
