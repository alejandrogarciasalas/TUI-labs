char serInString[100];
char response;

int redPin = 9;
int greenPin = 10;
int bluePin = 11;

int wait = 20;
char currStatus = 's';

int redVal = 255;
int greenVal = 1;
int blueVal = 1;

int i = 0;

void setup() {
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  
  Serial.begin(9600);
  Serial.println("Are you scared?");
}

void loop() {
  // ** READ SERIAL **
  
  memset(serInString, 0, 100); // clear string (char array)
  readSerialString(serInString);
  response = serInString[0];
  
  // Just check the first letter of char array 
  // Hacky but works for this specific case, I didn't want to use a C function nor iterate over the char array
  if (response == 'n') {   
    Serial.println("no");
    Serial.println("BOO!");
    Serial.println("what about now?");
    
    currStatus = 'n';
    wait = 1;
    
  } else if (response == 'y') { 
    Serial.println("yes");
    Serial.println("calm down...");
    Serial.println("what about now?");
    
    currStatus = 'y';
    wait = 10;
    blueVal = 255;
  } 
  
  // ** FADE ACCORDING TO STATUS **
  i += 1;
  if (currStatus == 's') { // normal loop
      if (i < 255) {
        redVal   -= 1;
        greenVal += 1;
        blueVal   = 1;
      } else if (i < 509) {
        redVal    = 1; 
        greenVal -= 1; 
        blueVal  += 1;
      } else if (i < 763) {
        redVal  += 1; 
        greenVal = 1; 
        blueVal -= 1; 
      } else {
        i = 1;
      }    
  } else if (currStatus == 'n') { // blue and red
    greenVal = 1;
    if (i < 255) {
      redVal   -= 1;
      blueVal += 1;
    } else if (i < 509) {
      redVal += 1;
      blueVal -= 1;
    } else {
      i = 1;
    }
  } else if (currStatus == 'y') { // blue and green
    redVal = 1;
    if (i < 255) {
      blueVal -= 1;
      greenVal += 1;
    } else if (i < 509) {
      blueVal += 1;
      greenVal -= 1;
    } else {
      i = 1;
    }
  }

  analogWrite(redPin, redVal);
  analogWrite(greenPin, greenVal);
  analogWrite(bluePin, blueVal);
  
  delay(wait);
}

void readSerialString (char *strArray) {
  int i = 0;
  if(!Serial.available()) {
    return;
  }
  while (Serial.available()) {
    strArray[i] = Serial.read();
    i++;
  }
}

