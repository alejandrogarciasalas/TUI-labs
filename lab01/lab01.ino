const int ledPin13 =  13; // green
const int ledPin12 =  12; // blue 
const int ledPin11 =  11; // red

void setup() {
  pinMode(ledPin11, OUTPUT);
  pinMode(ledPin12, OUTPUT);
  pinMode(ledPin13, OUTPUT);
}

void loop()
{
  digitalWrite(ledPin13, HIGH); 
  delay(80);
  digitalWrite(ledPin13, LOW);
  digitalWrite(ledPin12, HIGH);
  delay(80);
  digitalWrite(ledPin12, LOW);
  digitalWrite(ledPin11, HIGH);
  delay(80);
  digitalWrite(ledPin11, LOW);
  delay(500); 
}

