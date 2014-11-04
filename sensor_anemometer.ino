/*
this sketch is a skeleton of a generic sensor sketch, at the moment it is deisgned for the aneometer.

      TO DO
      =====
* genralise sketch
* import time object
*change magik number - arraySIZE
*instead of dataPoint array, could it be a String array and we can just separate the value from timestamp with a / or something?
*/
//create new instance of sensor with pin number and SENSOR_RESOLUTION

#include <Time.h>  
#include <Wire.h>  
#include <DS1307RTC.h>  // a basic DS1307 library that returns time as a time_t

//for datalogger
#include <SPI.h>
#include <SD.h>

#define LED 7            //define variable led to pin
#define SENSOR_RESOLUTION 10000   //define SENSOR_RESOLUTION of anemometer sensor
#define HALL_EFFECT 8     //define sensor pin
#define HALL_ON 100       //used to debounce the sensor if the magnet is ontop of sensor and is static    
#define SS 10          //SD card output must be set to output

//Sensor sensor(13,1000);
#define LOG_RESOLUTION 30000   //log data resolution
#define DP_SIZE LOG_RESOLUTION/SENSOR_RESOLUTION     //size of dataPointArray
#define redLED 6  //red error led
const int chipSelect = 10;
File dataFile;

void error(char *str)
{
  Serial.print("error: ");
  Serial.println(str);
  
  // red LED indicates error
  digitalWrite(redLED, HIGH);

  while(1);
}

//STEP 1 - INITIALISATION
void setup(){
  Serial.begin(9600);        //begin serial
  setSyncProvider(RTC.get);   // the function to sync the time from the RTC  
  pinMode(LED, OUTPUT);      //set pin mode to output
  pinMode(HALL_EFFECT,INPUT);      //set input pin
  #if WAIT_TO_START
  Serial.println("Type any character to start");
  while (!Serial.available());
#endif //WAIT_TO_START
  Serial.print("Initializing SD card...");
  pinMode(10, OUTPUT);
    // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    while (1) ;
  }
  Serial.println("card initialized.");
  
 // create a new file
  char filename[] = "LOGGER00.CSV";
  for (uint8_t i = 0; i < 100; i++) {
    filename[6] = i/10 + '0';
    filename[7] = i%10 + '0';
    if (! SD.exists(filename)) {
      // only open a new file if it doesn't exist
      dataFile = SD.open(filename, FILE_WRITE); 
      break;  // leave the loop!
    }
  }
  
  if (! dataFile) {
    error("couldnt create file");
  }

  
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
  String begin();
  String createString();              //for logging to datalogger
  
};//END SENSOR CLASS

//DEFAULT CONSTRUCTOR
Sensor::Sensor() {
  pin = 13;
  resolution = 10000;
}//END CONSTRUCTOR

//CORRECT CONSTRUCTOR
Sensor::Sensor (byte p, unsigned int r) {
  pin = p;              //pin number 
  resolution = r;       //SENSOR_RESOLUTION of data
  struct dataPoint;
}//END CONSTRUCTOR


//FUNCTION getData()
//count and return number of times sensor is triggered while time passed is less than SENSOR_SENSOR_SENSOR_SENSOR_SENSOR_SENSOR_SENSOR_RESOLUTION
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
//function to clear array
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
  arrayPosition = 0;
  for (int i = 0; i < DP_SIZE; i++) {
    Serial.print("VALUE = ");
  Serial.println(printValue(i));
  Serial.print("TIME STAMP = ");
  Serial.println(printTimeStamp(i));
  Serial.println("");
  }
}
String Sensor::createString(){
 String event;
 for (int i = 0; i < DP_SIZE; i++) {
   event += "Value = ";
   event += printValue(i);
   event += "Time Stamp = ";
   event += printTimeStamp(i);
 }
 return event;
}

String Sensor::begin(){
  
  while(arrayPosition < DP_SIZE){
    int data = getData();
    addDataPoint(data);
  }
  //once array is full do the following:
  printOutput();
  return createString();
}

void print2() {
  // make a string for assembling the data to log:
  String dataString = "";
  //create instance of anemometer class
  Sensor anemometer (HALL_EFFECT,SENSOR_RESOLUTION);
  dataString = anemometer.begin();
  anemometer.clearData();
  dataFile.println(dataString);
  Serial.println(dataString);
  dataFile.flush();
}
 
void blink(){
  digitalWrite(LED, HIGH); 
  delay(10);
  digitalWrite(LED,LOW);
}
