/********************************************************************/
// Include the libraries
#include <OneWire.h> 
#include <DallasTemperature.h>
#include <LiquidCrystal.h>
/********************************************************************/
// Data wire is plugged into pin 8 on the Arduino 
#define ONE_WIRE_BUS 8 
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
/********************************************************************/
// Setup a oneWire instance to communicate with any OneWire devices  
OneWire oneWire(ONE_WIRE_BUS); 
/********************************************************************/
// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);
/********************************************************************/ 
float start;
int odczyt;
float zadana;
void setup(void) 
{
 Serial.begin(9600);
 while(!Serial);
 pinMode(A0,INPUT); 
 lcd.begin(16,2);
 lcd.print("Pomiar");
 lcd.setCursor(0,1);
 lcd.print("Temp");
 

 // Start up the library 
 sensors.begin();
 sensors.requestTemperatures();
 start = sensors.getTempCByIndex(0);
} 
void loop(void) 
{ 
 sensors.requestTemperatures(); // Send the command to get temperature readings 
 odczyt = analogRead (A0);
 zadana = (((odczyt-35.00)/1020.00)*(125.00-start))+start;
 /*Serial.println(sensors.getTempCByIndex(0));
 lcd.clear(); 
 lcd.setCursor(0,0);
 lcd.print("Zadana T= ");
 lcd.print(zadana);
 lcd.print("C");
 lcd.setCursor(0,1);
 lcd.print("Obecna T= ");
 lcd.print(sensors.getTempCByIndex(0));
 lcd.print("C");
 */Serial.println(analogRead (A0));
   delay(1000); 
} 
