/*
this sketch is a skeleton of a generic sensor sketch, at the moment it is deisgned for the aneometer.

      TO DO
      =====
* genralise sketch
* import time object
*change magik number - arraySIZE
*instead of dataPoint array, could it be a String array and we can just separate the value from timestamp with a / or something?
*/
//create new instance of sensor with pin number and resolution

#include <Time.h>  
#include <Wire.h>  
#include <DS1307RTC.h>  // a basic DS1307 library that returns time as a time_t

#define LED 13            //define variable led to pin 13
#define RESOLUTION 10000   //define resolution of anemometer sensor
#define HALL_EFFECT 8     //define sensor pin
#define HALL_ON 100       //used to debounce the sensor if the magnet is ontop of sensor and is static
#define DP_SIZE 3      //size of dataPointArray
//Sensor sensor(13,1000);

//STEP 1 - INITIALISATION
void setup(){
  Serial.begin(9600);        //begin serial
  setSyncProvider(RTC.get);   // the function to sync the time from the RTC  
  pinMode(LED, OUTPUT);      //set pin mode to output
  pinMode(HALL_EFFECT,INPUT);      //set input pin
}

void loop(){
//STEP 2 - READ SENSORS
 print2();
//STEP 3 - PROCESSING
}

//CLASS DECLARATION
class Sensor {
  //private functions and variables
  private:
  byte pin;                             //the pin the sensor is plugged into, on the arduino board
  unsigned int resolution;              //the delay time between saving dataPoints
  unsigned long previousMillis;         //variable to store previousMillis for interval
  byte arrayPosition;                  //to store array position
  
  //public funtions and variables
  public:
  struct dataPoint{
    byte value;        //to hold value of sensor
    time_t timeStamp; //standard C time_t so elapsed times can be easily calculated and time values shared across different platforms.
  };
  //CONSTRUCTORS
  Sensor();
  Sensor(byte, unsigned int);
  //FUNCTIONS
  unsigned int getData();               //contains a while loop to listen for sesnor data from pin
  void reportData();                    //push dataPoint array to higher level
  void clearData();                     //clear dataPoint array
  byte getPin(void) {return pin;} 
  int getResolution(void) {return resolution;} 
  void addDataPoint(byte);            
  dataPoint dataPointArray[DP_SIZE];              //dataPoint array should contain all datapoints created in 
  void printOutput();
  byte printValue(int);
  time_t printTimeStamp(int);
  void begin();
  
};//END SENSOR CLASS

//DEFAULT CONSTRUCTOR
Sensor::Sensor() {
  pin = 13;
  resolution = 10000;
}//END CONSTRUCTOR

//CORRECT CONSTRUCTOR
Sensor::Sensor (byte p, unsigned int r) {
  pin = p;              //pin number 
  resolution = r;       //resolution of data
  struct dataPoint;
}//END CONSTRUCTOR


//FUNCTION getData()
//count and return number of times sensor is triggered in resolution
unsigned int Sensor::getData() {
  int count = 0;                                //local scope variable
  int staticCount;
  boolean MAGNET = true;                        //variable to monitor if magnet is static over sensor
  while ((unsigned long)(millis() - previousMillis) <= resolution){ //while time passed is less than interval
    if ((!digitalRead(pin)) && MAGNET) {      //if sensor is triggered 
      count++;            
      staticCount = 0;
      while ((!digitalRead(pin)) && (staticCount < HALL_ON)){
        staticCount++;
        blink();
        //Serial.print("Static count = ");
        //Serial.println(staticCount);
        if (staticCount >= HALL_ON){
          Serial.println("Magnet is ontop of sensor and static");      
          MAGNET = false;
        }//END IF
      }//END WHILE
    }//END IF
  }//END WHILE
  previousMillis = millis();      //set previous time to current time
  return count;                   //return total counts
}//end loop

//this function takes in a value and creates a timestamp and adds them both to the dataPointArray
void Sensor::addDataPoint(byte v) {
  time_t timeNow = now();              //get the current time
  dataPoint newDataPoint;              //create instance of structure dataPoint
  newDataPoint.value = v;              //add value to structure
  newDataPoint.timeStamp = timeNow;    //add the timestamp to 
  dataPointArray[arrayPosition] = newDataPoint;
  arrayPosition++;
}
void reportData(){
}
void Sensor::clearData(){
  for (int i = 0; i < DP_SIZE; i++) {
  dataPointArray[i].value = 0;
  dataPointArray[i].timeStamp = 0;
  }
}

byte Sensor::printValue(int i){
  return dataPointArray[i].value;
}
time_t Sensor::printTimeStamp(int i){
  return dataPointArray[i].timeStamp;
}
void Sensor::printOutput(){
  for (int i = 0; i < DP_SIZE; i++) {
    Serial.print("VALUE = ");
  Serial.println(printValue(i));
  Serial.print("TIME STAMP = ");
  Serial.println(printTimeStamp(i));
  Serial.println("");
  }
}

void Sensor::begin(){
  while(arrayPosition < DP_SIZE){
  int data = getData();
  addDataPoint(data);
  }
  printOutput();
  arrayPosition = 0;
  clearData();
  printOutput();
}
void print2() {
  Sensor anemometer (HALL_EFFECT,RESOLUTION);
  anemometer.begin();
}
 
//STEP 4 - OUTPUT
//(STEP 5 - TERMINATION)

void blink(){
  digitalWrite(LED, HIGH); 
  delay(10);
  digitalWrite(LED,LOW);
}
