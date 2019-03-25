
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
int onMicroseconds = 1800; //up towards battery
int offMicroseconds = 800; //down towards power plug

unsigned long volatile lastMoveTime = 0;
int minMoveDelay = 500;

//Slider Stuff
int minimumOffPos = 500;      //actual = 566; aggressive = 500; conservative = 490;
int maximumOnPos = 180;     //actual = 154; aggressive = 190; conservative = 200;
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
  if(!moving){
    unsigned long currentMillis = millis();
    if ((unsigned long)(currentMillis - lastInterruptTime) >= minInterruptDelay) {
      unprocessedMove = true;
      lastInterruptTime = currentMillis;
    }
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

  if(moving){
    unsigned long currentMillis = millis();
    if ((unsigned long)(currentMillis - lastMoveTime) >= minMoveDelay) {
      digitalWrite(motorEnablePin, LOW);
      moving = false;
      #ifdef DEBUG
        Serial.println("End Movement");
      #endif
    }
    return;

  }else if(unprocessedMove){
    switchOn = checkPos(true);
    #ifdef DEBUG
      Serial.println("Start Movement");
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
  motor.writeMicroseconds(onMicroseconds);
  digitalWrite(motorEnablePin, HIGH);
  moving = true;
 
  digitalWrite(13,HIGH); //Debug

  lastMoveTime = millis();
  
  #ifdef DEBUG
      Serial.println("Turn on");
  #endif
  
  //digitalWrite(motorEnablePin, LOW);
  
  //if(!checkPos(true)){
    //turnOff();
  //}
  
}

void turnOff(){
  
  motor.writeMicroseconds(offMicroseconds);
  digitalWrite(motorEnablePin, HIGH);
  moving = true;
  
  digitalWrite(13,LOW);  //Debug
  
  lastMoveTime = millis();

  #ifdef DEBUG
    Serial.println("Turn off");
  #endif

  //digitalWrite(motorEnablePin, LOW);

  //if(!checkPos(false)){
    //turnOn();
  //}
  
}

//True for up/on, false for down/off
bool checkPos(bool on){
  slideResistorPos = analogRead(slideResistorPin);

  #ifdef DEBUG
    Serial.print("Slide Pos: ");
    Serial.println(slideResistorPos);
  #endif

  if(on){
    return slideResistorPos < maximumOnPos;
  }else{
    return slideResistorPos > minimumOffPos;
  }
}
