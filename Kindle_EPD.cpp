#include "Kindle_EPD.h"
#include <Arduino.h>
#include <SPI.h>
#include <vector>

// Constants
#define EPD_WHITE 0xFF
#define EPD_BLACK 0x00
#define UPDATE_FULL 0
#define UPDATE_FAST 1
#define UPDATE_PARTIAL 2
#define GRAY_LEVELS 4


#define RESET_DELAY 10
#define POWER_ON_DELAY 300
#define DISPLAY_UPDATE_DELAY 1
#define SLEEP_DELAY 100

// Functional components for SPI commands
//////////////////////////////////////////////////

// SPI write byte
void SPI_Write(unsigned char value)
{                            
   SPI.transfer(value);
}

// SPI write command
void WriteCmd(unsigned char command, int screen)
{
    if(screen == 1) {
        screen1_CS_0;
        screen1_DC_0; 
    } else {
        screen2_CS_0;
        screen2_DC_0;
    }
    SPI_Write(command);
    if(screen == 1) {
        screen1_CS_1;
    } else {
        screen2_CS_1;
    }
}

// SPI write data
void WriteData(unsigned char data, int screen)
{
    if(screen == 1) {
        screen1_CS_0;
        screen1_DC_1;
    } else {
        screen2_CS_0;
        screen2_DC_1;
    }
    SPI_Write(data);
    if(screen == 1) {
        screen1_CS_1;
    } else {
        screen2_CS_1;
    }
}

//////////////////////////////////////////////////////

// Helper Functions
void waitBusy(int screen) {
    unsigned long start = millis();
    while ((screen == 1 ? isScreen1_BUSY : isScreen2_BUSY) == 0) {
        if (millis() - start > 5000) { // Timeout after 5 seconds
            Serial.println("Error: EPD busy timeout");
            break;
        }
    }
}

void resetDisplay(int screen) {
    if(screen == 1) {
        screen1_RST_0;
    } else {
        screen2_RST_0;
    }
    delay(RESET_DELAY);
    if(screen == 1) {
        screen1_RST_1;
    } else {
        screen2_RST_1;
    }
    delay(RESET_DELAY);
}

// Initialization
void initDisplay(int mode, int screen) {
    resetDisplay(screen);
    WriteCmd(0x00, screen);
    WriteData(0x1F, screen); // WriteData(0x13) here for 180 deg
    WriteCmd(0x04, screen); // POWER ON
    delay(POWER_ON_DELAY);
    waitBusy(screen);

    switch (mode) {
        case UPDATE_FAST:
            WriteCmd(0X50, screen); // VCOM AND DATA INTERVAL SETTING
            WriteData(0x29, screen);
            WriteData(0x07, screen);
            WriteCmd(0xE0, screen);
            WriteData(0x02, screen);
            WriteCmd(0xE5, screen);
            WriteData(0x5A, screen);
            break;

        case UPDATE_PARTIAL:
            WriteCmd(0xE0, screen);
            WriteData(0x02, screen);
            WriteCmd(0xE5, screen);
            WriteData(0x6E, screen);
            break;

        case UPDATE_FULL:
        default: // Default case for safety
            WriteCmd(0X50, screen); // VCOM AND DATA INTERVAL SETTING
            WriteData(0x21, screen);
            WriteData(0x07, screen);

            /* Additional code for 4G
            WriteCmd(0xE0, screen);
            WriteData(0x02, screen);
            WriteCmd(0xE5, screen);
            WriteData(0x5F, screen);
            */

            break;
    }
}

// Update
void updateDisplay(int screen) {
    WriteCmd(0x12, screen); // DISPLAY update
    delay(DISPLAY_UPDATE_DELAY);
    waitBusy(screen);
}

// Data Transfer
void fillScreen(const std::vector<unsigned char>& data, unsigned char fill_value, bool fast_update, int screen) {
    unsigned int i;

    WriteCmd(0x10, screen); // write old data
    for (i = 0; i < EXPECTED_LENGTH; i++) {
        WriteData(EPD_WHITE, screen);
    }

    WriteCmd(0x13, screen); // write new data
    if (!data.empty()) {
        for (i = 0; i < data.size(); i++) {
            WriteData(data[i], screen);
        }
    } else {
        for (i = 0; i < EXPECTED_LENGTH; i++) {
            WriteData(fill_value, screen);
        }
    }
    updateDisplay(screen);
}


// Handle cases where an image is provided
void displayImage(const std::vector<unsigned char>& image, int screen, 
                  bool fast_update , bool partial_update, 
                  unsigned int x1, unsigned int y1, 
                  unsigned int x2, unsigned int y2) {
    // Handle initialization
    initDisplay(fast_update ? UPDATE_FAST : (partial_update ? UPDATE_PARTIAL : UPDATE_FULL), screen);

    if (partial_update) {
        // Validate coordinates
        if (x1 >= SCREEN_WIDTH || y1 >= SCREEN_HEIGHT || x2 >= SCREEN_WIDTH || y2 >= SCREEN_HEIGHT || x1 > x2 || y1 > y2) {
            Serial.println("Error: Invalid coordinates for partial update.");
            return;
        }

        // Calculate expected length for partial update
        unsigned int expected_partial_length = ((x2 - x1 + 1) * (y2 - y1 + 1)) / 8;
        if (image.size() < expected_partial_length) {
            Serial.printf("Warning: Image data length (%u bytes) is smaller than expected (%u bytes).\n", image.size(), expected_partial_length);
        }

        // Partial update
        partialUpdate(x1, y1, image, x2 - x1 + 1, y2 - y1 + 1, screen);
    } else {
        // Full update
        if (image.size() < EXPECTED_LENGTH) {
            Serial.printf("Warning: Image data length (%u bytes) is smaller than expected (%u bytes).\n", image.size(), EXPECTED_LENGTH);
        }

        // Perform full screen update
        fillScreen(image, EPD_WHITE, fast_update, screen);
    }
}

// Handle cases where nullptr is passed (clear the screen)
void displayImage(std::nullptr_t, int screen, bool fast_update) {
    fillScreen({}, EPD_WHITE, fast_update, screen); // Clear the screen
}

void partialUpdate(unsigned int x_start, unsigned int y_start, const std::vector<unsigned char>& data, 
                   unsigned int part_width, unsigned int part_height, int screen) {
    unsigned int x_end = x_start + part_width - 1;
    unsigned int y_end = y_start + part_height - 1;
    unsigned int i;

    // Validate dimensions
    if (x_end >= SCREEN_WIDTH || y_end >= SCREEN_HEIGHT) {
        Serial.println("Error: Partial update dimensions exceed screen size.");
        return;
    }

    initDisplay(UPDATE_PARTIAL, screen);

    WriteCmd(0x91, screen); // Enter partial mode
    WriteCmd(0x90, screen); // Resolution setting
    WriteData(x_start / 256, screen);
    WriteData(x_start % 256, screen);
    WriteData(x_end / 256, screen);
    WriteData((x_end % 256), screen);
    WriteData(y_start / 256, screen);
    WriteData(y_start % 256, screen);
    WriteData(y_end / 256, screen);
    WriteData((y_end % 256), screen);
    WriteData(0x01, screen);

    // Write image data
    for (i = 0; i < data.size(); i++) {
        WriteData(data[i], screen);
    }

    updateDisplay(screen);
}

void partialUpdateAll(const std::vector<unsigned char>& data, int screen) {
    if (data.size() < EXPECTED_LENGTH) {
        Serial.println("Error: Data length is smaller than the full screen size.");
        return;
    }
    partialUpdate(0, 0, data, SCREEN_HEIGHT, SCREEN_WIDTH, screen); // Correct call
}

void deepSleep(int screen) {
    WriteCmd(0X50, screen); // VCOM AND DATA INTERVAL SETTING
    WriteData(0xf7, screen); // WBmode:VBDF 17|D7 VBDW 97 VBDB 57  WBRmode:VBDF F7 VBDW 77 VBDB 37  VBDR B7

    WriteCmd(0X02, screen); // power off
    waitBusy(screen);
    delay(SLEEP_DELAY);
    WriteCmd(0X07, screen); // deep sleep
    WriteData(0xA5, screen);
}
