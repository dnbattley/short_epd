#ifndef _EPD_H_
#define _EPD_H_

#define EPD_WIDTH   648 
#define EPD_HEIGHT  480
#define EPD_ARRAY  (EPD_WIDTH * EPD_HEIGHT / 8)

// IO settings
/*
ESP32-WROOM-32D (Using hardware SPI)
BUSY——A14
RES——A15
DC——A16
CS——A17
SCK—GPIO18
SDIN—GPIO23
*/
#define isEPD_W21_BUSY digitalRead(A14)  // BUSY (HIGH or LOW)
#define EPD_W21_RST_0 digitalWrite(A15, LOW)  // RES
#define EPD_W21_RST_1 digitalWrite(A15, HIGH)
#define EPD_W21_DC_0 digitalWrite(A16, LOW)  // DC
#define EPD_W21_DC_1 digitalWrite(A16, HIGH)
#define EPD_W21_CS_0 digitalWrite(A17, LOW)  // CS
#define EPD_W21_CS_1 digitalWrite(A17, HIGH)

void SPI_Write(unsigned char value);
void EPD_W21_WriteDATA(unsigned char data);
void EPD_W21_WriteCMD(unsigned char command);

// Full screen update display
void epd_init(int mode);
void EPD_WhiteScreen_ALL(const unsigned char *data);
void EPD_WhiteScreen_White(void);
void EPD_WhiteScreen_Black(void);
void epd_deep_sleep(void);

// Partial update display
void epd_partial_update(unsigned int x_start, unsigned int y_start, const unsigned char *data, unsigned int part_column, unsigned int part_line);
void epd_partial_update_all(const unsigned char *data);

// GUI
void epd_display(const unsigned char *Image);

#endif
