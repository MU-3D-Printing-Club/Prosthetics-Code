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
int trigger = 600; //change later with new function

#define thumbPin 3    //these are the PWM pins where servos are connected
#define pointerPin 5
#define middlePin 6
#define ringPin 9
#define pinkyPin 10
 
int state = 1;
float currentVoltage=0;   //this is where to store each read from the myo
#define threshold .4    //this is the percentage of the max read we use to define a flex
#define myoIn A1      //the pin where myo signal comes in


int setTrigger();
int check(int);
int setOpen();
int setPoint();
int setPinch();
int setFist();
void handPosition(int,int,int,int,int);
 
// THIS PART ONLY RUNS ONCE
void setup() {
Serial.begin(9600);     //starts up serial communication between arduino and computer
  Serial.println("linking servos");
  thumb.attach(thumbPin, min, max);    //defines where the servos are and their max/mins
  pointer.attach(pointerPin, min, max);
  middle.attach(middlePin, min, max);
  ring.attach(ringPin, min, max);
  pinky.attach(pinkyPin, min, max);
  Serial.println("Starting");       //makes the hand relax all of the way
  setOpen(); //set default position
  trigger = setTrigger();
}
 
// THIS PART OF THE CODE LOOPS CONTINIOUSLY
void loop() {
  if(check(currentVoltage = analogRead(myoIn))){
    state++;
    state = state%5;
    //Serial.println(currentVoltage);        //prints in the serial moniter
    if(state == 1){
      handPosition(max,max,max,max,max);
    }
    else if(state == 2){
      handPosition(max,min,max,max,max);
    }
    else if(state == 3){
      handPosition(min,min,min,max,max);
    }
    else if(state == 4){
      handPosition(min,min,min,min,min);
    }
    else {
      state = 1; //edgecase if logic fail
      handPosition(max,max,max,max,max);
    }
  }
}
int setTrigger(){
  float average = 0;
  int i, j;
  do{
    for(j = 1; j <= 3; j++){
      Serial.println("Reading impulse ");
      Serial.print(j);
      Serial.print(" in the next 3 seconds");
      for(i = 3; i != 0; i--){
        Serial.println(i);
        delay(1000);
      }
      Serial.println("Reading");
      average = average + analogRead(myoIn);
    }
  } while(average < 300);
  return (int)(average/3);
}

int check(int voltage){
  if(voltage > trigger){
    return 1;
  }
  return 0;
}

void handPosition(int thumbPos,int pointerPos,int middlePos,int ringPos,int pinkyPos){
  //function hand position takes in 5 integer inputs for each finger and moves the servos according to
  //each input. for the servos, maximum extension is 1000 and minimum (all the way in) is 2000.
  thumb.writeMicroseconds(thumbPos);   //this is a function from the servo.h library basically gives the pulse width
  pointer.writeMicroseconds(pointerPos);
  middle.writeMicroseconds(middlePos);
  ring.writeMicroseconds(ringPos);
  pinky.writeMicroseconds(pinkyPos);
  delay(1000);       //this is so the servos have time to get to their set position
  thumb.writeMicroseconds(thumb.readMicroseconds());
  pointer.writeMicroseconds(pointer.readMicroseconds());
  middle.writeMicroseconds(middle.readMicroseconds());
  ring.writeMicroseconds(ring.readMicroseconds());
  pinky.writeMicroseconds(pinky.readMicroseconds());
  delay(250);
}
