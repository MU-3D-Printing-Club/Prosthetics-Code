char command = 'i';

#include <Servo.h>  //use the library
Servo thumb;            //defines the name
Servo pointer;
Servo middle;
Servo ring;
Servo pinky;
#define thumbPin 3
#define pointerPin 5
#define middlePin 6
#define ringPin 9
#define pinkyPin 10
char inString = 'a';
#define max 1000    //should be right
#define actualMin 2000
#define min 2000

void bat() {
  Serial1.println(analogRead(A0)/(1024.0));
}

void setup() {
  Serial1.begin(9600);
  thumb.attach(thumbPin, actualMin, max);
  pointer.attach(pointerPin, actualMin, max);
  middle.attach(middlePin, actualMin, max);
  ring.attach(ringPin, actualMin, max);
  pinky.attach(pinkyPin, actualMin, max);
  bat();
  Serial1.println("Begin");
  palm();
}

void loop() {
  bat();
  fist();
  palm();
  if (Serial1.available() > 0) {
    command = Serial1.read();
    switch (command) {
      case 'f':
        fist();
        Serial1.println(command);
        break;
      case 'p':
        palm();
        Serial1.println(command);
        break;
      case 'm':
        mid();
        Serial1.println(command);
        break;
      case 't':
        thumbsUp();
        Serial1.println(command);
        break;
      case 'h':
        hangTen();
        Serial1.println(command);
        break;
        case 's':
        handShake(max+500);
        Serial1.println(command);
        break;
        case 'b':
        showOff();
        Serial1.println(command);
        break;
    }
  }
}

//functions--------------------------

void fist()
{
  pointer.writeMicroseconds(min);
  middle.writeMicroseconds(min);
  ring.writeMicroseconds(min);
  pinky.writeMicroseconds(min);
  
  delay(700);
  thumb.writeMicroseconds(min);
  delay(1000);
}

void palm()
{
  thumb.writeMicroseconds(max);
  pointer.writeMicroseconds(max);
  middle.writeMicroseconds(max);
  ring.writeMicroseconds(max);
  pinky.writeMicroseconds(max);
  delay(1000);
}

void thumbsUp()
{
  thumb.writeMicroseconds(max);
  pointer.writeMicroseconds(min);
  middle.writeMicroseconds(min);
  ring.writeMicroseconds(min);
  pinky.writeMicroseconds(min);
  delay(1000);
}

void mid()
{
  thumb.writeMicroseconds(min);
  pointer.writeMicroseconds(min);
  middle.writeMicroseconds(max);
  ring.writeMicroseconds(min);
  pinky.writeMicroseconds(min);
  delay(1000);
}

void hangTen()
{
  thumb.writeMicroseconds(max);
  pointer.writeMicroseconds(min);
  middle.writeMicroseconds(min);
  ring.writeMicroseconds(min);
  pinky.writeMicroseconds(max);
  delay(1000);
}

void handShake(int pp)
{
  thumb.writeMicroseconds(pp-100);
  pointer.writeMicroseconds(pp);
  middle.writeMicroseconds(pp+30);
  ring.writeMicroseconds(pp+150);
  pinky.writeMicroseconds(pp+300);
  delay(1000);
}

void showOff()
{
  #define d 1000
  pinky.writeMicroseconds(min);
  delay(d);
  ring.writeMicroseconds(min);
  pinky.writeMicroseconds(max);
  delay(d);
  middle.writeMicroseconds(min);
  ring.writeMicroseconds(max);
  delay(d);
  pointer.writeMicroseconds(min);
  middle.writeMicroseconds(max);
  delay(d);
  pointer.writeMicroseconds(max);
  thumb.writeMicroseconds(min);
  delay(d);
  thumb.writeMicroseconds(max);
}

char handWrite(char inChar) {
  switch (inChar) {
    case 'f':
      fist();
      break;
    case 'p':
      palm();
      break;
  }
}
