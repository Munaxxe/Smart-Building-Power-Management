#include <SoftwareSerial.h>
#include <LiquidCrystal.h> //library for LCD 

// include Wire library code (needed for I2C protocol devices)
#include<Wire.h>

//Constants declaration and initialisation
const int rs = 13, en = 12;

// initializing the library with the numbers of the interface pins 
LiquidCrystal lcd(rs, en, 11, 10, 9, 8);
 
//GSM Module initialisation
SoftwareSerial SIM900(1, 6); //TX RX


//Variable declarations
int offsetvoltage = 2542; 
int airCon = 7;
int light = 4;
int pir = 3;
int ldr = 2;
int motion, lightInt, greenStatus, buzzerAndRedStatus, lightStatus, airConStatus, lightIntStatus, tempV, t;
int buzzerAndRed = A3;
int green = A2;
float vCheck;
int temp = A1;
int hotair = A0;
int coldair = 5;

void setup()
{
 Serial.begin(9600);
 SIM900.begin(9600);
 //Serial Monitor Title

Serial.println("|SMART BUILDING|");  

pinMode(light, OUTPUT);
pinMode(green, OUTPUT);
pinMode(buzzerAndRed, OUTPUT);
pinMode(pir, INPUT);
pinMode(airCon, OUTPUT);
pinMode(ldr, INPUT);
pinMode(temp, INPUT);
pinMode(hotair, OUTPUT);
pinMode(coldair, OUTPUT);

lcd.begin(20, 4); // settibg up the LCD's number of columns and rows:
lcd.setCursor(0,0);
lcd.print("\t\t\t\tC19137614S");
lcd.setCursor(0,1);
lcd.print("\t\tSMART BUILDING");

Wire.begin();

analogWrite(coldair, 0);
digitalWrite(hotair, 0);
}

void loop()
{
//Measuring Temperature
lcd.setCursor(0,2);
lcd.print("T = ");
lcd.print(tempV);
lcd.print("-C ");

//Light Status
lcd.print("Light = ");
if(lightInt != 0){
  lcd.print("ON ");
  }
else if(lightInt == 0){
  lcd.print("OFF");
  }

//PIR Status
lcd.setCursor(0,3);
lcd.print("R/Occupancy = ");

if(motion != 0){
  lcd.print("ON ");
  }
else if(motion == 0){
  lcd.print("OFF");
  }

tempV = analogRead(temp);
tempV = (tempV*500)/1024;

motion = digitalRead(pir);
lightInt = digitalRead(ldr);
greenStatus = analogRead(green);
buzzerAndRedStatus = analogRead(buzzerAndRed);
airConStatus = digitalRead(airCon);
lightStatus = digitalRead(light);

Serial.print("Temperature: ");
Serial.println(tempV);

conditionCheck();

}
 
void conditionCheck()
{
//Motion during the day
if(lightInt == 0 && motion == 1 )
{
analogWrite(buzzerAndRed, 0);
analogWrite(green, 255);
digitalWrite(airCon, 1);
digitalWrite(light, 0);

tempR();

Serial.println("DAY with Occupant");

SIM900.print("AT+CMGF=1\r"); 
SIM900.println("AT + CMGS = \"+263784600988\"");// recipient's mobile number
Serial.println("AT + CMGS = \"+263784600988\"");// recipient's mobile number
SIM900.println("HIGH LIGHT INTENSITY- LIGHTS ARE OFF, MOTION DETECTED"); // message to send
Serial.println("AIRCON IS ON");
Serial.println("MOTION DETECTED");
Serial.println("LIGHT IS OFF");
SIM900.println((char)26); // End of AT command 
Serial.println((char)26);
SIM900.println(); 
}

//Motion during the night
if(lightInt == 1 && motion == 1)
{
analogWrite(buzzerAndRed, 0);
analogWrite(green, 255);
digitalWrite(airCon, 1);
digitalWrite(light, 1);

tempR();

Serial.println("NIGHT with Occupant");

SIM900.print("AT+CMGF=1\r"); 
SIM900.println("AT + CMGS = \"+263784600988\"");// recipient's mobile number
Serial.println("AT + CMGS = \"+263784600988\"");// recipient's mobile number
SIM900.println("LOW LIGHT INTENSITY- LIGHTS ARE ON, AIRCON IS ON, MOTION DETECTED"); // message to send
Serial.println("AIRCON IS ON");
Serial.println("MOTION DETECTED");
Serial.println("LIGHT IS ON");
SIM900.println((char)26); // End pf AT command
Serial.println((char)26);
SIM900.println(); 
}

// NO motion during the day 
if(motion == 0 && lightInt == 0)
{
analogWrite(buzzerAndRed, 0);
analogWrite(green, 255);
digitalWrite(airCon, 0);
digitalWrite(light, 0);

Serial.println("DAY With No Occupant");

SIM900.print("AT+CMGF=1\r"); 
SIM900.println("AT + CMGS = \"+263784600988\"");// recipient's mobile number
Serial.println("AT + CMGS = \"+263784600988\"");// recipient's mobile number
SIM900.println("HIGH LIGHT INTENSITY- LIGHTS ARE OFF, AIRCON IS OFF, NO MOTION DETECTED"); // message to send
Serial.println("AIRCON IS OFF");
Serial.println("NO MOTION DETECTED");
Serial.println("LIGHT IS OFF");
SIM900.println((char)26); // End of AT command
Serial.println((char)26);
SIM900.println(); 
}

//No motion during the night
if(motion == 0 && lightInt == 1)
{
analogWrite(buzzerAndRed, 0);
analogWrite(green, 255);
digitalWrite(airCon, 0);
digitalWrite(light, 0);

Serial.println("NIGHT With No Occupant");

SIM900.print("AT+CMGF=1\r"); 
SIM900.println("AT + CMGS = \"+263784600988\"");// recipient's mobile number
Serial.println("AT + CMGS = \"+263784600988\"");// recipient's mobile number
SIM900.println("HIGH LIGHT INTENSITY- LIGHTS ARE OFF, AIRCON IS OFF, NO MOTION DETECTED"); // message to send
Serial.println("AIRCON IS OFF");
Serial.println("NO MOTION DETECTED");
Serial.println("LIGHT IS OFF");
SIM900.println((char)26); // End of AT command
Serial.println((char)26);
SIM900.println(); 
}

//Fault detection
if(vCheck >= 300)
{
 
digitalWrite(green, 0);
digitalWrite(airCon,0);
digitalWrite(light, 0);
digitalWrite(buzzerAndRed, 1);

SIM900.print("AT+CMGF=1\r"); 
SIM900.println("AT + CMGS = \"+263784600988\"");// recipient's mobile number
Serial.println("AT + CMGS = \"+263784600988\"");// recipient's mobile number
SIM900.println("FAULT DETECTED"); // message to send
Serial.println("FAULT DETECTED");
SIM900.println((char)26); // End /of AT command
Serial.println((char)26);
SIM900.println(); 
}
}
//End Of Main Loop

//Temperature Sensor conditions
void tempR(){
if(tempV >= 25 && motion == 1){
  digitalWrite(coldair, 1);
  analogWrite(hotair, 0);
  }
  else if(tempV < 25 && motion == 1){
    analogWrite(hotair, 255);
    digitalWrite(coldair, 0);
    }
  }
