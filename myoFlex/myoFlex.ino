//this code takes input from a myoelctric and toggles the hand between the contracted
// and extended positions when there is a spike above a user-defined level
// future projects will include running this code with low power
 
#include <Servo.h>     //library used for servos
 
Servo thumb;            //defines the name of the five servos and them as servo objects
Servo pointer;
Servo middle;
Servo ring;
Servo pinky;
 
#define min 2000    //static integers of minimum and maximum servo extension
#define max 1000
int trigger 600 //change later with new function

#define thumbPin 3    //these are the PWM pins where servos are connected
#define pointerPin 5
#define middlePin 6
#define ringPin 9
#define pinkyPin 10
 
int state = 0;
float currentVoltage=0;   //this is where to store each read from the myo
#define threshold .4    //this is the percentage of the max read we use to define a flex
#define myoIn A1      //the pin where myo signal comes in

int setTrigger();
int trigger(int);
int setOpen();
int setPoint();
int setPinch();
int setFist();
 
// THIS PART ONLY RUNS ONCE
void setup() {
Serial.begin(9600);     //starts up serial communication between arduino and computer
  Serial.println("Starting");       //makes the hand relax all of the way
  trigger = setTrigger();
  state = 1;
}
 
// THIS PART OF THE CODE LOOPS CONTINIOUSLY
void loop() {
if(trigger(currentVoltage = analogRead(myoIn))){
  state++;
  state = state%5;
  //Serial.println(currentVoltage);        //prints in the serial moniter
  if(state == 1){
    if(!setOpen()){
        Serial.println("Failed");
    }
    else{
      Serial.println("open");
    }
  }
  else if(state == 2){
      if(!setPoint()){
        Serial.println("Failed");
      }
    else{
      Serial.println("point");
    }
  }
  else if(state == 3){
    if(!setPinch()){
      Serial.println("Failed");
    }
    else{
      Serial.println("pinch");
    }
  }
  else {
    if(state == 4){
    if(!setFist()){
      Serial.println("Failed");
    }
    else{
      Serial.println("fist");
    }
  }
  }
}

int setTrigger(){
  float average = 0;
  do{
    for(int j = 1; j <= 3; j++){
      Serial.println("Reading impulse %d in next 3 seconds", j);
      for(int i = 3; i != 0; i--){
        delay(1000);
        Serial.println("%d", i);
      }
      average = average + analogRead(myoIn);
    }
  } while(average < 300);
  return (int)(average/3);
}

int trigger(int voltage){
  if(voltage > trigger){
    return 1;
  }
  return 0;
}
 
int setOpen(){
  //build open
  return 1;
}
int setPoint(){
  //build point
  return 1;
}
int setPinch(){
  //bluild pinch
  return 1;
}
int setFist(){
  //build fist
 return 1;
}
