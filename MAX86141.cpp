#include <SPI.h>
#include <arduino.h>
#include "MAX86141.h"


//global

//LEDC Sequence Array
    uint8_t LEDC[6];

//PPG Ranges
uint8_t PPG1_ADC_RGE;
uint8_t PPG2_ADC_RGE;

//data buffer
uint8_t dataBuffer[MAX_BUFFER_SIZE];


//class constructor with setup of SPI VSPI
MAX86141::MAX86141(){

    SPIClass *vspi = NULL;
    vspi = new SPIClass(VSPI);
    SPISettings = new SPISettings(SPIFREQ,SBI_BIT,SPI_MODE);

    
}


//begin function to setup the spi connection
bool MAX86141::begin(){

    vspi->begin(SCLK,MISO,MOSI,CS); //using custom pins 
}


// readRegister to read 8-bit register
uint8_t  MAX86141::readRegister(uint8_t regAddress){
    vspi->beginTransaction(SPISettings);

    digitalWrite(CS,LOW);
    vspi->transfer(regAddress);
    uint8_t data = vspi->transfer(0xFF);
    vspi->endTransaction();
    digitalWrite(CS,HIGH);
    return data;
    
}

//writeRegister to write 8-bit data into registers

void MAX86141::writeRegister(uint8_t regAddress, uint8_t data){
    vspi->beginTransaction(SPISettings);
    digitalWrite(CS,LOW);
    vspi->transfer(regAddress);
    vspi->transfer(data);
    vspi->endTransaction();
    digitalWrite(CS,HIGH);
}

//burst read

// burst read
 void MAX86141::burstRead(uint8_t regAddress,uint8_t* buffer,uint8_t length){
    vspi->beginTransaction(SPISettings);

    digitalWrite(CS,LOW);
    vspi->transfer(regAddress);
    vspi->transfer(buffer,length);
    vspi->endTransaction();
    digitalWrite(CS,HIGH);
    
}

//number of data samples available
uint8_t sampleCount(){
    uint8_t OVF_COUNTER = readRegister(0x06);
    uint8_t FIFO_DATA_COUNT = readRegister(0x07);
    if (OVF_COUNTER == 0){
        return FIFO_DATA_COUNT ; // no overflow , no of samples equals saved in data count
    }else{
        return 128; // overflow lost data
    }
}

//extract LEDC Sequence codes
void getLEDCSequence(uint8_t* LEDC){
    LEDC[0] = readRegister(LED_SEQ_REG_1) & 0xF;
    LEDC[1] = readRegister(LED_SEQ_REG_1) >> 4;
    LEDC[2] = readRegister(LED_SEQ_REG_2) & 0xF;
    LEDC[3] = readRegister(LED_SEQ_REG_2) >> 4;
    LEDC[4] = readRegister(LED_SEQ_REG_3) & 0xF;
    LEDC[5] = readRegister(LED_SEQ_REG_3) >> 4;
}

//extract PPG configuration details
void getPPGConfiguration(){
    PPG1_ADC_RGE = (readRegister(PPG_CONFIG_1) & 0xC) >> 2;
    PPG2_ADC_RGE = (readRegister(PPG_CONFIG_1) & 0x30) >> 4;
}

//calculate number of PPG channels activated
uint8_t getPPGChannelsCount(){
    getPPGConfiguration();
    uint8_t PPGChannelsCount = 0 ;
    if (PPG1_ADC_RGE){ PPGChannelsCount ++ };
    if (PPG2_ADC_RGE) { PPGChannelsCount ++ };
    return PPGChannelsCount;
}
//calculate number of LED channels using LEDCx Registers
uint8_t getLEDChannelsCount(){
    getLEDCSequence();
    uint8_t LEDChannelsCount = 0 ;
    for (int i = 0 ;i <6; i++){
        if (LEDC[i] != 0){
            LEDChannelsCount ++ ;
        }
    }
    return LEDChannelsCount;
}
void deviceDataRead(){
    int i;
    uint8_t sampleCnt; // samples count -> to be read from fifo register 0x07;
    uint8_t dataBuffer[FIFO_SAMPLES * 3] // 3 bytes per sample
    uint8_t LEDChannels;
    uint8_t PPGChannels;

    //calculate values
    sampleCnt = sampleCount();
    LEDChannels = getLEDChannelsCount();
    PPGChannels = getPPGChannelsCount();

    uint8_t ChannelsCount = LEDChannels + PPGChannels;
    Uint8_t dataPointCount = sampleCnt/ChannelsCount;

    //initialising data arrays
    uint8_t tag[LEDChannels][PPGChannels][dataPointCount];     // tag channels 
    uint8_t LEDData[LEDChannels][PPGChannels][dataPointCount]; // tag channels 

    uint8_t tag[dataPointCount][ChannelsCount]; //temporary buffer
    uint8_t LEDData[dataPointCount][ChannelsCount]; // temporay buffer

    //read fifo upto sampleCnt into dataBuffer
    burstRead(FIFO_DATA,dataBuffer,sampleCnt);

    for (int i = 0; i < dataPointCount; i++){
        for ( int j=0;j<ChannelsCount;j++){
            // extracting tag from each data point by 
            //right shifting 3 for getting 5 bits
            // and masking with 00011111 to extract them 
            tag[i][j] = (dataBuffer[i*3*ChannelsCount + 3*j]>>3)  & 0x1f; 

            //extracting led data per data point by left shifting first  byte 16
            // left shifting second byte 8
            //third byte no shift
            // masking with 07FFFF to rmeove the left most 5 bits of tag
            LEDData[i][j] = (dataBuffer[i*3*ChannelsCount+j*3] << 16) | 
                            (dataBuffer[i*3*ChannelsCount+j*3+1] << 8 )|
                            (dataBuffer[i*3*ChannelsCount+j*3+2]) 
                            & 0x7FFFF;
        }
}

int channel;
// arranging the extracted data as per channel numbers
for (int i = 0; i < dataPointCount; i++){
    channel = 0; 
    for (int j = 0 ; j < LEDChannels ; j++ ){
        for (int k =0 ; k < PPGChannels;k++){

                tag[j][k][i] = tag[i][channel];
                LEDData[j][k][i] = LEDData[i][channel];

                channel++;
            }
        }    
    }


}


