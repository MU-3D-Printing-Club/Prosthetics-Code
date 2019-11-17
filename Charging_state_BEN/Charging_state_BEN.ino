#include <Adafruit_NeoPixel.h>  //used for the neopixel

#define neoPin 11 //NeoPixel Multicolor LED
#define chargingStatePin 12
#define chargingErrorPin 4
#define N_LEDS 0
#define batteryVoltagePin A0


Adafruit_NeoPixel neoPixel = Adafruit_NeoPixel(N_LEDS, neoPin, NEO_GRB + NEO_KHZ800);

void setup() {
  // put your setup code here, to run once:
    neoPixel.begin();
    pinMode(chargingStatePin, INPUT);
    pinMode(chargingErrorPin, INPUT);
    pinMode(neoPin, OUTPUT);
}

void loop() {
  int chargingState = digitalRead(chargingStatePin);
  int chargingError = digitalRead(chargingErrorPin);
  unsigned long duration = pulseIn(batteryVoltagePin, HIGH);

  uint32_t red = neoPixel.Color(255, 0, 0);
  uint32_t orange = neoPixel.Color(255, 165, 0);
  uint32_t green = neoPixel.Color(144, 238, 144);

  if(duration == 256){
    neoPixel.fill(red);
    neoPixel.show();
    Serial.println("Error in charging has occured");
    delay(500);
  }

  else if(chargingState == LOW){
    neoPixel.fill(orange);
    neoPixel.show();
    Serial.println("The device is actively charging");
    delay(500);
  }

  else if(chargingState == HIGH){
    neoPixel.fill(green);
    neoPixel.show();
    Serial.println("The device is fully charged");
    delay(500);
  }

  else{
    Serial.println("AN ERROR OCCURED: UNABLE TO DETERMINE CHARGING STATUS");
    neoPixel.fill(red);
    neoPixel.show();
    delay(500);
    neoPixel.clear();
    neoPixel.show();
    delay(500);
  }

}
