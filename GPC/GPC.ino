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


    
    //Przypisanie wartości do macierzy CA z karty SD

    readmatrix("CA.txt", CA, HY, HY);
    
    //Przypisanie wartości do macierzy CB z karty SD
    
    readmatrix("CB.txt", CB, HY, HU);
    
    //Przypisanie wartości do macierzy HA z karty SD
    
    readmatrix("HA.txt", HA, HY, NA);

    //Przypisanie wartości do macierzy HB z karty SD
    
    readmatrix("HB.txt", HB, HY, NB);
        

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
        Serial.println(str);
    #else
        /* Silent function */
    #endif
    while(1);
}
