class Sensor {
  public:
  unsigned int getData();   //contains a while loop to listen for sesnor data from pin
  saveData();               //store dtat point in datapoint array
  reportData();             //push dataPoint array to higher level
  clearData();              //clear dataPoint array
  byte pin;                 //the pin the sensor is plugged into, on the arduino board
  unsigned int resolution;  //the delay time between saving dataPoints
  dataPoint[];              //dataPoint array should contain all datapoints created in
  struct dataPoint {
    byte value;
    time_t timeStamp; //standard C time_t so elapsed times can be easily calculated and time values shared across different platforms.
  }
  
  private:
  unsigned long previousMillis;
}

//constructor for sensor class - mc pin number, 
Sensor::Sensor(byte tempPin, unsigned int tempResolution) {
  pin = tempPin;
  resolution = tempResolution;
}

//count and return number of times sensor is triggered in resolution
unsigned int Sensor::getData() {
  int count = 0;      //local scope variable
   while ((unsigned long)(millis() - previousMillis) <= resolution) { //while time passed is less than interval
    if (!digitalRead(pin)) {      //if sensor is triggered 
     count++;                  //add one to count
     //***must add debouncing to sensor reading  
    }//END IF
   }//END WHILE
   previousMillis = millis();      //set previous time to current time
   return count;                   //return total counts
}//end loop

Sensor::saveData() {
  
  
}


  
  
  

