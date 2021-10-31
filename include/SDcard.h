#ifndef SDCARD_H
#define SDCARD_H

#include <FS.h>
#include <SPI.h>
#include <SD.h>
#include "queue.h"
#include "dataStructs.h"

#define SD_CS 4

bool SD_write(const String & file, const String & data);

#endif