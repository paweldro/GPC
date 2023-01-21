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

Matrix UK(1, 1);
Matrix UKminusjeden(1, 1);

//wartość zadana
int wz = 20;

//chwilowe wyjscie
Matrix wyjscie(1, 1);

Matrix macierzdoprzesuwania(HU, HU);
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

    //Przypisanie wartości do macierzy I z karty SD
    
    readmatrix("I.txt", I, HU, HU);

    //Przypisanie wartości do macierzy I z karty SD
    
    readmatrix("jed.txt", jed, 1, HU);

    //Obliczanie macierzy H, P1, P2
    
    H = CA.Invers() * CB;
    P1 = -1* CA.Invers() * HA;
    P2 =  CA.Invers() * HB;
    
    //Wyświetlanie macierzy P2
    
    //P2.vPrintFull();
    float p = 0.5;
    W = 2*(H.Transpose()*H+p*I);
    pomocnicza = -1*jed*W.Invers();

    //V = -2*H.Transpose()*(YR-P1*Ywlewo-P2*deltaUwlewo); //loop dodane
    //deltaUk = pomocnicza*V; //loop dodane
    
    //deltaUwprawo[0][0]= deltaUk[0][0];
    
    //generowanie macierzy do przesuwania
    for(int i = 0; i<HU-1; i++ ){
      macierzdoprzesuwania[i][i+1]=1;
      
    }
    //generowanie macierzy YR
       for(int i = 0; i<HU; i++ ){
      YR[i][0]=wz;
      
    }
      
    //YR = YR.Transpose();
    //macierzdoprzesuwania[HY-1][0]=1;
   
    //YR = YR*macierzdoprzesuwania;
    //YR = YR.Transpose();

    
    /*//przesuwanie macierzy Ywlewo
    for(int i = NA-1; i>0; i-- )
    { 
      Ywlewo[i][0]=Ywlewo[i-1][0];
      }
     Ywlewo[0][0]=wyjscie[0][0];*/

    
    
    //wyliczanie delta u w lewo deltaUwlewo trzeba dopasować do transmitancji 
    //deltaUwlewo[0][0] =deltaUk[0][0];
    
    //Ywlewo.vPrintFull();

    //obliczanie chwilowego wyjscia - to na końcu
    //wyjscie=H*deltaUwprawo+P1*Ywlewo+P2*deltaUwlewo;

    
   }
  

void loop() {
  //loop
  if(steps < 40)
  {
  V = -2*H.Transpose()*(YR-P1*Ywlewo-P2*deltaUwlewo); //obliczanie wartosci V
  deltaUk = pomocnicza*V; //obliczanie wartosci delta Uk

  
  deltaUwprawo[0][0]= deltaUk[0][0]; //zweryfikowc bo wpisujemy tylko pierwsza wartosc, reszta to zera
  
  wyjscie=H*deltaUwprawo+P1*Ywlewo+P2*deltaUwlewo; //obliczanie chwilowego wyjscia

  //przesuwanie
  //przesuwanie macierzy Ywlewo
    for(int i = NA-1; i>0; i-- )
    { 
      Ywlewo[i][0]=Ywlewo[i-1][0];
      }
     Ywlewo[0][0]=wyjscie[0][0];
   
   //wyliczanie delta u w lewo deltaUwlewo trzeba dopasować do transmitancji 
    deltaUwlewo[0][0] =deltaUk[0][0];
    Serial.println(wyjscie[0][0]);
    //wyjscie.vPrintFull();
    delay(1000);
    steps = steps + 1;
  }
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
