#ifndef _EPD_H_
#define _EPD_H_

#define EPD_WIDTH   648 
#define EPD_HEIGHT  480
#define EPD_ARRAY  (EPD_WIDTH * EPD_HEIGHT / 8)

// IO settings
/*
ESP32-WROOM-32D (Using hardware SPI)
Screen 1:
BUSY——A14
RES——A15
DC——A16
CS——A17

Screen 2:
BUSY——A19
RES——A20
DC——A21
CS——A22

SCK—GPIO18
SDIN—GPIO23
*/

// Screen 1 pins
#define isScreen1_BUSY digitalRead(A14)
#define screen1_RST_0 digitalWrite(A15, LOW)
#define screen1_RST_1 digitalWrite(A15, HIGH)
#define screen1_DC_0 digitalWrite(A16, LOW)
#define screen1_DC_1 digitalWrite(A16, HIGH)
#define screen1_CS_0 digitalWrite(A17, LOW)
#define screen1_CS_1 digitalWrite(A17, HIGH)

// Screen 2 pins
#define isScreen2_BUSY digitalRead(A19)
#define screen2_RST_0 digitalWrite(A20, LOW)
#define screen2_RST_1 digitalWrite(A20, HIGH)
#define screen2_DC_0 digitalWrite(A21, LOW)
#define screen2_DC_1 digitalWrite(A21, HIGH)
#define screen2_CS_0 digitalWrite(A22, LOW)
#define screen2_CS_1 digitalWrite(A22, HIGH)

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
