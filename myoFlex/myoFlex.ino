//this code takes input from a myoelctric and toggles the hand between the contracted
// and extended positions when there is a spike above a user-defined level
// future projects will include running this code with low power

#include <Adafruit_NeoPixel.h>
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
bool isLocked = false; //0 is not locked
bool isIniting = true;

enum pins
{
  THUMB_PIN = 3,
  INDEX_PIN = 5,
  MIDDLE_PIN = 6,
  RING_PIN = 9,
  PINKY_PIN = 10,
  VBAT_PIN = A0,
  MYO_PIN = A5,
  LED_PIN = 11,
  BUTTON_PIN = 7
};

enum hand_state
{
  OPEN, FIST, POINT, PINCH
};

Adafruit_NeoPixel led(1, LED_PIN, NEO_RGB + NEO_KHZ800);

#define overload 1028

int state = OPEN;    // sets default state to be open
#define threshold .4    // this is the percentage of the extendmax read we use to define a flex

int setTrigger();
bool check(int);
void collect(int *, int *, int *);
void handPosition(int, int, int, int, int);
void locked();
void set_led();

//wait 10 seconds then set led color for 1 second based on battery voltage
ISR(TIMER3_OVF_vect)
{
  static char count = -1;

  if (isIniting) {
    count = ++count % 4;
    if (count < 2)
      led.setPixelColor(0, 0, 0, 20);
    else
      led.setPixelColor(0, 0, 0, 0);
  } else {
    count = ++count % 40; // 0.1hz real
    //count = ++count % 8; // 1hz test
    if (count < 4) {
      const float battery_voltage = analogRead(VBAT_PIN) * 0.0049; //not reading reliably
      //const float battery_voltage = 2.5; test code

      const float normalized = battery_voltage - 3.0;

      float light_level = 20.0 * normalized;

      light_level = light_level < 0.0 ? 0.0 : light_level;
      light_level = light_level > 20.0 ? 20.0 : light_level;

      led.setPixelColor(0, 20.0 - light_level, light_level, 0);

      Serial1.print("Battery Voltage: ");
      Serial1.println(battery_voltage);
    } else
      led.setPixelColor(0, 0, 0, 0);
  }
  led.show();
}

// THIS PART ONLY RUNS ONCE
void setup() {
  isIniting = true;

  led.begin();
  led.clear(); //make sure led is off on startup because it will be a super bright blue otherwise
  led.show();

  //put in normal mode because this is set somewhere else beforehand
  TCCR3A = 0;
  //set prescaler to 64
  TCCR3B = 3;
  //enable timer interupt on overflow
  TIMSK3 = 1;

  Serial1.begin(9600);     // starts up serial communication between arduino and computer

  Serial1.println("linking servos");
  thumb.attach(THUMB_PIN, extendmin, extendmax);    //defines where the servos are and their extendmax/extendmins
  pointer.attach(INDEX_PIN, extendmin, extendmax);
  middle.attach(MIDDLE_PIN, extendmin, extendmax);
  ring.attach(RING_PIN, extendmin, extendmax);
  pinky.attach(PINKY_PIN, extendmin, extendmax);

  pinMode(BUTTON_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), locked, FALLING);

  Serial1.println("Starting");       //makes the hand relax all of the way
  handPosition(extendmax, extendmax, extendmax, extendmax, extendmax);

  Serial1.println("Reading in 3");
  delay(1000);
  Serial1.println("Reading in 2");
  delay(1000);
  Serial1.println("Reading in 1");
  delay(1000);

  trigger = setTrigger();
  isIniting = false;
}

//setup starts by linking the Servo objects to each pin and detailing the limits per each argument. Then it opens the hand for a default position and will call the setTrigger function. Once it's collected, the setup is complete

void loop() {
  float currentVoltage = 0;

  if (!isLocked && check(currentVoltage = analogRead(MYO_PIN))) { // collects voltage, then assigns to currentVoltage, then passes currentVoltage to check function, then checks the check return value to see if it's higher than trigger value

    state = (++state) % (FIST + 1); // increment through available state values

    Serial1.print("state is ");
    Serial1.println(state);

    Serial1.print("myo voltage is: ");
    Serial1.println(currentVoltage); // prints in the Serial1 moniter

    switch (state) {
      case OPEN:
        Serial1.println("Open");
        handPosition(extendmax, extendmax, extendmax, extendmax, extendmax);
        break;
      case FIST:
        Serial1.println("Fist");
        handPosition(extendmin, extendmin, extendmin, extendmin, extendmin);
        break;
      case POINT:
        Serial1.println("Point");
        handPosition(extendmax, extendmin, extendmax, extendmax, extendmax);
        break;
      case PINCH:
        Serial1.println("Pinch");
        handPosition(extendmin, extendmin, extendmin, extendmax, extendmax);
        break;
      default:
        state = OPEN; //edgecase if logic fail
        handPosition(extendmax, extendmax, extendmax, extendmax, extendmax);
        break;
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
      Serial1.print("Reading impulse ");
      Serial1.print(j);
      Serial1.println(" in the next 3 seconds");
      for(i = 3; i != 0; i--){//counts down 3 seconds
        Serial1.println(i);
        delay(1000);
      }
      Serial1.println("Reading");
      average = average + analogRead(myoIn);//adds up collected values with previous values
      Serial1.print("Total is:");
      Serial1.println(average);

    }

      average = average/3;
      Serial1.print("Average is:");
      Serial1.println(average);
  } while((average) < 200);//if it's less than 200 than we reattempt collection
  return (int)(average);//returns average upon success
  }*/


int setTrigger() {
  float average = 0;
  int i, j;
  int max1, max2, max3;
  int loopcount, starttime, endtime;
  do {
    average = 0;//resets collection value if it failed prior
    max1 = 0;
    max2 = 0;
    max3 = 0;
    loopcount = 0;
    starttime = millis();
    endtime = starttime;
    while ((endtime - starttime) < 3000) // do this loop for about 1 second
    {
      collect(&max1, &max2, &max3);
      loopcount = loopcount + 1;
      endtime = millis();
    }
    average = (max1 + max2 + max3) / 3;
    Serial1.println("max1");
    Serial1.print(max1);
    Serial1.println("max2");
    Serial1.print(max2);
    Serial1.println("max3");
    Serial1.print(max3);
    Serial1.println("average");
    Serial1.println(average);
    Serial1.println("loop");
    Serial1.println(loopcount);
  } while ((average) < 200); //if it's less than 200 than we reattempt collection
  return (int)(average);//returns average upon success
}

void collect(int * max1, int * max2, int * max3) {
  float prevVolt = analogRead(MYO_PIN);
  float currVolt = analogRead(MYO_PIN);
  float nextVolt = analogRead(MYO_PIN);
  if (currVolt < overload && prevVolt < currVolt && currVolt > nextVolt) {
    if (currVolt > *max1) {
      *max1 = currVolt;
      return;
    }
    else if (currVolt > *max2) {
      *max2 = currVolt;
      return;
    }
    else if (currVolt > *max3) {
      *max3 = currVolt;
      return;
    }
  }
}

//setTrigger collects 3 inputs by the user with a 3 second delay between each one. It calculates the average and if it's lower than 200, it's an assumed error and will reset the collection. Once completed it returns the average

bool check(int voltage) {
  return voltage > trigger && voltage < overload;
}

//check collects the voltage, and if it's greater than the trigger value, will return 1 for true, else 0 for false

void locked() {
  static unsigned long last_lock_time = 0;

  if (last_lock_time > millis()) // if the last lock time is in the future, there probably was an overflow so we should reset
    last_lock_time = 0;

  if (millis() - last_lock_time > 1000) { // only change state once a second to prevent button bouncing issues
    last_lock_time = millis();
    isLocked = !isLocked;
    Serial1.print("Button State: ");
    Serial1.println(isLocked);
  }
}

void handPosition(int thumbPos, int pointerPos, int middlePos, int ringPos, int pinkyPos) {
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
