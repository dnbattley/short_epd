#include"Kindle_EPD.h"  // All-in-one E-paper SPI driver


unsigned char ImageBW[EPD_ARRAY];//Define canvas space  
void setup() {
 /* ESP32-WROOM-32D (Using hardware SPI)
  BUSY——GPIO32  RES——GPIO33  DC——GPIO25  CS——GPIO26  SCK—GPIO18  SDIN—GPIO23  */
   pinMode(14, INPUT);  //BUSY
   pinMode(15, OUTPUT); //RES 
   pinMode(16, OUTPUT); //DC   
   pinMode(17, OUTPUT); //CS   
  BUSY——GPIO27  RES——GPIO14  DC——GPIO12  CS——GPIO13  SCK—GPIO18  SDIN—GPIO23  */
   pinMode(19, INPUT);  //BUSY
   pinMode(20, OUTPUT); //RES 
   pinMode(21, OUTPUT); //DC   
   pinMode(22, OUTPUT); //CS 
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
