float temp;
int tempPin = A0;
int ledPin = 13;
void setup()
{
  Serial.begin(9600);
  pinMode(tempPin, INPUT);
  pinMode(13, OUTPUT);  
}

void loop()
{
 temp = analogRead(tempPin);
  temp = temp * 0.48828125;
  Serial.print("TEMPRATURE = ");
  Serial.print(temp);
  Serial.print("*C");
  Serial.println();
  delay(1000);
}
