#include <Adafruit_NeoPixel.h>  //used for the neopixel

#define neoPin 11 //NeoPixel Multicolor LED
#define chargingStatePin 12
#define chargingErrorPin 4
#define N_LEDS 0


Adafruit_NeoPixel strip = Adafruit_NeoPixel(N_LEDS, neoPin, NEO_GRB + NEO_KHZ800);

void setup() {
  // put your setup code here, to run once:
    strip.begin();
}

void loop() {
  // put your main code here, to run repeatedly:

}
