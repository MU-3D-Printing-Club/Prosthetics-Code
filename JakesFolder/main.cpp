#include <Arduino.h>
// ***** Old Battery Check Code
int analogValue = 0;
float voltage = 0;
int ledDelay = 200;
// *****




#define A0 14
// Function Declaration
int getBandgap();

void setup() {
  Serial.begin(9600);
  Serial.println("Setting up...");
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(1000);
  Serial.println("Checking Battery Voltage...\n");
  analogValue = analogRead(A0);
  voltage = analogValue * .0048;
  if (voltage > 0.1) {
    Serial.println(voltage);
  }
   


  // int batteryResults = getBandgap();
  // //Serial.println("Result: %d", batteryResults);
  // Serial.print("Result:");
  // Serial.println(batteryResults);

}

//const long InternalReferenceVoltage = 1062;  // Adjust this value to your board's specific internal BG voltage
 
// Code courtesy of "Coding Badly" and "Retrolefty" from the Arduino forum
// results are Vcc * 100
// So for example, 5V would be 500.
// int getBandgap (){
//     // REFS0 : Selects AVcc external reference
//     // MUX3 MUX2 MUX1 : Selects 1.1V (VBG) 
//    // ADMUX = bit (REFS0) | bit (MUX3) | bit (MUX2) | bit (MUX1);
//     ADMUX = bit (A0) | bit (MUX3) | bit (MUX2) | bit (MUX1);
//     ADCSRA |= bit( ADSC );  // start conversion
//     while (ADCSRA & bit (ADSC))
//       { }  // wait for conversion to complete (toss this measurement)
//     ADCSRA |= bit( ADSC );  // start conversion
//     while (ADCSRA & bit (ADSC))
//       { }  // wait for conversion to complete
//     int results = (((InternalReferenceVoltage * 1024) / ADC) + 5) / 10;

//   return results;
//   } // end of getBandgap