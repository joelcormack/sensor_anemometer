/*
this sketch is a skeleton of a generic sensor sketch, at the moment it is deisgned for the aneometer.

      TO DO
      =====
* genralise sketch
* import time object
*/
//create new instance of sensor with pin number and resolution

#define LED 13            //define variable led to pin 13
#define RESOLUTION 10000   //define resolution of anemometer sensor
#define HALL_EFFECT 8     //define sensor pin
#define HALL_ON 100
//Sensor sensor(13,1000);

//STEP 1 - INITIALISATION
void setup(){
  Serial.begin(9600);        //begin serial
  pinMode(LED, OUTPUT);      //set pin mode to output
  pinMode(HALL_EFFECT,INPUT);      //set input pin
}

void loop(){
//STEP 2 - READ SENSORS
  //Serial.println("HI");
 // print();
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
  //public funtions and variables
  public:
  Sensor();
  Sensor(byte, unsigned int);
  unsigned int getData();               //contains a while loop to listen for sesnor data from pin
  void saveData();                      //store dtat point in datapoint array
  void reportData();                    //push dataPoint array to higher level
  void clearData();                     //clear dataPoint array

  byte getPin(void) {return pin;} 
  int getResolution(void) {return resolution;} 
  
  struct dataPoint {
    byte value;
    String timeStamp; //standard C time_t so elapsed times can be easily calculated and time values shared across different platforms.
  }dataPointOne;
  dataPoint dataPointArray[];              //dataPoint array should contain all datapoints created in 
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

void Sensor::saveData() {
//create time object
}
void reportData(){
}
void clearData(){
}
void print2() {
  Sensor anemometer (HALL_EFFECT,RESOLUTION);
  Serial.print(anemometer.getData());
  Serial.print(" rotations per ");
  Serial.print(RESOLUTION);
  Serial.print(" milliseconds");
  Serial.println("");
  Serial.println("");
}
 
//STEP 4 - OUTPUT
//(STEP 5 - TERMINATION)

void blink(){
  digitalWrite(LED, HIGH); 
  delay(10);
  digitalWrite(LED,LOW);
}



