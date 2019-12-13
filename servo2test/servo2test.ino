#include"ServoTimer2.h"
ServoTimer2 servo1;
void setup() 
{
servo1.attach(6);   
}
void loop() 
{                 // put your main code here, to run repeatedly:
servo1.write(1000);  //min pulse width for 0 degree
delay(1000);
servo1.write(1500);  //pulse width for 90 degree
delay(1000);
servo1.write(2000);  //max pulse width for around 180 degree
delay(1000);
}
