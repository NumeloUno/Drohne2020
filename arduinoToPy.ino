#include <SoftwareSerial.h>
#include <TinyGPS.h>
#include <Wire.h>
#include <Servo.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>
  
Adafruit_BNO055 bno = Adafruit_BNO055(55);
 

/* This sample code demonstrates the normal use of a TinyGPS object.
   It requires the use of SoftwareSerial, and assumes that you have a
   4800-baud serial GPS device hooked up on pins 4(rx) and 3(tx).
*/

TinyGPS gps;
SoftwareSerial ss(4, 3);


class drone{
  public:
  float orX = 0;
  float orY = 0;
  float orZ = 0;
  float gpsLat = 0;
  float gpsLon = 0;

  int rotorSpeed[4] = {0,0,0,0}; //values between 0 and 1000

  void updateGPS(){ //Updating gps takes 1 second so it should only be done occasionaly

    bool newData = false;
    unsigned long chars;
    unsigned short sentences, failed;
    for (unsigned long start = millis(); millis() - start < 1000;)
    {
      while (ss.available())
      {
        char c = ss.read();
        //Serial.write(c); // uncomment this line if you want to see the GPS data flowing
        if (gps.encode(c)) // Did a new valid sentence come in?
          newData = true;
      }
    }
    if (newData)
    {
      float flat, flon;
      unsigned long age;
      gps.f_get_position(&flat, &flon, &age);
      gpsLat = flat;
      gpsLon = flon;
      /* //uncomment for satellite and error data
      Serial.print(" SAT=");
      Serial.print(gps.satellites() == TinyGPS::GPS_INVALID_SATELLITES ? 0 : gps.satellites());
      Serial.print(" PREC=");
      Serial.print(gps.hdop() == TinyGPS::GPS_INVALID_HDOP ? 0 : gps.hdop());
      */
    }
   
  }
  
  void updateOrientation(){
    sensors_event_t event; 
    bno.getEvent(&event);
    orX = event.orientation.x;
    orY = event.orientation.y;
    orZ = event.orientation.z;
  }

  void setRotorSpeed(int a, int b, int c, int d){ //call once to update rotor speed
    rotorSpeed[0] = a;
    rotorSpeed[1] = b;
    rotorSpeed[2] = c;
    rotorSpeed[3] = d;
  }
  void spinRotor(){ //call every iteration
  }

  void printData(){
    if(gpsLat == 0 && gpsLon == 0)Serial.println("GPS not initialized properly");
    if(orX == 0 && orY == 0 && orZ == 0)Serial.println("Gyro not initialized properly. Consider restarting the program.");
    Serial.print("X: ");
    Serial.print(orX, 10);
    Serial.print("\tY: ");
    Serial.print(orY, 10);
    Serial.print("\tZ: ");
    Serial.print(orZ, 10);Serial.println("");
    Serial.print("Lat: ");
    Serial.print(gpsLat, 10);
    Serial.print("\tLon: ");
    Serial.print(gpsLon, 10);Serial.println("");
    Serial.print("r1: ");
    Serial.print(rotorSpeed[0]);
    Serial.print("\tr2: ");
    Serial.print(rotorSpeed[1]);
    Serial.print("\tr3: ");
    Serial.print(rotorSpeed[2]);
    Serial.print("\tr4: ");
    Serial.print(rotorSpeed[3]);
    Serial.println("");
    Serial.println("");
    Serial.println("");
  }


};

drone Drone;



Servo ESC[4]; 
void setup()
{
  // Attach the ESC on pin 9-12
  for(int i = 0; i < 4; i++){
    ESC[i].attach(i+9,1000,2000); // (pin, min pulse width, max pulse width in microseconds)
    ESC[i].write(2000);
  } 
  delay(2000);
  for(int i = 0; i < 4; i++){
    ESC[i].write(1000);
  }
  delay(2000);
 
  
  //setup serial connection 9600 baud
  Serial.begin(9600);
  ss.begin(9600);
  
  if(!bno.begin()) //check if bno set up correctly
  {
    /* There was a problem detecting the BNO055 ... check your connections */
    Serial.print("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
    while(1);
  }
  delay(1000); 
  bno.setExtCrystalUse(true);
}




void processString(String str){
  //Cut string at ; to avoid bug where string gets sent twice (happens often)
  String msg;
  bool valid = false;
  //copy important part of str to actual message (msg)
  for(int i = 1; i < str.length(); i++){
    if(str[i] == ';'){valid = true;break;}
    msg+=str[i];
  }
  //check if message begins with ~ and ends with ;
  if(!valid || str[0] != '~'){Serial.println("invalid MSG");return;}
  else{Serial.println("processing MSG...");}

  //Msg only sets 4 rotor speeds
  for (int i = 0; i < 4; i++){ //4 rotors
    
    Drone.rotorSpeed[i] = 0;
    for(int j = 0; j < 4; j++){ //take 4 chars for each rotor
      //make digit from char and then 4 digit number
      Drone.rotorSpeed[i] += (msg[i*4+j]-48)*pow(10,3-j); 
      }  
  
  }
  
}

void readSerial(){

  String readString;
  while (Serial.available())
  {
    if (Serial.available() >0)
    {
      char c = Serial.read();  //gets one byte from serial buffer
      readString += c; //makes the string readString
    }
  }

  if (readString.length() >0)
  {
    processString(readString);
  }
  
}

int ctr = 1;
void loop()
{
  /* Listen to Python */
  readSerial();
  /* Update sensors */
  if(ctr%10 == 0)Drone.updateOrientation();Drone.printData();
  if(ctr%2000 == 0)Drone.updateGPS();

  for(int i = 0; i < 4; i++){
    ESC[i].write(Drone.rotorSpeed[i]+1075);    // Send the signal to the ESC
  }
  
  /* maintenance */
  delay(10);
  ctr++;
}
