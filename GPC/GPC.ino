#include <Wire.h>
#include <elapsedMillis.h>
#include "konfig.h"
#include "matrix.h"
#include "mpc.h"



/* Plant system */
Matrix A(SS_X_LEN, SS_X_LEN);

//MOJE ZMIENNE
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
    
    
    /* Ref: https://www.mathworks.com/help/control/ug/mimo-state-space-models.html#buv3tp8-1
     * 
     * State-Space Model of Jet Transport Aircraft
     *  This example shows how to build a MIMO model of a jet transport. Because the development of a physical model 
     *  for a jet aircraft is lengthy, only the state-space equations are presented here. See any standard text in 
     *  aviation for a more complete discussion of the physics behind aircraft flight.
     * The jet model during cruise flight at MACH = 0.8 and H = 40,000 ft. is
     * 
     * (The model has two inputs and two outputs. The units are radians for beta (sideslip angle) and phi (bank angle) and 
     * radians/sec for yaw (yaw rate) and roll (roll rate). The rudder and aileron deflections are in degrees.)
     */ 
   /* 
    A[0][0] = -0.0558;      A[0][1] = -0.9968;      A[0][2] =  0.0802;      A[0][3] = 0.0415;
    A[1][0] =  0.5980;      A[1][1] = -0.1150;      A[1][2] = -0.0318;      A[1][3] = 0.0000;
    A[2][0] = -3.0500;      A[2][1] =  0.3880;      A[2][2] = -0.4650;      A[2][3] = 0.0000;
    A[3][0] =  0.0000;      A[3][1] =  0.0805;      A[3][2] =  1.0000;      A[3][3] = 0.0000;
    A.vPrint();
    */

    CA[0][0] =  1.0000;      CA[0][1] = 0.0000;      CA[0][2] =  0.0000;      CA[0][3] = 0.0000;     CA[0][4] = 0.0000;
    CA[1][0] =  -2.4170;      CA[1][1] = 1.0000;      CA[1][2] = 0.0000;      CA[1][3] = 0.0000;     CA[1][4] = 0.0000;
    CA[2][0] = 1.8339;      CA[2][1] = -2.4170;      CA[2][2] = 1.0000;      CA[2][3] = 0.0000;     CA[2][4] = 0.0000;
    CA[3][0] =  -0.4169;      CA[3][1] =  1.8339;      CA[3][2] =  -2.4170;      CA[3][3] =1.0000;     CA[3][4] = 0.0000;
    CA[4][0] =  0.0000;      CA[4][1] =  -0.4169;      CA[4][2] =  1.8339;      CA[4][3] =-2.4170;     CA[4][4] = 1.0000;


    CB[0][0] =  0.0119;      CB[0][1] = 0.0000;     
    CB[1][0] =  0.0089;      CB[1][1] = 0.0119;      
    CB[2][0] =  0.0000;      CB[2][1] = 0.0089;      
    CB[3][0] =  0.0000;      CB[3][1] =  0.0000;     
    CB[4][0] =  0.0000;      CB[4][1] =  0.0000;    

    HA[0][0] =  -2.4170;      HA[0][1] =  1.8339;      HA[0][2] =  -0.4169;     
    HA[1][0] =   1.8339;      HA[1][1] = -0.4169;      HA[1][2] = 0.0000;     
    HA[2][0] = -0.4169;      HA[2][1] = 0.0000;       HA[2][2] =0.0000;      
    HA[3][0] =  0.0000;        HA[3][1] =  0.0000;       HA[3][2] = 0.0000;      
    HA[4][0] =  0.0000;      HA[4][1] = 0.0000;        HA[4][2] =0.0000;  

    HB[0][0] =  0.0089;     
    HB[1][0] =   0.0000;          
    HB[2][0] = 0.0000;        
    HB[3][0] =  0.0000;            
    HB[4][0] =  0.0000;     

    H = CA.Invers() * CB;
    P1 = -1* CA.Invers() * HA;
    P2 =  CA.Invers() * HB;
    P2.vPrintFull();
}


void loop() {
  //loop
   
   

}




void SPEW_THE_ERROR(char const * str)
{
    #if (SYSTEM_IMPLEMENTATION == SYSTEM_IMPLEMENTATION_PC)
        cout << (str) << endl;
    #elif (SYSTEM_IMPLEMENTATION == SYSTEM_IMPLEMENTATION_EMBEDDED_ARDUINO)
        Serial.println(str);
    #else
        /* Silent function */
    #endif
    while(1);
}
