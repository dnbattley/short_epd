#ifndef _EPD_H_
#define _EPD_H_

#include <SPI.h>
#include <vector> // Include vector for modern C++ arrays


#define SCREEN_WIDTH 648
#define SCREEN_HEIGHT 480
#define EXPECTED_LENGTH (SCREEN_WIDTH * SCREEN_HEIGHT / 8) // Data length in bytes

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

// Function prototypes
void SPI_Write(unsigned char value);
void WriteData(unsigned char data, int screen);
void WriteCmd(unsigned char command, int screen);

// Full screen update display
void initDisplay(int mode, int screen);
void fillScreen(const std::vector<unsigned char>& data, unsigned char fill_value, bool fast_update, int screen);
void deepSleep(int screen);

// Partial update display
void partialUpdate(unsigned int x_start, unsigned int y_start, const std::vector<unsigned char>& data, 
                   unsigned int part_column, unsigned int part_line, int screen);
void partialUpdateAll(const std::vector<unsigned char>& data, int screen);

// GUI
void displayImage(const std::vector<unsigned char>& image, int screen, 
                  bool fast_update = false, bool partial_update = false, 
                  unsigned int x1 = 0, unsigned int y1 = 0, 
                  unsigned int x2 = SCREEN_WIDTH - 1, unsigned int y2 = SCREEN_HEIGHT - 1);
void displayImage(std::nullptr_t, int screen, bool fast_update = false); // Overload for clearing the screen

#endif
