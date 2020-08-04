

#include <Servo.h>
Servo ESC1;     // create servo object to control the ESC
Servo ESC2;     // create servo object to control the ESC
Servo ESC3;     // create servo object to control the ESC
Servo ESC4;     // create servo object to control the ESC
void setup() {
  // Attach the ESC on pin 9
  ESC1.attach(5,1000,2000); // (pin, min pulse width, max pulse width in microseconds) 
  ESC2.attach(6,1000,2000); // (pin, min pulse width, max pulse width in microseconds) 
  ESC3.attach(9,1000,2000); // (pin, min pulse width, max pulse width in microseconds) 
  ESC4.attach(11,1000,2000); // (pin, min pulse width, max pulse width in microseconds) 
 
  ESC1.write(2000);
  ESC2.write(2000);
  ESC3.write(2000);
  ESC4.write(2000);
  delay(2000);
  
  ESC1.write(1000);
  ESC2.write(1000);
  ESC3.write(1000);
  ESC4.write(1000);
  delay(2000);


  
}

void calibrate(int ctr,int setfreq){
  int phaseL = 200;
  if(ctr < phaseL){ //mid
   // ESC.write(1500);  
  }
  if(ctr > phaseL && ctr < phaseL*2){ //high
    ESC1.write(2000);
    ESC2.write(2000);
    ESC3.write(2000);
    ESC4.write(2000);
  }
  if(ctr > phaseL*2 && ctr < phaseL*3){ //low
    ESC1.write(1000);
    ESC2.write(1000);
    ESC3.write(1000);
    ESC4.write(1000);
  }
  if(ctr>phaseL*3){
    ESC1.write(setfreq);
    ESC2.write(setfreq);
    ESC3.write(setfreq);
    ESC4.write(setfreq);
   }
}
int ctr = 0;
void loop() {
  //potValue = analogRead(A0);   // reads the value of the potentiometer (value between 0 and 1023)
  //potValue = map(potValue, 0, 1023, 0, 180);   // scale it to use it with the servo library (value between 0 and 180)
  ESC1.write(1800-ctr*2);
  ESC2.write(1800-ctr*3);
  ESC3.write(1100+ctr*4);
  ESC4.write(1100+ctr);// Send the signal to the ESC
  delay(100);
  ctr++;
}
