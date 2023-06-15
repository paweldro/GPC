#include <Wire.h>
#include <OneWire.h> 
#include <DallasTemperature.h>
#include <LiquidCrystal.h>
#include <elapsedMillis.h>
#include "konfig.h"
#include "matrix.h"


//obsługa karty SD
#include <SD.h>
#include <SPI.h>

File myFile;

const int chipSelect = BUILTIN_SDCARD;



/********************************************************************/
// Data wire is plugged into pin 8 on the Arduino 
#define ONE_WIRE_BUS 8 
const int rs = 12, en = 11, d4 = 2, d5 = 3, d6 = 4, d7 = 5;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
/********************************************************************/
// Setup a oneWire instance to communicate with any OneWire devices  
OneWire oneWire(ONE_WIRE_BUS); 
/********************************************************************/
// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);
/********************************************************************/ 
float pom;
float start_pom;
int odczyt;
float zadana;
int odliczanie;
bool zmiana_zadania;
bool pierwszy;

int obliczenia;
float procent; //Obie rzeczy do PWM



int steps = 0;
//Deklaracja macierzy
Matrix CA(HY, HY);
Matrix CB(HY, HU);
Matrix HA(HY, NA);
Matrix HB(HY, NB);

Matrix H(HY, HU);
Matrix P1(HY, NA);
Matrix P2(HY,NB);

Matrix W(HU, HU);
Matrix I(HU, HU);
Matrix jed(1 ,HU);
Matrix pomocnicza(1, HU);

Matrix YR(HY, 1);  //zmiana wartości od potencjomnetru 
Matrix Ywlewo(NA, 1);
Matrix deltaUwlewo(NB, 1);
Matrix V(HU, 1);

//delta u przewidujace
Matrix deltaUwprawo(HU, 1);

Matrix deltaUk(1, 1);
Matrix deltaUkpoprz(1, 1);

Matrix UK(1, 1);
Matrix UKminusjeden(1, 1);

//wartość zadana
int wz = 20;

//chwilowe wyjscie
Matrix wyjscie(1, 1);

Matrix macierzdoprzesuwania(HU, HU);
void setup() {
    /* serial to display data */
   Serial.begin(9600);
   while(!Serial) {}
   pinMode(LED_BUILTIN, OUTPUT);
   pinMode(A0,INPUT);
   pinMode(1,INPUT);
   pinMode(0,OUTPUT);  
   lcd.begin(16,2);
   lcd.print("Projekt Grupowy");
   lcd.setCursor(0,1);
   lcd.print("8@KSA'2023");
   zmiana_zadania = false;
   pierwszy = true;
   odliczanie = 0;
 
   obliczenia = 0;
   analogWrite(0,obliczenia); //PWM
 
    
 // Start up the library 
 sensors.begin();
 sensors.requestTemperatures();



//start_pom = 23;
 //wyjscie[0][0] = 23;


  start_pom = sensors.getTempCByIndex(0);
  wyjscie[0][0] = sensors.getTempCByIndex(0);



  //przesuwanie macierzy Ywlewo
    for(int i = NA-1; i>-1; i-- )
    { 
      
      Ywlewo[i][0]=wyjscie[0][0]- start_pom;
      }
      
  //  Serial.print("Ywlewo: ");
   // Ywlewo.vPrintFull();   

    
    //inicjalizacja karty SD
    Serial.print("Inicjalizacja karty SD: ");

    if (!SD.begin(chipSelect)) {
      Serial.println("Nieudana");
      return;
    }
    Serial.println("Udana.");


    
    //Przypisanie wartości do macierzy CA z karty SD

    readmatrix("CA.txt", CA, HY, HY);
    
    //Przypisanie wartości do macierzy CB z karty SD
    
    readmatrix("CB.txt", CB, HY, HU);
    
    //Przypisanie wartości do macierzy HA z karty SD
    
    readmatrix("HA.txt", HA, HY, NA);

    //Przypisanie wartości do macierzy HB z karty SD
    
    readmatrix("HB.txt", HB, HY, NB);

    //Przypisanie wartości do macierzy I z karty SD
    
    readmatrix("I.txt", I, HU, HU);

    //Przypisanie wartości do macierzy I z karty SD
    
    readmatrix("jed.txt", jed, 1, HU);

    //Obliczanie macierzy H, P1, P2
    
    H = CA.Invers() * CB;
    P1 = -1* CA.Invers() * HA;
    P2 =  CA.Invers() * HB;


   // Serial.print("H: ");
   // H.vPrintFull();
    
   // Serial.print("P1: ");
   // P1.vPrintFull();

   // Serial.print("P2: ");
   // P2.vPrintFull();   
    
    
    float p = 5;
    W = 2*(H.Transpose()*H+p*I);
    pomocnicza = -1*jed*W.Invers();

    //Serial.print("W: ");
   // W.vPrintFull();   

    //Serial.print("pomocnicza: ");
    //pomocnicza.vPrintFull();   
    
    //deltaUwprawo[0][0]= deltaUk[0][0];
    
    //generowanie macierzy do przesuwania
    for(int i = 0; i<HU-1; i++ ){
      macierzdoprzesuwania[i][i+1]=1;
      
    }

    
    pom = sensors.getTempCByIndex(0);
    Serial.print("Temperatura początkowa: ");
    Serial.println(pom);
    Serial.print("Pomiar:   ");
    Serial.print("PWM:  ");
    Serial.print("delta uk:  ");
    Serial.println("uk:  ");
    
    UK[0][0] = 0;
    deltaUkpoprz[0][0] = 0;
   
    
   }
  

void loop() {
  //loop


pom = sensors.getTempCByIndex(0);

sensors.requestTemperatures(); // Send the command to get temperature readings 
 while(pierwszy){
     odczyt = analogRead (A0);
      //zadana = (((odczyt-27.00)/1000.00)*(65.00-start_pom))+start_pom;
      zadana = 47.7;
      if(zadana<start_pom){
        //zadana = start_pom;
        zadana = 47.7;
      }
     odliczanie = odliczanie+1;
     lcd.clear();
     lcd.setCursor(0,0); 
     lcd.print("Zadawana T= ");
     for(int i = 0; i<HU; i++ ){
      YR[i][0]=zadana - start_pom;
      
    }
     lcd.print(zadana);
     lcd.setCursor(0,1);
     lcd.print(10-odliczanie);
     if(odliczanie==10){
      pierwszy = false;
      }
      delay(1000);
 }


 if(digitalRead(1) == HIGH){
    while(digitalRead(1) == HIGH){
      odczyt = analogRead (A0);
      //zadana = (((odczyt-27.00)/1000.00)*(65.00-start_pom))+start_pom;
      zadana = 47.7;
      if(zadana<start_pom){
        //zadana = start_pom;
        zadana = 47.7;
      }
      lcd.clear(); 
      lcd.print("Zadawana T= ");
      lcd.print(zadana);

          //generowanie macierzy YR
       for(int i = 0; i<HU; i++ ){
      YR[i][0]=zadana - start_pom;
      
    }
      delay(1000);
      }
  }

 
 
 lcd.clear(); 
 lcd.setCursor(0,0);
 lcd.print("Zadana T= ");
 lcd.print(zadana);
 lcd.print("C");
 lcd.setCursor(0,1);
 lcd.print("Obecna T= ");
 lcd.print(sensors.getTempCByIndex(0));
 lcd.print("C");

  
    
  
  V = -2*H.Transpose()*(YR-P1*Ywlewo-P2*deltaUwlewo); //obliczanie wartosci V
  deltaUk = pomocnicza*V; //obliczanie wartosci delta Uk

   deltaUwprawo[0][0]= deltaUk[0][0]; //zweryfikowc bo wpisujemy tylko pierwsza wartosc, reszta to zera
  //deltaUwprawo.vPrintFull();
 
  

//  Serial.print("V: ");
 //  V.vPrintFull();   

    
      
  wyjscie[0][0] = sensors.getTempCByIndex(0);
  wyjscie[0][0]= wyjscie[0][0] - start_pom;
 
  //wyjscie=H*deltaUwprawo+P1*Ywlewo+P2*deltaUwlewo; //obliczanie chwilowego wyjscia

  //przesuwanie macierzy Ywlewo
    for(int i = NA-1; i>0; i-- )
    { 
      Ywlewo[i][0]=Ywlewo[i-1][0];
      }
       Ywlewo[0][0]=wyjscie[0][0];

   // Serial.print("Ywlewo: ");
   //Ywlewo.vPrintFull();  
   
   //wyliczanie delta u w lewo deltaUwlewo trzeba dopasować do transmitancji 
    deltaUwlewo[0][0] =deltaUk[0][0];
   


    deltaUkpoprz[0][0] = UK[0][0];
    UK[0][0]= UK[0][0]+deltaUk[0][0];
    
    
    procent = UK[0][0];
   
    if(procent<0)
    {
      procent = 0;
      UK[0][0] = 0;
      deltaUk[0][0] =UK[0][0]- deltaUkpoprz[0][0];
      
    }
    if(procent>100){
      procent = 100;
      UK[0][0] = 100;
      deltaUk[0][0] =UK[0][0]- deltaUkpoprz[0][0];
      
    }
//obliczenia = (sqrt(UK[0][0]*10*22/100)/15)*255; // z % na syg 0-255  // alternatywne wyliczanie pwm 
obliczenia = (sqrt(UK[0][0])*25.5); // z % na syg 0-255
   Serial.print(wyjscie[0][0]);
   Serial.print("     ");
    Serial.print(obliczenia);
    Serial.print("      ");
    Serial.print(deltaUk[0][0]);
    Serial.print("      ");
    Serial.println(UK[0][0]);
 
 //Serial.print(obliczenia);
 
 
 
 
 //Serial.print(obliczenia);
 analogWrite(0,obliczenia); // wyjscie
 
   //delay(100); 
  delay(60000);


    
}


void readmatrix(char const *filename, Matrix &matrix, int x, int y)
{
     myFile = SD.open(filename);

    if (myFile) 
  {
    while (myFile.available())
    {  
      for(int i=0;i<x;i++)
      {
        
        for(int j=0;j<y;j++)
        {
          float myFloat = myFile.parseFloat();
          matrix[i][j]=myFloat;
        }
      }
      myFile.close();  
    }
  }  
  else {
   Serial.println("Nie można otworzyć pliku: " + String(filename));
   
  }       
 
}



void SPEW_THE_ERROR(char const * str)
{
  //Funkcja potrzebna do działania biblioteki mpc.h/matrix.h
  
    #if (SYSTEM_IMPLEMENTATION == SYSTEM_IMPLEMENTATION_PC)
        cout << (str) << endl;
    #elif (SYSTEM_IMPLEMENTATION == SYSTEM_IMPLEMENTATION_EMBEDDED_ARDUINO)
        //Serial.println(str);
    #else
        /* Silent function */
    #endif
    while(1);
}
