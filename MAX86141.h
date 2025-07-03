#ifndef MAX86141_H
#define MAX86141_H

#include "MAX86141_Registers.h"


#include <SPI.h>
#include <arduino.h>
#include<stdint.h>


#define PART_ID 0xFF
#define MISO 32
#define MOSI 26
#define SCLK 25
#define CS 33
#define SPIFREQ 10000000;
#define SPI_BIT MSBFIRST;
#define SPI_MODE SPI_MODE3;



typedef uint8_t FifoData;





class MAX86141 {
    public:
        MAX86141();
        bool begin();
        void deviceDataRead(void);
        uint8_t sampleCount();//to read sample count
        void getLEDCSequence(uint8*);//to get LEDC sequence
        void getPPGConfiguration();//to read PPG values
        uint8_t getPPGChannelsCount();//to get ppg channel count
        uint8_t getLEDChannelsCount();//to get LED channels count




    private:

        uint8_t readRegister(uint8_t regAddress,uint8_t length);
        void writeRegister(uint8_t regAddress, uint8_t data);
        void burstRead(uint8_t regAddress,uint8_t* buffer, uint8_t length);
};






#endif
