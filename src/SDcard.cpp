#include "SDcard.h"

SDCard::SDCard(uint8_t _mosi, uint8_t _miso, uint8_t _sck, uint8_t _cs):
    mosi(_mosi), miso(_miso), sck(_sck), cs(_cs){};

bool SDCard::init(){
    //pinMode(boardLed, OUTPUT);
    //digitalWrite(boardLed, LOW);
    spi = SPIClass(HSPI);
    spi.begin(sck, miso, mosi);
    SPI.setClockDivider(SPI_CLOCK_DIV2);
    
    if(!SD.begin(cs, spi)){
        return false;
    }

    return true;
}

bool SDCard::write(String path, const String & dataFrame){
    File file = SD.open(path, "a");  
    
    if(file) {
        if(!file.write((uint8_t *) dataFrame.c_str(), dataFrame.length())) {
            file.close();
            return false;
        }
    }else {
        return false;
    }
    
    file.close();

    return true;
}