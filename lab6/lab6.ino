// ********** Accelerometer Stuff **********
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_LIS3DH.h>
#include <Adafruit_Sensor.h>

// Used for software SPI
#define LIS3DH_CLK 13
#define LIS3DH_MISO 12
#define LIS3DH_MOSI 11
// Used for hardware & software SPI
#define LIS3DH_CS 10

// software SPI
Adafruit_LIS3DH lis = Adafruit_LIS3DH(LIS3DH_CS, LIS3DH_MOSI, LIS3DH_MISO, LIS3DH_CLK);
// hardware SPI
//Adafruit_LIS3DH lis = Adafruit_LIS3DH(LIS3DH_CS);
// I2C
//Adafruit_LIS3DH lis = Adafruit_LIS3DH();

#if defined(ARDUINO_ARCH_SAMD)
// for Zero, output on USB Serial console, remove line below if using programming port to program the Zero!
   #define Serial SerialUSB
#endif

// ********** Piezo Buzzer stuff **********

#include "pitches.h"

// notes in the melody:
int melody[] = {
  NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_G3, 0, NOTE_B3, NOTE_C4
};

// note durations: 4 = quarter note, 8 = eighth note, etc.:
int noteDurations[] = {
  4, 8, 8, 4, 4, 4, 4, 4
};


int piezoPin1 = 8;
int piezoPin2 = 7;
int piezoPin3 = 6;

void toneMultiplePiezos(int note, int noteDuration) {
  tone(piezoPin1, note, noteDuration);
  tone(piezoPin2, note, noteDuration);
  tone(piezoPin3, note, noteDuration);
}

void noToneMultiplePiezos() {
  noTone(piezoPin1);
  noTone(piezoPin2);
  noTone(piezoPin3);
}

#include "elapsedMillis.h"
elapsedMillis timeElapsed;
float initialVelocity = 0.0;
float currentVelocity = 0.0;
int timeThreshold = 0.0;

void setup() {
  // **********
    
  // iterate over the notes of the melody:
  for (int thisNote = 0; thisNote < 8; thisNote++) {
    // to calculate the note duration, take one second divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int noteDuration = 1000 / noteDurations[thisNote];
    toneMultiplePiezos(melody[thisNote], noteDuration);
    // to distinguish the notes, set a minimum time between them.
    // the note's duration + 30% seems to work well:
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    // stop the tone playing:
    noToneMultiplePiezos();
  }

  #ifndef ESP8266
    while (!Serial);     // will pause Zero, Leonardo, etc until serial console opens
  #endif
  Serial.begin(9600);
  Serial.println("LIS3DH test!");
  if (! lis.begin(0x18)) {   // change this to 0x19 for alternative i2c address
    Serial.println("Couldnt start");
    while (1);
  }
  Serial.println("LIS3DH found!");
  lis.setRange(LIS3DH_RANGE_4_G);   // 2, 4, 8 or 16 G!
  Serial.print("Range = "); Serial.print(2 << lis.getRange());  
  Serial.println("G");    
}

void loop() {
  lis.read();      // get X Y and Z data at once
  // Then print out the raw data
  Serial.print("X:  "); Serial.print(lis.x); 
  Serial.print("  \tY:  "); Serial.print(lis.y); 
  Serial.print("  \tZ:  "); Serial.print(lis.z); 

  /* Or....get a new sensor event, normalized */ 
  sensors_event_t event; 
  lis.getEvent(&event);
  
  /* Display the results (acceleration is measured in m/s^2) */
  Serial.print("\t\tX: "); Serial.print(event.acceleration.x);
  Serial.print(" \tY: "); Serial.print(event.acceleration.y); 
  Serial.print(" \tZ: "); Serial.print(event.acceleration.z); 
  Serial.println(" m/s^2 ");

  Serial.println();
 
  delay(1);

//  toneMultiplePiezos(abs(float(event.acceleration.x)), 2000);
//  toneMultiplePiezos(abs(float(event.acceleration.y)), 2000);
//  toneMultiplePiezos(abs(float(event.acceleration.z)), 2000);
//  if (abs(event.acceleration.x) < 2) {
//    toneMultiplePiezos(NOTE_C1, 2000);  
//  } else if (abs(event.acceleration.x) < 3) {
//    toneMultiplePiezos(NOTE_D1, 2000);      
//  } else if (abs(event.acceleration.x) < 4) {
//    toneMultiplePiezos(NOTE_E1, 2000);      
//  } else if (abs(event.acceleration.x) < 5) {
//    toneMultiplePiezos(NOTE_F1, 2000);      
//  } else if (abs(event.acceleration.x) < 6) {
//    toneMultiplePiezos(NOTE_G1, 2000);      
//  } else if (abs(event.acceleration.x) < 7) {
//    toneMultiplePiezos(NOTE_A1, 2000);      
//  } else if (abs(event.acceleration.x) < 8) {
//    toneMultiplePiezos(NOTE_B1, 2000);      
//  }  
//

  int numtoruleitall = (abs(event.acceleration.x) + abs(event.acceleration.y) + abs(event.acceleration.z)) / 3;
  Serial.println(numtoruleitall);

  if (numtoruleitall <= 5) {
    numtoruleitall = 0;
  }
//  toneMultiplePiezos(abs(numtoruleitall) * 100 , 1000);

  if (timeElapsed > timeThreshold + 1) {
    timeThreshold = timeElapsed;
    currentVelocity = (initialVelocity  + (numtoruleitall * (timeElapsed/1000)));
//    initialVelocity = currentVelocity;
    Serial.println(currentVelocity);
  }  

  int roll abs(RP_calculate(event.acceleration.x, event.acceleration.y, event.acceleration.z));
  toneMultiplePiezos(NOTE_D1 * abs(currentVelocity), 1000);  
  // toneMultiplePiezos(abs(currentVelocity) * roll, 1000);  
}



float RP_calculate(float x, float y, float z){
  double x_Buff = float(x);
  double y_Buff = float(y);
  double z_Buff = float(z);
  float roll = atan2(y_Buff , z_Buff) * 57.3;
  float pitch = atan2((- x_Buff) , sqrt(y_Buff * y_Buff + z_Buff * z_Buff)) * 57.3;
  Serial.println("roll");
  Serial.println(roll);
  return roll;
}
