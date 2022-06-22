#ifndef SDCARD_H
#define SDCARD_H

#include <FS.h>
#include <SPI.h>
#include <SD.h>
#include "queue.h"
#include "dataStructs.h"

//uint8_t boardLed = 2;

const String rawPath;
const String clcPath;

class SDCard{
    uint8_t mosi;
    uint8_t miso; 
    uint8_t sck;
    uint8_t cs;
    SPIClass spi;

    public:
    SDCard(uint8_t _mosi, uint8_t miso, uint8_t _sck, uint8_t _cs);
    bool init();
    bool write(String path, const String & data); 
};

#endif