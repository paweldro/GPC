#include <Wire.h>
#include <elapsedMillis.h>
#include "konfig.h"
#include "matrix.h"
#include "mpc.h"

//obsługa karty SD
#include <SD.h>
#include <SPI.h>

File myFile;

const int chipSelect = BUILTIN_SDCARD;


//Deklaracja macierzy
Matrix CA(HY, HY);
Matrix CB(HY, HU);
Matrix HA(HY, NA);
Matrix HB(HY, NB);

Matrix H(HY, HU);
Matrix P1(HY, NA);
Matrix P2(HY,NB);


void setup() {
    /* serial to display data */
    Serial.begin(115200);
    while(!Serial) {}
    pinMode(LED_BUILTIN, OUTPUT);
    
    //inicjalizacja karty SD
    Serial.print("Inicjalizacja karty SD: ");

    if (!SD.begin(chipSelect)) {
      Serial.println("Nieudana");
      return;
    }
    Serial.println("Udana.");


    
    //Przypisanie wartości do macierzy CA

    myFile = SD.open("CA.txt");

    if (myFile) 
  {
    while (myFile.available())
    {  
      for(int i=0;i<HY;i++)
      {
        
        for(int j=0;j<HY;j++)
        {
          float myFloat = myFile.parseFloat();
          CA[i][j]=myFloat;
        }
      }
      myFile.close();  
    }
  }  
  else {
    Serial.println("Cannot open file!");
  } 

    //Przypisanie wartości do macierzy CB
    
    myFile = SD.open("CB.txt");

    if (myFile) 
  {
    while (myFile.available())
    {  
      for(int i=0;i<HY;i++)
      {
        
        for(int j=0;j<HU;j++)
        {
          float myFloat = myFile.parseFloat();
          CB[i][j]=myFloat;
        }
      }
      myFile.close();  
    }
  }  
  else {
    Serial.println("Cannot open file!");
  }    

    //Przypisanie wartości do macierzy HA z karty SD
    
    myFile = SD.open("HA.txt");

    if (myFile) 
  {
    while (myFile.available())
    {  
      for(int i=0;i<HY;i++)
      {
        
        for(int j=0;j<NA;j++)
        {
          float myFloat = myFile.parseFloat();
          HA[i][j]=myFloat;
        }
      }
      myFile.close();  
    }
  }  
  else {
    Serial.println("Cannot open file!");
  } 

    //Przypisanie wartości do macierzy HB
    
    myFile = SD.open("HB.txt");

    if (myFile) 
  {
    while (myFile.available())
    {  
      for(int i=0;i<HY;i++)
      {
        
        for(int j=0;j<NB;j++)
        {
          float myFloat = myFile.parseFloat();
          HB[i][j]=myFloat;
        }
      }
      myFile.close();  
    }
  }  
  else {
    Serial.println("Cannot open file!");
  }       

    //Obliczanie macierzy H, P1, P2
    
    H = CA.Invers() * CB;
    P1 = -1* CA.Invers() * HA;
    P2 =  CA.Invers() * HB;
    
    //Wyświetlanie macierzy P2
    
    P2.vPrintFull();
    
}


void loop() {
  //loop
   
   

}





void SPEW_THE_ERROR(char const * str)
{
  //Funkcja potrzebna do działania biblioteki mpc.h
  
    #if (SYSTEM_IMPLEMENTATION == SYSTEM_IMPLEMENTATION_PC)
        cout << (str) << endl;
    #elif (SYSTEM_IMPLEMENTATION == SYSTEM_IMPLEMENTATION_EMBEDDED_ARDUINO)
        Serial.println(str);
    #else
        /* Silent function */
    #endif
    while(1);
}
