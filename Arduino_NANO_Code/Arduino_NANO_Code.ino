
//#define DEBUG 1

#include "Servo.h"

//Interrupt Stuff
int interruptPin = 2;
unsigned long volatile lastInterruptTime = 0;
int minInterruptDelay = 1500;

//Serial Stuff
int hardwareBaudRate = 9600;
int softwareBaudRate = 9600;

//Motor Stuff
Servo motor;
int motorPWMPin = 5;
int motorEnablePin = 6;
int onMicroseconds = 800;
int offMicroseconds = 1600;
int motorMoveTime = 1000;

//Slider Stuff
int minimumOnPos = 500;      //actual = 566; aggressive = 500; conservative = 490;
int maximumOffPos = 180;     //actual = 154; aggressive = 190; conservative = 200;
int slideResistorPin = A0;
int slideResistorPos = 0;

//Switch state Stuff
bool switchOn = true;
bool unprocessedMove = false;
bool moving = false;

//Button stuff
int buttonPin = 3;

void setup() {
  #ifdef DEBUG
    Serial.begin(hardwareBaudRate);
  #endif

  //
  pinMode(13,OUTPUT);
  
  motor.attach(motorPWMPin);
  pinMode(motorEnablePin, OUTPUT);

  pinMode(interruptPin, INPUT);
  attachInterrupt(0, processInterrupt, RISING);

  pinMode(buttonPin, INPUT);
  attachInterrupt(1, processInterrupt, RISING);

  turnOn();

  //digitalWrite(motorEnablePin, HIGH); //This is used as a global servo enable; Keep disabled by default
}

void processInterrupt(){
  unsigned long currentMillis = millis();
  if ((unsigned long)(currentMillis - lastInterruptTime) >= minInterruptDelay) {
     unprocessedMove = true;
     lastInterruptTime = currentMillis;
  }
}

void loop() {
  #ifdef DEBUG
    Serial.println("Idle");
  #endif
  
  delay(100);
  update();
  
}

void update(){
  if(unprocessedMove){

    #ifdef DEBUG
      Serial.println("moving");
    #endif
    
    if(switchOn){
      turnOff();
    }else{
      turnOn();
    }
    unprocessedMove = false;
  }
}

void turnOn(){
  digitalWrite(motorEnablePin, HIGH);
  motor.writeMicroseconds(onMicroseconds);
  
  digitalWrite(13,HIGH); //Debug
  
  switchOn = true;
  delay(motorMoveTime);
  digitalWrite(motorEnablePin, LOW);
  
  if(!checkPos(true)){
    //turnOff();
  }
  
}

void turnOff(){
  digitalWrite(motorEnablePin, HIGH);
  motor.writeMicroseconds(offMicroseconds);
  
  digitalWrite(13,LOW);  //Debug
  
  switchOn = false;
  delay(motorMoveTime);
  digitalWrite(motorEnablePin, LOW);

  if(!checkPos(false)){
    //turnOn();
  }
  
}

//True for up/on, false for down/off
bool checkPos(bool on){
  slideResistorPos = analogRead(slideResistorPin);

  #ifdef DEBUG
    Serial.print("Slide Pos: ");
    Serial.println(slideResistorPos);
  #endif

  if(on){
    return slideResistorPos > minimumOnPos;
  }else{
    return slideResistorPos < maximumOffPos;
  }
}
