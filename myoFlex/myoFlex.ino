<<<<<<< Updated upstream
//this code takes input from a myoelctric and toggles the hand between the contracted
// and extended positions when there is a spike above a user-defined level
// future projects will include running this code with low power
//jd is cool
//jd is cooler


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
#define buttonPin 0 //This is the pin the button is located(NEEDS TO BE CHANGED TO REFLECT THE ACTUAL LOCATION ON THE HARDWARE)


float currentVoltage=0;   //this is where to store each read from the myo
#define threshold .4    //this is the percentage of the max read we use to define a flex
#define myoIn A1      //the pin where myo signal comes in

bool contract=false;   //variable we will use to toggle action
int buttonState = 0;  //variable for reading the pushButton status

// THIS PART ONLY RUNS ONCE
void setup() {
  Serial.begin(9600);     //starts up serial communication between arduino and computer
  thumb.attach(thumbPin, min, max);    //defines where the servos are and their max/mins
  pointer.attach(pointerPin, min, max);
  middle.attach(middlePin, min, max);
  ring.attach(ringPin, min, max);
  pinky.attach(pinkyPin, min, max);

  pinMode(buttonPin, INPUT);  //initializes buttonPin as an input
  Serial.println("Begin");    //prints a start message
  handPosition(max,max,max,max,max);       //makes the hand relax all of the way
}

// THIS PART OF THE CODE LOOPS CONTINIOUSLY
void loop() {
  buttonState = digitalRead(buttonPin);   //reads the state of the pushButton value
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

  if(buttonState == HIGH){
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
  else{
    Serial.println("The hand is now turned off");
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
=======
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

int trigger = 600; //default value incase if all fails

#define thumbPin 3    //these are the PWM pins where servos are connected
#define pointerPin 5
#define middlePin 6
#define ringPin 9
#define pinkyPin 10
 
int state = 1;    //sets default state to be open
float currentVoltage=0;   //this is where to store each read from the myo
#define threshold .4    //this is the percentage of the max read we use to define a flex
#define myoIn A1      //the pin where myo signal comes in

int setTrigger();
int check(int);
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
  handPosition(max,max,max,max,max);
  trigger = setTrigger();
}

//setup starts by linking the Servo objects to each pin and detailing the limits per each argument. Then it opens the hand for a default position and will call the setTrigger function. Once it's collected, the setup is complete

void loop() {
  Serial.println(analogRead(myoIn));
  if(check(currentVoltage = analogRead(myoIn))){//collects voltage, then assigns to currentVoltage, then passes currentVoltage to check function, then checks the check return value to see if it's higher than trigger value
    state++; //increases our state
    state = state%4; //ensoures our only values are between 0-3 and loops back if otherwise
    Serial.print("state is ");
    Serial.println(state);
    //Serial.println(currentVoltage);        //prints in the serial moniter
    if(state == 0){
      Serial.println("Open");
      handPosition(max,max,max,max,max);
    }
    else if(state == 1){
      Serial.println("Point");
      handPosition(max,min,max,max,max);
    }
    else if(state == 2){
      Serial.println("Pinch");
      handPosition(min,min,min,max,max);
    }
    else if(state == 3){
      Serial.println("Fist");
      handPosition(min,min,min,min,min);
    }
    else {
      state = 1; //edgecase if logic fail
      handPosition(max,max,max,max,max);
    }
  }
}

//the loop function will continously check the users input and assign it to currentVoltage, then it's verified through the check function to see if its higher than the set trigger value from setup, if so then it will switch between the four hand positions in a looping structure

int setTrigger(){

  float average;
  do{
    //Finds the max flex for a total of 3 trials
    float sumMaxFlexOfTrials = 0;
    
    for(int j = 1; j <= 3; j++){  //loops to collect the three impulses
      Serial.print("Reading max of Flex ");
      Serial.print(j);
      Serial.println(" reading during the next 3 seconds");
      float maxRecordedFlex = 0;
      for(int i = 3000; i != 0; i -= 300){  //records values every 1/5 of a second and keeps track of which reading was the max
        float reading1 = analogRead(myoIn);
        delay(100);
        float reading2 = analogRead(myoIn);
        delay(100);
        float reading3 = analogRead(myoIn); 
        float currentReading = reading2;
        
        if(currentReading > reading1 && currentReading < reading3 && currentReading > maxRecordedFlex)
          maxRecordedFlex = currentReading;
        delay(100);
      }

      sumMaxFlexOfTrials += maxRecordedFlex; //Adds the max of this trial to the sum of the maxes of all the trials
    }

    average = sumMaxFlexOfTrials/3;
  } while(average < 200);//if it's less than 200 than we reattempt collection

  return (int)(average);//returns average upon success
}

int check(int voltage){
  if(voltage > trigger){
    return 1;
  }
  return 0;
}

//check collects the voltage, and if it's greater than the trigger value, will return 1 for true, else 0 for false

void handPosition(int thumbPos,int pointerPos,int middlePos,int ringPos,int pinkyPos){
  //function hand position takes in 5 integer inputs for each finger and moves the servos according to
  //each input. for the servos, maximum extension is 1000 and minimum (all the way in) is 2000.
  thumb.writeMicroseconds(thumbPos);   //this is a function from the servo.h library basically gives the pulse width
  pointer.writeMicroseconds(pointerPos);
  middle.writeMicroseconds(middlePos);
  ring.writeMicroseconds(ringPos);
  pinky.writeMicroseconds(pinkyPos);
  delay(1000);       //this is so the servos have time to get to their set position
  thumb.writeMicroseconds(thumb.readMicroseconds()); //adjusts servos to adjust desired position to actual position reached to avoid burnout
  pointer.writeMicroseconds(pointer.readMicroseconds());
  middle.writeMicroseconds(middle.readMicroseconds());
  ring.writeMicroseconds(ring.readMicroseconds());
  pinky.writeMicroseconds(pinky.readMicroseconds());
  delay(250);
}

//handPosition takes 5 integer values to be set to each servo for the finger, with one second of time to adjust, their position is read and if not accurate, will adjust servo command to stay at the position
>>>>>>> Stashed changes
