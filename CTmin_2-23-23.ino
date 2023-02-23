#include <OneWire.h>
#include <DallasTemperature.h>
#define ONE_WIRE_BUS A0 
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

//Erik! These are the numbers you'll be changing (Scroll down to change "fish #")
float TempSet = -0.3; //This is what temp the heater will turn off/on at
const int numReadings = 40;



// Smoothing and Temperature
double Celsius = 0;
float Fahrenheit = 0;

double readings[numReadings];
int index = 0;
float total = 0;
float average = 0;
float Newpoint = 0;
float Oldpoint = 0;
float Oldtime = 0;
float Newtime = 0;
float Timeinterval = 0;
float delayT = 0;
double rate;
const unsigned long eventinterval = 1000;
unsigned long previousTime = 0;

//Turn on Heater
int LEDpin = 8;
const int SetTemp = TempSet-.1;

//Cool Ass Button
int Buttonpin = A3;
int triggerpin = 12;
int Buttonread;
int fishcounter = 1;

void setup() {
Serial.begin(9600);
pinMode(Buttonpin,INPUT);
pinMode(triggerpin,OUTPUT);
pinMode(LEDpin, OUTPUT);
digitalWrite(triggerpin, 200);
 for(int thisReading = 0; thisReading <numReadings; thisReading++) 
    readings[thisReading] = 0;
}
void loop() {
  

Buttonread= analogRead(Buttonpin);
if (Buttonread > 2) {
  Serial.print("CTmin Reached");
  Serial.print(",");
  Serial.print("Organism #"); // <------------------------------------- Erik look here!!!!!! (Change the text in the parenthesis to have it print that on button press)
  Serial.print(fishcounter);
  Serial.print(",");
  Serial.print(Celsius);
  Serial.print(",");
  Serial.println(millis()/1000);
  fishcounter += 1;
  delay(1100);

}


unsigned long currentTime = millis();

if (currentTime - previousTime >= eventinterval)
{

//Temperature Set

  Oldtime = millis();
  sensors.requestTemperatures();
  Celsius = sensors.getTempCByIndex(0);
  delay(500);
  //Fahrenheit = sensors.toFahrenheit(Celsius);
  Newtime = millis();
 previousTime = currentTime;

//Smoothing loop
total= total - readings[index];
readings[index] = Celsius;
total= total + readings[index];
index = index +1;
if(index >= numReadings) index = 0;

//Rate Calculation
Oldpoint = average;
average = total/numReadings; 
Newpoint = average;    
Timeinterval = (Newtime-Oldtime)/1000;
rate = ((Newpoint-Oldpoint)/Timeinterval)*60;
if(rate < 30) {
    Serial.print(rate,4);
  Serial.print(",");
  Serial.print(millis()/1000);
  Serial.print(",");
  Serial.println(Celsius,4);
}
 else {Serial.print("Calibrating, Ready in ");
 Serial.print((numReadings + 2)-(millis()/1000));
 Serial.println(" seconds");
}
  

if(rate >= SetTemp) {
  analogWrite(LEDpin, 200);
}
 else {digitalWrite(LEDpin, LOW);
}

}

}
