#ifndef _EPD_H_
#define _EPD_H_

#define EPD_WIDTH   648 
#define EPD_HEIGHT  480
#define EPD_ARRAY  EPD_WIDTH*EPD_HEIGHT/8  

//
//IO settings
/*
ESP32-WROOM-32D (Using hardware SPI)
BUSY——A14
RES——A15
DC——A16
CS——A17
SCK—GPIO18
SDIN—GPIO23
*/
#define isEPD_W21_BUSY digitalRead(A14)  //BUSY
#define EPD_W21_RST_0 digitalWrite(A15,LOW)  //RES
#define EPD_W21_RST_1 digitalWrite(A15,HIGH)
#define EPD_W21_DC_0  digitalWrite(A16,LOW) //DC
#define EPD_W21_DC_1  digitalWrite(A16,HIGH)
#define EPD_W21_CS_0 digitalWrite(A17,LOW) //CS
#define EPD_W21_CS_1 digitalWrite(A17,HIGH)


void SPI_Write(unsigned char value);
void EPD_W21_WriteDATA(unsigned char datas);
void EPD_W21_WriteCMD(unsigned char command);

//Full screen update display
void EPD_Init(void); 
void EPD_Init_180(void);  
void EPD_WhiteScreen_ALL(const unsigned char *datas);
void EPD_WhiteScreen_White(void);
void EPD_WhiteScreen_Black(void);
void EPD_DeepSleep(void);
//Partial update display 
void EPD_Init_Part(void);
void EPD_SetRAMValue_BaseMap(const unsigned char * datas);
void EPD_Dis_PartAll(const unsigned char * datas);
void EPD_Dis_Part(unsigned int x_start,unsigned int y_start,const unsigned char * datas,unsigned int PART_COLUMN,unsigned int PART_LINE);
void EPD_Dis_Part_Time(unsigned int x_start,unsigned int y_start,
                        const unsigned char * datas_A,const unsigned char * datas_B,
                        const unsigned char * datas_C,const unsigned char * datas_D,const unsigned char * datas_E,
                        unsigned char num,unsigned int PART_COLUMN,unsigned int PART_LINE);                    
//Fast update display
void EPD_Init_Fast(void);
void EPD_WhiteScreen_ALL_Fast(const unsigned char *datas);   
//4 Gray
void EPD_Init_4G(void);
void EPD_WhiteScreen_ALL_4G (const unsigned char *datas);		
 //GUI
 void EPD_Display(unsigned char *Image);

#endif



