int led0r = 13;
int led0g = 12;
int led0b = 11;

int led1r = 10;
int led1g = 9;
int led1b = 8;

int led2r = 7;
int led2g = 6;
int led2b = 5;

int pot0Pin = A0;
int pot0Val = 0;

int pot1Pin = A1;
int pot1Val = 0;

int pot2Pin = A2;
int pot2Val = 0;

void setup() {
  pinMode(led0r, OUTPUT);
  pinMode(led0g, OUTPUT);
  pinMode(led0b, OUTPUT);
  pinMode(led1r, OUTPUT);
  pinMode(led1g, OUTPUT);
  pinMode(led1b, OUTPUT);
  pinMode(led2r, OUTPUT);
  pinMode(led2g, OUTPUT);
  pinMode(led2b, OUTPUT);
}

void loop() {
  pot0Val = analogRead(pot0Pin);
  pot1Val  = analogRead(pot1Pin);
  pot2Val = analogRead(pot2Pin);

  int brightness = pot0Val/4;
  int steering = pot1Val;
  int carSpeed = pot2Val;
  
  if (steering < 210) {
    if (carSpeed < 210) {
      analogWrite(led0r, brightness);
      analogWrite(led1r, 0);
      analogWrite(led2r, 0);
    } else if (carSpeed < 420) {
      analogWrite(led0g, brightness);
      analogWrite(led1g, 0);
      analogWrite(led2g, 0);      
      } else {
      analogWrite(led0b, brightness);
      analogWrite(led1b, 0);
      analogWrite(led2b, 0);        
      }
  } else if (steering < 420) {
    if (carSpeed < 210) {
      analogWrite(led0r, 0);
      analogWrite(led1r, brightness);
      analogWrite(led2r, 0);
    } else if (carSpeed < 420) {
      analogWrite(led0g, 0);
      analogWrite(led1g, brightness);
      analogWrite(led2g, 0);      
      } else {
      analogWrite(led0b, 0);
      analogWrite(led1b, brightness);
      analogWrite(led2b, 0);        
      }
  } else {
    if (carSpeed < 210) {
      analogWrite(led0r, 0);
      analogWrite(led1r, 0);
      analogWrite(led2r, brightness);
    } else if (carSpeed < 420) {
      analogWrite(led0g, 0);
      analogWrite(led1g, 0);
      analogWrite(led2g, brightness);      
      } else {
      analogWrite(led0b, 0);
      analogWrite(led1b, 0);
      analogWrite(led2b, brightness);        
      }
  }

  delay(carSpeed);

  analogWrite(led0r, 0);
  analogWrite(led1r, 0);
  analogWrite(led2r, 0);
  
  analogWrite(led0g, 0);
  analogWrite(led1g, 0);
  analogWrite(led2g, 0);

  analogWrite(led0b, 0);
  analogWrite(led1b, 0);
  analogWrite(led2b, 0);
    
  delay(carSpeed);
}

