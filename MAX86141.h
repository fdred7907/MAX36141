#ifndef MAX86141_H
#define MAX86141_H

#include "MAX86141_Registers.h"

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


    private:
        // CircularBuffer<FifoData,FIFO_SAMPLES> fifoBuffer;

        uint8_t readRegister(uint8_t regAddress,uint8_t length);
        void writeRegister(uint8_t regAddress, uint8_t data);
        void burstRead(uint8_t regAddress,uint8_t* buffer, uint8_t length);
        // void readFifoData();
};

//general

/* #device number
#spi device numbers
#default configurations

//ppg adc values
#ppg adc current range
#ppg adc integration time range
#photodiode input capacitance


//led values
#full scale led current
#minimum output voltage

*/




#endif
