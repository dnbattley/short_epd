#ifndef _EPD_H_
#define _EPD_H_

#define EPD_WIDTH   648 
#define EPD_HEIGHT  480
#define EPD_ARRAY  (EPD_WIDTH * EPD_HEIGHT / 8)

// IO settings
/*
ESP32-WROOM-32D (Using hardware SPI)
Screen 1:
BUSY——32
RES——33
DC——25
CS——26

Screen 2:
BUSY——27
RES——14
DC——12
CS——13

SCK—GPIO18
SDIN—GPIO23
*/

// Screen 1 pins
#define isScreen1_BUSY digitalRead(32)
#define screen1_RST_0 digitalWrite(33, LOW)
#define screen1_RST_1 digitalWrite(33, HIGH)
#define screen1_DC_0 digitalWrite(25, LOW)
#define screen1_DC_1 digitalWrite(25, HIGH)
#define screen1_CS_0 digitalWrite(26, LOW)
#define screen1_CS_1 digitalWrite(26, HIGH)

// Screen 2 pins
#define isScreen2_BUSY digitalRead(27)
#define screen2_RST_0 digitalWrite(14, LOW)
#define screen2_RST_1 digitalWrite(14, HIGH)
#define screen2_DC_0 digitalWrite(12, LOW)
#define screen2_DC_1 digitalWrite(12, HIGH)
#define screen2_CS_0 digitalWrite(13, LOW)
#define screen2_CS_1 digitalWrite(13, HIGH)

void SPI_Write(unsigned char value);
void WriteData(unsigned char data, int screen);
void WriteCmd(unsigned char command, int screen);

// Full screen update display
void initDisplay(int mode, int screen);
void fillScreenALL(const unsigned char *data, int screen);
void fillScreenWhite(int screen);
void fillScreenBlack(int screen);
void deepSleep(int screen);

// Partial update display
void partialUpdate(unsigned int x_start, unsigned int y_start, const unsigned char *data, unsigned int part_column, unsigned int part_line, int screen);
void partialUpdateAll(const unsigned char *data, int screen);

// GUI
void displayImage(const unsigned char *Image, int screen);

#endif
