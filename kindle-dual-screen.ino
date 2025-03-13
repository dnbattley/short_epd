#include"Kindle_EPD.h"  // All-in-one E-paper SPI driver


unsigned char ImageBW[EPD_ARRAY];//Define canvas space  
void setup() {

// BUSY——GPIO32  RES——GPIO33  DC——GPIO25  CS——GPIO26  SCK—GPIO18  SDIN—GPIO23
pinMode(32, INPUT);  // BUSY
pinMode(33, OUTPUT); // RES
pinMode(25, OUTPUT); // DC
pinMode(26, OUTPUT); // CS
// BUSY——GPIO27  RES——GPIO14  DC——GPIO12  CS——GPIO13  SCK—GPIO18  SDIN—GPIO23
pinMode(27, INPUT);  // BUSY
pinMode(14, OUTPUT); // RES
pinMode(12, OUTPUT); // DC
pinMode(13, OUTPUT); // CS

   //SPI
   SPI.beginTransaction(SPISettings(10000000, MSBFIRST, SPI_MODE0)); 
   SPI.begin ();  
}

void loop() {
    unsigned char i;
 
      initDisplay(0,1); //Full screen update initialization.
      displayImage(NULL,1); //Clear screen function.
      EPD_DeepSleep(); //Enter the sleep mode and please do not delete it, otherwise it will reduce the lifespan of the screen.
      delay(2000); //Delay for 2s. 

  while(1); // The program stops here
  
} 
