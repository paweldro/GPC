#include <Wire.h>
#include <elapsedMillis.h>
#include "konfig.h"
#include "matrix.h"
#include "mpc.h"


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
    
    
    //Przypisanie wartości do macierzy CA

    CA[0][0] =  1.0000;      CA[0][1] = 0.0000;      CA[0][2] =  0.0000;      CA[0][3] = 0.0000;     CA[0][4] = 0.0000;
    CA[1][0] =  -2.4170;      CA[1][1] = 1.0000;      CA[1][2] = 0.0000;      CA[1][3] = 0.0000;     CA[1][4] = 0.0000;
    CA[2][0] = 1.8339;      CA[2][1] = -2.4170;      CA[2][2] = 1.0000;      CA[2][3] = 0.0000;     CA[2][4] = 0.0000;
    CA[3][0] =  -0.4169;      CA[3][1] =  1.8339;      CA[3][2] =  -2.4170;      CA[3][3] =1.0000;     CA[3][4] = 0.0000;
    CA[4][0] =  0.0000;      CA[4][1] =  -0.4169;      CA[4][2] =  1.8339;      CA[4][3] =-2.4170;     CA[4][4] = 1.0000;

    //Przypisanie wartości do macierzy CB
    
    CB[0][0] =  0.0119;      CB[0][1] = 0.0000;     
    CB[1][0] =  0.0089;      CB[1][1] = 0.0119;      
    CB[2][0] =  0.0000;      CB[2][1] = 0.0089;      
    CB[3][0] =  0.0000;      CB[3][1] =  0.0000;     
    CB[4][0] =  0.0000;      CB[4][1] =  0.0000;    

    //Przypisanie wartości do macierzy HA
    
    HA[0][0] =  -2.4170;      HA[0][1] =  1.8339;      HA[0][2] =  -0.4169;     
    HA[1][0] =   1.8339;      HA[1][1] = -0.4169;      HA[1][2] = 0.0000;     
    HA[2][0] = -0.4169;      HA[2][1] = 0.0000;       HA[2][2] =0.0000;      
    HA[3][0] =  0.0000;        HA[3][1] =  0.0000;       HA[3][2] = 0.0000;      
    HA[4][0] =  0.0000;      HA[4][1] = 0.0000;        HA[4][2] =0.0000;  

    //Przypisanie wartości do macierzy HB
    
    HB[0][0] =  0.0089;     
    HB[1][0] =   0.0000;          
    HB[2][0] = 0.0000;        
    HB[3][0] =  0.0000;            
    HB[4][0] =  0.0000;     

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
