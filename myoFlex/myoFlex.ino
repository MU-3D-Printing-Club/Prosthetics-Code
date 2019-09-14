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

#define thumbPin 3    //these are the PWM pins where servos are connected
#define pointerPin 5
#define middlePin 6
#define ringPin 9
#define pinkyPin 10


float currentVoltage=0;   //this is where to store each read from the myo
#define threshold .4    //this is the percentage of the max read we use to define a flex
#define myoIn A1      //the pin where myo signal comes in

bool contract=false;   //variable we will use to toggle action

// THIS PART ONLY RUNS ONCE
void setup() {
Serial.begin(9600);     //starts up serial communication between arduino and computer
  thumb.attach(thumbPin, min, max);    //defines where the servos are and their max/mins
  pointer.attach(pointerPin, min, max);
  middle.attach(middlePin, min, max);
  ring.attach(ringPin, min, max);
  pinky.attach(pinkyPin, min, max);
  Serial.println("Begin");    //prints a start message
  handPosition(max,max,max,max,max);       //makes the hand relax all of the way
}

// THIS PART OF THE CODE LOOPS CONTINIOUSLY 
void loop() {
 currentVoltage = analogRead(myoIn);     //reads myoelectric input
 currentVoltage=currentVoltage/1023;      // converts from 0-1023 scale to 0-1 scale
 Serial.println(currentVoltage);        //puts it in the serial moniter
 if (contract==false && currentVoltage>threshold){      //if we see a spike and the hand is relaxed
 Serial.println("Contract!");     
  handPosition(min,min,min,min,min);            //make it squeeze; see subfunction below loop body
  contract=true;                                 //change the boolean
 }else if (contract==true && currentVoltage>threshold){   //if it is already contracted then make it relax
  Serial.println("Extend!");
  handPosition(max,max,max,max,max);
  contract=false;
}
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
}
