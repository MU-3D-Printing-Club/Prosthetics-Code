//this code takes input from a myoelctric and toggles the hand between the contracted
// and extended positions when there is a spike above a user-defined level
// future projects will include running this code with low power
 
#include <Servo.h>     //library used for servos
 
Servo thumb;            //defines the name of the five servos and them as servo objects
Servo pointer;
Servo middle;
Servo ring;
Servo pinky;
 
#define extendmin 2000    //static integers of extendminimum and extendmaximum servo extension
#define extendmax 1000

int trigger = 600; //default value incase if all fails
int rotation = 2; //set to 4 for more opitons

#define thumbPin 3    //these are the PWM pins where servos are connected
#define pointerPin 5
#define middlePin 6
#define ringPin 9
#define pinkyPin 10

#define overload 1028
 
int state = 1;    //sets default state to be open
#define threshold .4    //this is the percentage of the extendmax read we use to define a flex
#define myoIn A1      //the pin where myo signal comes in

int setTrigger();
int check(int);
void collect(int *, int *, int *);
void handPosition(int,int,int,int,int);
 
// THIS PART ONLY RUNS ONCE
void setup() {
Serial.begin(9600);     //starts up serial communication between arduino and computer
  Serial.println("linking servos");
  thumb.attach(thumbPin, extendmin, extendmax);    //defines where the servos are and their extendmax/extendmins
  pointer.attach(pointerPin, extendmin, extendmax);
  middle.attach(middlePin, extendmin, extendmax);
  ring.attach(ringPin, extendmin, extendmax);
  pinky.attach(pinkyPin, extendmin, extendmax);
  Serial.println("Starting");       //makes the hand relax all of the way
  handPosition(extendmax,extendmax,extendmax,extendmax,extendmax);
  Serial.println("Reading in 3");
  delay(1000);
  Serial.println("Reading in 2");
  delay(1000);
  Serial.println("Reading in 1");
  delay(1000);
  trigger = setTrigger();
}

//setup starts by linking the Servo objects to each pin and detailing the limits per each argument. Then it opens the hand for a default position and will call the setTrigger function. Once it's collected, the setup is complete

void loop() {
  Serial.println(analogRead(myoIn));
  float currentVoltage=0;
  if(check(currentVoltage = analogRead(myoIn))){//collects voltage, then assigns to currentVoltage, then passes currentVoltage to check function, then checks the check return value to see if it's higher than trigger value
    state++; //increases our state
    state = state%rotation; //ensoures our only values are between 0-3 and loops back if otherwise
    Serial.print("state is ");
    Serial.println(state);
    //Serial.println(currentVoltage);        //prints in the serial moniter
    if(state == 0){
      Serial.println("Open");
      handPosition(extendmax,extendmax,extendmax,extendmax,extendmax);
    }
    else if(state == 1){
      Serial.println("Fist");
      handPosition(extendmin,extendmin,extendmin,extendmin,extendmin);
    }
    else if(state == 2){
      Serial.println("Point");
      handPosition(extendmax,extendmin,extendmax,extendmax,extendmax);
    }
    else if(state == 3){
      Serial.println("Pinch");
      handPosition(extendmin,extendmin,extendmin,extendmax,extendmax);
    }
    else {
      state = 1; //edgecase if logic fail
      handPosition(extendmax,extendmax,extendmax,extendmax,extendmax);
    }
  }
}

//the loop function will continously check the users input and assign it to currentVoltage, then it's verified through the check function to see if its higher than the set trigger value from setup, if so then it will switch between the four hand positions in a looping structure

/*int setTrigger(){
  float average = 0;
  int i, j;
  do{
    average = 0;//resets collection value if it failed prior
    for(j = 1; j <= 3; j++){//loops to collect the three impulses
      Serial.print("Reading impulse ");
      Serial.print(j);
      Serial.println(" in the next 3 seconds");
      for(i = 3; i != 0; i--){//counts down 3 seconds
        Serial.println(i);
        delay(1000);
      }
      Serial.println("Reading");
      average = average + analogRead(myoIn);//adds up collected values with previous values
      Serial.print("Total is:");
      Serial.println(average);
      
    }
    
      average = average/3;
      Serial.print("Average is:");
      Serial.println(average);
  } while((average) < 200);//if it's less than 200 than we reattempt collection
  return (int)(average);//returns average upon success
}*/


int setTrigger(){
  float average = 0;
  int i, j;
  int max1, max2, max3;
  int loopcount, starttime, endtime;
  do{
    average = 0;//resets collection value if it failed prior
    max1 = 0;
    max2 = 0;
    max3 = 0;
    loopcount = 0;
    starttime = millis();
    endtime = starttime;
    while ((endtime - starttime) < 3000) // do this loop for about 1 second
    {
    collect(&max1,&max2,&max3);
    loopcount = loopcount+1;
    endtime = millis();
    }
    average = (max1 + max2 + max3)/3;
    Serial.println("max1");
    Serial.print(max1);
    Serial.println("max2");
    Serial.print(max2);
    Serial.println("max3");
    Serial.print(max3);
    Serial.println("average");
    Serial.println(average);
    Serial.println("loop");
    Serial.println(loopcount);
  } while((average) < 200);//if it's less than 200 than we reattempt collection
  return (int)(average);//returns average upon success
}

void collect(int * max1, int * max2, int * max3){
  float prevVolt = analogRead(myoIn);
  float currVolt = analogRead(myoIn);
  float nextVolt = analogRead(myoIn);
  if(currVolt < overload && prevVolt < currVolt && currVolt > nextVolt){
    if(currVolt > *max1){
      *max1 = currVolt;
      return;
    }
    else if(currVolt > *max2){
      *max2 = currVolt;
      return;
    }
    else if(currVolt > *max3){
      *max3 = currVolt;
      return;
    }
  }
}

//setTrigger collects 3 inputs by the user with a 3 second delay between each one. It calculates the average and if it's lower than 200, it's an assumed error and will reset the collection. Once completed it returns the average

int check(int voltage){
  if(voltage > trigger && voltage < overload){
    return 1;
  }
  return 0;
}

//check collects the voltage, and if it's greater than the trigger value, will return 1 for true, else 0 for false

void handPosition(int thumbPos,int pointerPos,int middlePos,int ringPos,int pinkyPos){
  //function hand position takes in 5 integer inputs for each finger and moves the servos according to
  //each input. for the servos, extendmaximum extension is 1000 and extendminimum (all the way in) is 2000.
   //this is a function from the servo.h library basically gives the pulse width
  pointer.writeMicroseconds(pointerPos);
  middle.writeMicroseconds(middlePos);
  ring.writeMicroseconds(ringPos);
  pinky.writeMicroseconds(pinkyPos);
  delay(200);
  thumb.writeMicroseconds(thumbPos);  
  delay(1000);       //this is so the servos have time to get to their set position
  thumb.writeMicroseconds(thumb.readMicroseconds()); //adjusts servos to adjust desired position to actual position reached to avoid burnout
  pointer.writeMicroseconds(pointer.readMicroseconds());
  middle.writeMicroseconds(middle.readMicroseconds());
  ring.writeMicroseconds(ring.readMicroseconds());
  pinky.writeMicroseconds(pinky.readMicroseconds());
  delay(250);
}

//handPosition takes 5 integer values to be set to each servo for the finger, with one second of time to adjust, their position is read and if not accurate, will adjust servo command to stay at the position
