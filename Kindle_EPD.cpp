#include "Kindle_EPD.h"

// Constants
#define EPD_WHITE 0xFF
#define EPD_BLACK 0x00
#define EPD_UPDATE_FULL 0
#define EPD_UPDATE_FAST 1
#define EPD_UPDATE_PARTIAL 2
#define EPD_GRAY_LEVELS 4

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
void EPD_W21_WriteCMD(unsigned char command)
{
    EPD_W21_CS_0;
    EPD_W21_DC_0;  // D/C#   0:command  1:data  
    SPI_Write(command);
    EPD_W21_CS_1;
}

// SPI write data
void EPD_W21_WriteDATA(unsigned char data)
{
    EPD_W21_CS_0;
    EPD_W21_DC_1;  // D/C#   0:command  1:data
    SPI_Write(data);
    EPD_W21_CS_1;
}

//////////////////////////////////////////////////////

// Global Variables
unsigned char partImage[1000]; // Partial canvas space

// Helper Functions
void epd_wait_busy(void) {
    // Implement a timeout mechanism to avoid potential infinite loops
    unsigned long start = millis();
    while (isEPD_W21_BUSY == 0) {
        if (millis() - start > 5000) { // Timeout after 5 seconds
            Serial.println("Error: EPD busy timeout");
            break;
        }
    }
}

void epd_reset(void) {
    EPD_W21_RST_0;
    delay(RESET_DELAY);
    EPD_W21_RST_1;
    delay(RESET_DELAY);
}

// Initialization
void epd_init(int mode) {
    epd_reset();
    EPD_W21_WriteCMD(0x00);
    EPD_W21_WriteDATA(0x1F); // EPD_W21_WriteDATA(0x13) here for 180 deg
    EPD_W21_WriteCMD(0x04); // POWER ON
    delay(POWER_ON_DELAY);
    epd_wait_busy();

    switch (mode) {
        case EPD_UPDATE_FAST:
            EPD_W21_WriteCMD(0X50); // VCOM AND DATA INTERVAL SETTING
            EPD_W21_WriteDATA(0x29);
            EPD_W21_WriteDATA(0x07);
            EPD_W21_WriteCMD(0xE0);
            EPD_W21_WriteDATA(0x02);
            EPD_W21_WriteCMD(0xE5);
            EPD_W21_WriteDATA(0x5A);
            break;

        case EPD_UPDATE_PARTIAL:
            EPD_W21_WriteCMD(0xE0);
            EPD_W21_WriteDATA(0x02);
            EPD_W21_WriteCMD(0xE5);
            EPD_W21_WriteDATA(0x6E);
            break;

        case EPD_UPDATE_FULL:
        default: // Default case for safety
            EPD_W21_WriteCMD(0X50); // VCOM AND DATA INTERVAL SETTING
            EPD_W21_WriteDATA(0x21);
            EPD_W21_WriteDATA(0x07);

            /* Additional code for 4G
            EPD_W21_WriteCMD(0xE0);
            EPD_W21_WriteDATA(0x02);
            EPD_W21_WriteCMD(0xE5);
            EPD_W21_WriteDATA(0x5F);
            */

            break;
    }
}

// Update
void epd_update(void) {
    EPD_W21_WriteCMD(0x12); // DISPLAY update
    delay(DISPLAY_UPDATE_DELAY);
    epd_wait_busy();
}

// Data Transfer
void epd_fill_screen(const unsigned char *data, unsigned char fill_value, bool fast_update) {
    unsigned int i;

    EPD_W21_WriteCMD(0x10); // write old data
    for (i = 0; i < EPD_ARRAY; i++) {
        EPD_W21_WriteDATA(EPD_WHITE);
    }

    EPD_W21_WriteCMD(0x13); // write new data
    if (data != NULL) {
        for (i = 0; i < EPD_ARRAY; i++) {
            EPD_W21_WriteDATA(data[i]);
        }
    } else {
        for (i = 0; i < EPD_ARRAY; i++) {
            EPD_W21_WriteDATA(fill_value);
        }
    }
    epd_update();
}

void epd_display(const unsigned char *image) {
    epd_fill_screen(image, EPD_WHITE, false);
}

void epd_partial_update(unsigned int x_start, unsigned int y_start, const unsigned char *data, unsigned int part_column, unsigned int part_line) {
    unsigned int x_end = x_start + part_line - 1;
    unsigned int y_end = y_start + part_column - 1;
    unsigned int i;

    epd_init(EPD_UPDATE_PARTIAL);

    EPD_W21_WriteCMD(0x91); // Enter partial mode
    EPD_W21_WriteCMD(0x90); // Resolution setting
    EPD_W21_WriteDATA(x_start / 256);
    EPD_W21_WriteDATA(x_start % 256);
    EPD_W21_WriteDATA(x_end / 256);
    EPD_W21_WriteDATA((x_end % 256) - 1);
    EPD_W21_WriteDATA(y_start / 256);
    EPD_W21_WriteDATA(y_start % 256);
    EPD_W21_WriteDATA(y_end / 256);
    EPD_W21_WriteDATA((y_end % 256) - 1);
    EPD_W21_WriteDATA(0x01);

    EPD_W21_WriteCMD(0x13); // Write new data
    for (i = 0; i < part_column * part_line / 8; i++) {
        EPD_W21_WriteDATA(data[i]);
    }
    epd_update();
}

void epd_partial_update_all(const unsigned char *data) {
    epd_partial_update(0, 0, data, EPD_HEIGHT, EPD_WIDTH);
}

void epd_deep_sleep(void) {
    EPD_W21_WriteCMD(0X50); // VCOM AND DATA INTERVAL SETTING
    EPD_W21_WriteDATA(0xf7); // WBmode:VBDF 17|D7 VBDW 97 VBDB 57  WBRmode:VBDF F7 VBDW 77 VBDB 37  VBDR B7

    EPD_W21_WriteCMD(0X02); // power off
    epd_wait_busy();
    delay(SLEEP_DELAY);
    EPD_W21_WriteCMD(0X07); // deep sleep
    EPD_W21_WriteDATA(0xA5);
}
