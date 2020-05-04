#include <Servo.h>
#include<SoftwareSerial.h>
//SoftwareSerial gsm(A1,A2);
//tells the arduino what each pin use is connected to
#define trigPin 5
#define echoPin 6
#define ledPin  4
#define speakerPin 12
#define pirPin  2
#define pumpPin  3
#define tempPin  A0
float tempC;
float temp;
int pirState = LOW;
int val = 0;
char phone_no[]="8281183856";

int motorPin = 8; //right side to IB - forward

int motorPin2 = 9; //left side to IA - forward

int motorPin3 = 10; //right side to IA - backward

int motorPin4 = 11; //left side to IB - backward

Servo servoMain; // Define our Servo


void setup() //Sets each defined pin as an input or 
                //output so that the arduino what to expect at each pin
{//gsm.begin(9600);   // Setting the baud rate of GSM Module 
  Serial.begin(9600); // set up Serial library at 9600 bps
   
    servoMain.attach(7); // servo on digital pin 10
 
  pinMode(trigPin, OUTPUT); //Unltrasonic output
  pinMode(echoPin, INPUT); //Ultrasonic input
  pinMode(tempPin,INPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(pirPin, INPUT);
  pinMode(speakerPin, OUTPUT);
  pinMode(pumpPin, OUTPUT);
  pinMode(tempPin, INPUT);
  
  pinMode(motorPin, OUTPUT);
  pinMode(motorPin2, OUTPUT);
  pinMode(motorPin3, OUTPUT);
  pinMode(motorPin4, OUTPUT);
 }

//Ping Function to get the distance from the HC-SR04
unsigned long ping()
{
   // Trigger the uSonic sensor (HC-SR04) to send out a ping
   long duration, distance; //Initial Distance measure
   digitalWrite(trigPin, LOW);  // Added this line
     delayMicroseconds(1); // Added this line
   digitalWrite(trigPin, HIGH); //  delayMicroseconds(1000); - Removed this line
     delayMicroseconds(1); // Added this line
   digitalWrite(trigPin, LOW);
   duration = pulseIn(echoPin, HIGH);
   distance = (duration/2) / 29.1;
   delay(100);
   //clearlcd();
   return (distance);
}


void send_sms()
{
  Serial.begin(9600);
  Serial.println("AT+CMGF=1");//set GSM to text mode
  delay(200);
Serial.print("AT+CMGS=\"");
    Serial.print(phone_no);
    Serial.println("\"");
    delay(500);
   
       Serial.println("Human presence detected");
    Serial.print("Temperature: ");
    Serial.print(tempC);
    Serial.println ("*C");
    Serial.println("send evacuation immediately");

      Serial.write(0x1A);//sends ctrl+z  for end of the message
       Serial.write(0x0D);//carrige return in hex
        Serial.write(0x0A);//line feed in hex (The 0D0A of characters is the signal for the end of the line
      delay(100); 

 /*    Serial.println("AT+CMGF=1\r\n");      //set GSM to text mode
  delay(200);
Serial.print("AT+CMGS=\"");
    Serial.print(phone_no);
    Serial.println("\"\r\n");
    delay(500);
    Serial.println("Human presence detected");
    Serial.println("Temperature: ");
    Serial.print(temp);
    Serial.println("send evacuation immediately");
    Serial.write(0x1A);
    Serial.println((char)26);
    delay(200);
    */
}


void led_blink()        //human presence indication
 {
   digitalWrite(ledPin, HIGH);
   delay(2000);
   digitalWrite(ledPin, LOW);
   delay(200);
   digitalWrite(speakerPin, HIGH);
     delay(2000);
     digitalWrite(speakerPin, LOW);
   digitalWrite(ledPin, HIGH);
   delay(500);
   digitalWrite(ledPin, LOW);
   delay(200);
   digitalWrite(ledPin, HIGH);
   delay(500);
   digitalWrite(ledPin, LOW);
   delay(200);
 }
 
 
 
//Forward Fuction sets motors to go forward
void forward()
{
      //Serial.println("Forward");    
digitalWrite(motorPin, HIGH);

digitalWrite(motorPin2, LOW);      //foreward

digitalWrite(motorPin3, HIGH);  

digitalWrite(motorPin4, LOW);
delay(10);              
}

void halt(){ 

digitalWrite(motorPin, LOW);

digitalWrite(motorPin2, LOW);

digitalWrite(motorPin3, LOW);  

digitalWrite(motorPin4, LOW);
delay(2000);
 }


void backward() {  

    digitalWrite(motorPin, LOW);  

    digitalWrite(motorPin2, HIGH);  

    digitalWrite(motorPin3, LOW);  

    digitalWrite(motorPin4, HIGH);

} 

//Right Function sets motors for right
void right()
{
      //Serial.println("right");    
     digitalWrite(motorPin, LOW);  

      digitalWrite(motorPin2, HIGH);          //right

      digitalWrite(motorPin3, HIGH);  

      digitalWrite(motorPin4, LOW);
      delay(3000);
   
}

// Left Function sets motors for left
void left()
{        
      //Serial.println("left");    
      digitalWrite(motorPin, HIGH);  

      digitalWrite(motorPin2, LOW);          //left

      digitalWrite(motorPin3, LOW);  

      digitalWrite(motorPin4, HIGH);

      delay(3000);
     
}

// Back Fnction sets motors going backwards
void back()
{
      //Serial.println("back");    
    digitalWrite(motorPin, LOW);  

    digitalWrite(motorPin2, HIGH);  

    digitalWrite(motorPin3, LOW);  // back

    digitalWrite(motorPin4, HIGH); 
    delay(3000);
}

unsigned long LM35Temp() // Function to gather LM35 sensor data for temp, returns reading
{
tempC = analogRead(tempPin);
tempC = (5.0*tempC*100.0)/1024.0;
Serial.print("temp in C is :");
Serial.print((byte)tempC);
}

void loop()
{ 
  
  long int forwdis, leftdis, rightdis;
  
temp = LM35Temp(); // Runs LM35Temp function saves value in temp

  servoMain.write(90); // Centre Servo
  delay(150);
  forwdis = ping(); // Runs Ping fucntion reurns distance saves as forward distance
  Serial.println (forwdis); // Print forward distance
  
 

  if (forwdis >= 15) //If the forward dis is greater than 12 cm keep going forward
  {
    if(tempC>34)
      {
        halt();
        digitalWrite(ledPin, HIGH);
        digitalWrite(pumpPin, HIGH);
        delay(10000);
        digitalWrite(ledPin,LOW);
        digitalWrite(pumpPin,LOW);
        }
         int pirVal = digitalRead(pirPin);
   if(pirVal == HIGH)
   {
     halt();
    led_blink();
    send_sms();
     if (pirState == LOW) {
// we have just turned on
Serial.println("Motion detected!");
// We only want to print on the output change, not state
pirState = HIGH;
}
 else {
digitalWrite(ledPin, LOW); // turn LED OFF
if (pirState == HIGH){
// we have just turned of
Serial.println("Motion ended!");
// We only want to print on the output change, not state
pirState = LOW;
}
}
   }
     
   
   
  
   else if(tempC<34)
   {
     digitalWrite(ledPin, LOW);
     digitalWrite(speakerPin, LOW);
     forward();
   }
  }
  else if (forwdis < 15)  //If distance is less than 15 cm then
  {
    back();
   
    
        
    // Look left and get left distance
    servoMain.write(165); 
    delay(150);
    leftdis = ping();
    Serial.println (leftdis);
    
    // Look right and get right distance
    servoMain.write(15);
    delay(150);
    rightdis = ping();
    Serial.println (rightdis);
    
    // Reset servo centre possition
    servoMain.write(90);
    delay(150);
      
      // If Left distance is greater than right then go left
      if (leftdis > rightdis) 
      {
        Serial.print("LEFT"); // Display direction of movment
        Serial.print(leftdis); // Print distance in cm
        left();
      }

      // If right distance is greater than left distance go right
      else if (rightdis > leftdis) 
      {
        Serial.print("RIGHT"); // Display direction of movment
        Serial.print(rightdis); // Print distance in cm
        right();
      }
    
      // If each are the same then go backwards and clear LCD
      else if (rightdis == leftdis) 
      {
         back();  
      }   
  }
}
  

