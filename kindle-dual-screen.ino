//
// THIS IS COMPILED BUT UNTESTED CODE FOR THE KINDLE PROJECT
// 
// FUNCTIONALITY IS TO SET UP TWO SCREENS, PRINTING AN IMAGE TO THEM
// VIA THE "DISPLAYIMAGE()" FUNCTION
//
// USAGE: displayImage(image, screen_number)
// displayImage(image_data, 1); // Defaults to full refresh
// displayImage(image_data, 1, true); // Fast refresh
// displayImage(partial_image, 2, false, true, 100, 100, 200, 200); // Partial refresh on screen 2
//
//
//
//

#include "Kindle_EPD.h"  // All-in-one E-paper SPI driver
#include <SPI.h>
#include <Arduino.h>
#include <vector>
// Define a vector to hold the canvas space
std::vector<unsigned char> ImageBW; // No need to predefine size yet

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
    /*
    // Load image data from SD card
    std::vector<unsigned char> imageData = loadImageData("/image.bin");

    if (!imageData.empty()) {
        // Display the loaded image
        displayImage(imageData, 1);
    }
    */
    displayImage(nullptr,1); //Clear screen function.
    deepSleep(1); // ALWAYS Enter sleep mode after calling displayImage

    delay(2000); // Delay for 2s
    while (1);   // Stop the program
}

/*
    // Example with a proper image
    std::vector<unsigned char> imageData(EXPECTED_LENGTH, EPD_WHITE); // Example: Initialize with white pixels
    displayImage(&imageData, 1);
*/


/* EXAMPLE CODE FOR THE SD CARD LOADING
#include <vector>
#include <SD.h>

std::vector<unsigned char> loadImageData(const char* filename) {
    File imageFile = SD.open(filename, FILE_READ);

    if (!imageFile) {
        Serial.println("Error: Failed to open the file on the SD card.");
        return {};
    }

    // Allocate vector to hold file data
    std::vector<unsigned char> buffer(imageFile.size());
    imageFile.read(buffer.data(), buffer.size());
    imageFile.close();

    return buffer;
}
*/