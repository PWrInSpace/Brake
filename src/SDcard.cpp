#include "SDcard.h"

bool SD_write(const String & path, const String & dataFrame){
    
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