#ifndef MAX86141_REGISTERS_H
#define MAX86141_REGISTERS_H

//status registers for interrupts

#define INTERRUPT_STATUS_1 0x00
#define INTERRUPT_STATUS_2 0x01
#define INTERRUPT_ENABLE_1 0x02
#define INTERRUPT_ENABLE_2 0x03

//FIFO REGISTERS

#define FIFO_WR_PTR 0x04
#define FIFO_RD_PTR 0x05
#define FIFO_OVF_CTR 0x06
#define FIFO_DAT_CTR 0x07
#define FIFO_DATA 0x08
#define FIFO_CONFIG_1 0x09
#define FIFO_CONFIG_2 0x0A

//SYSTEM CONTROL -> modes,resets,etc
#define SYS_CTRL_REG 0x0D

//PPG CONFIGURATION
#define PPG_SYN_CTRL 0x10
#define PPG_CONFIG_1 0x11
#define PPG_CONFIG_2 0x12
#define PPG_CONFIG_3 0x13
#define PRXY_INT_TSH 0x14
#define PHD_BIAD 0x15

//ppg picket fence
#define PPG_PKT_FEN 0x16

//LED sequence control

#define LED_SEQ_REG_1 0x20
#define LED_SEQ_REG_2 0x21
#define LED_SEQ_REG_3 0x22

//LED Pulse amplitude
#define LED_PA_1 0x23
#define LED_PA_2 0x24
#define LED_PA_3 0x25
#define LED_PA_4 0x26
#define LED_PA_5 0x27
#define LED_PA_6 0x28
#define LED_PIL_PA 0x29
#define LED_RAN_1 0x2A
#define LED_RAN_2 0x2B

//Maximum buffer size
#define MAX_BUFFER_SIZE 128

//PPG1 High Res

#define S1_HI_RES_DAC1 0x2C
#define S2_HI_RES_DAC1 0x2D
#define S3_HI_RES_DAC1 0x2E
#define S4_HI_RES_DAC1 0x2F
#define S5_HI_RES_DAC1 0x30
#define S6_HI_RES_DAC1 0x31

//PPG 2 Hi Res DAC

#define S1_HI_RES_DAC2 0x32
#define S2_HI_RES_DAC2 0x33
#define S3_HI_RES_DAC2 0x34
#define S4_HI_RES_DAC2 0x35
#define S5_HI_RES_DAC2 0x36
#define S6_HI_RES_DAC2 0x37

//temperature
#define DIE_TEMP_CONF 0x40
#define DIE_TEMP_INT 0x41
#define DIE_TEMO_FRA 0x42

//sha 
#define SHA_CMD 0xF0
#define SHA_CFG 0xF1

//memory
#define MEM_CTRL 0xF2
#define MEM_IDX 0xF3
#define MEM_DATA 0xF4

#define PART_ID 0xFF


//adc input current

typedef enum ADCInputCurr {
    PPGX_ADC_RGE_4_0 = 0x00,
    PPGX_ADC_RGE_8_0 = 0x01,
    PPGX_ADC_RGE_16_0 = 0x02,
    PPGX_ADC_RGE_32_0 = 0x03,

} ADCInputCurr;

typedef enum ADCIntegTime {
    PGP_TINT_14_8 = 0x0,
    PGP_TINT_29_4 = 0x1,
    PGP_TINT_58_7 = 0x2,
    PGP_TINT_117_3 = 0x3,
} ADCIntegTime;

#define MIN_PPG_SAMPLE_RATE 0x0A
#define MAX_PPG_SAMPLE_RATE 0x13

typedef enum PhotoDiodeCap {
    PDBIASX_65 = 0x1,
    PDBIASX_130 = 0x5,
    PDBIASX_260 = 0x6,
    PDBIASX_520 = 0x7,

} PhotoDiodeCap;

typedef enum LEDCurrent {
    LEDX_RGE_31 = 0x0,
    LEDX_RGE_62 = 0x1,
    LEDX_RGE_93 = 0x2,
    LEDX_RGE_124 = 0x3,
    
} LEDCurrent;

typedef enum LEDVoltage {
    LEDX_RGE_160 = 0x0,
    LEDX_RGE_317 = 0x1,
    LEDX_RGE_495 = 0x2,
    LEDX_RGE_700 = 0x3,
} LEDVoltage;

typedef enum LEDSeqDataType {
    LED_SEQ_DATA_NONE = 0000,
    LED_SEQ_DATA_LED1 = 0001,
    LED_SEQ_DATA_LED2 = 0010,
    LED_SEQ_DATA_LED3 = 0011,
    LED_SEQ_DATA_LED1_LED2 = 0100,
    LED_SEQ_DATA_LED1_LED3 = 0101,
    LED_SEQ_DATA_LED2_LED3 = 0110,
    LED_SEQ_DATA_LED1_LED2_LED3 = 0111,
    LED_SEQ_PILOT_LED1 = 1000,
    LED_SEQ_DIR_AMBIENT = 1001,
    LED_SEQ_DATA_LED4 = 1010,
    LED_SEQ_DATA_LED5 = 1011,
    LED_SEQ_DATA_LED6 = 1100,    
} LEDSeqDataType;

typedef enum FIFOTag {
    PPG1_LEDC1 = 00001,
    PPG1_LEDC2 = 00010,
    PPG1_LEDC3 = 00011,
    PPG1_LEDC4 = 00100,
    PPG1_LEDC5 = 00101,
    PPG1_LEDC6 = 00110,
    PPG2_LEDC1 = 00111,
    PPG2_LEDC2 = 01000,
    PPG2_LEDC3 = 01001,
    PPG2_LEDC4 = 01010,
    PPG2_LEDC5 = 01011,
    PPG2_LEDC6 = 01100,
    PPF1_LEDC1 = 01101,
    PPF1_LEDC2 = 01110,
    PPF1_LEDC3 = 01111,
    PPF2_LEDC1 = 10011,
    PPF2_LEDC2 = 10100,
    PPF2_LEDC3 = 10101,
    PROX1_DATA = 11001,
    PROX2_DATA = 11010,
    INVALID_DATA = 11110,
    TIME_STAMP = 11111,
} FIFOTag;

#endif





