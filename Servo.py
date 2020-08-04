#include <Servo.h>
Servo ESC;     // create servo object to control the ESC
int potValue;  // value from the analog pin
void setup() {
  // Attach the ESC on pin 9
  ESC.attach(9,1000,2000); // (pin, min pulse width, max pulse width in microseconds) 

  ESC.write(2000);
  delay(2000);
  ESC.write(1000);
  delay(2000);
  
}

void calibrate(int ctr,int setfreq){
  int phaseL = 200;
  if(ctr < phaseL){ //mid
   // ESC.write(1500);  
  }
  if(ctr > phaseL && ctr < phaseL*2){ //high
    ESC.write(2000);
  }
  if(ctr > phaseL*2 && ctr < phaseL*3){ //low
    ESC.write(1000);
  }
  if(ctr>phaseL*3){
    ESC.write(setfreq);
   }
}
int ctr = 0;
void loop() {
  //potValue = analogRead(A0);   // reads the value of the potentiometer (value between 0 and 1023)
  //potValue = map(potValue, 0, 1023, 0, 180);   // scale it to use it with the servo library (value between 0 and 180)
  ESC.write(1100+ctr);    // Send the signal to the ESC
  delay(100);
  ctr++;
}
