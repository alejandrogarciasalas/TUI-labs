int fsrPin = 0;
int flexPin = 1;
 
void setup(void) {
  Serial.begin(9600);   
}
 
void loop(void) {
  int fsrReading = analogRead(fsrPin); 
  int flexReading = analogRead(flexPin);
  Serial.print(fsrReading);
  Serial.print(',');
  Serial.println(flexReading);
  delay(1000);
} 

