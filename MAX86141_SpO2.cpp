#include "MAX86141_SpO2.h"


MAX86141 max;
MAX86141_SpO2::MAX86141_SpO2(){

}

float ir_dc;
float red_dc;
float red_ac_avg;
float ir_ac_avg;





void MAX86141:initialize(){
    max.writeRegister(SYS_CTRL_REG,(0x1) >> 7); //Soft Reset
    delay(1000);
    max.readRegister(INTERRUPT_STATUS_1) ; //CLEAR INTERRUPTS
    max.readRegister(INTERRUPT_STATUS_2); //clear interrupts
    max.writeRegister(SYS_CTRL_REG,(0x2)>>6); //shdn 
    max.writeRegister(PPG_CONFIG_1,PGP_TINT_117_3 >> 6); //pulse width setting
    max.writeRegister(PPG_CONFIG_1,((max.readRegister(PPG_CONFIG_1) & ~(0b00001100))| (PPGX_ADC_RGE_16_0 << 2))); //ADC Range 16uA
    max.writeRegister(PPG_CONFIG_1,((max.readRegister(PPG_CONFIG_1) & ~(0b00110000))| (PPGX_ADC_RGE_16_0 << 4))); //ADC Range 16uA

    //dual channel use
    max.writeRegister(PHD_BIAS,((max.readRegister(PHD_BIAS) & ~(0b00000111))|  LEDX_RGE_124 )); //led1 124 mA
    max.writeRegister(PHD_BIAS,((max.readRegister(PHD_BIAS) & ~(0b01110000))|  LEDX_RGE_124 << 4)); //led2 124 mA
    max.writeRegister(LED_PA_1,0x20) ; //led1 current 15.36 amp
    max.writeRegister(LED_PA_2,0x20) ; //led2 current 15.36 amp

    max.writeRegister(PPG_CONFIG_2,(max.readRegister(PPG_CONFIG_2)| 0x0 << 2)); // sample avveraging 1
    max.writeRegister(PPG_CONFIG_2,(max.readRegister(PPG_CONFIG_2) & ~(0b11111000)| 0x00 << 3)); // sample rate
    max.writeRegister(PPG_CONFIG_3,((max.readRegister(PPG_CONFIG_3) & ~(0b11000000)) | 0x3 << 6)); // LED settling time
    max.writeRegister(PHD_BIAS,(max.max.readRegister(PHD_BIAS) | 0x01 )); // PD biasing for cpd 0-65 pF


}

float MAX86141_SpO2::getSpO2(){
    //set up for led1 -> IR
    // LED2 -> Red
    //  2 channel PPG measurement

    max.deviceDataRead(); //collect ppg data with the setting

    // LEDData[0][0] -> LED1A data IR channel A
    // LEDData[0][1] -> LED1B data IR channel B
    // LEDData[1][0] -> LED2A data RED channel A
    // LEDData[1][1] -> LED2B data RED channel B

    //calculation of IR and RED DC
    float ir_sum = 0;
    float red_sum = 0;

    for (int i = 0 ; i <dataPointCount;i++){
        ir_sum += LEDData[0][0][i] + LEDData[0][1][i];
        red_sum += LEDData[1][0][i] + LEDData[1][1][i];
    }

    ir_dc = ir_sum/dataPointCount;
    red_dc = red_sum/dataPointCount;

    //calculation of IR and RED AC
    uint8_t ir_ac[dataPointCount];
    uint8_t red_ac[dataPointCount];

    for (i=0;i<dataPointCount;i++){
        ir_ac[i] = (LEDData[0][0][i] - ir_dc) + (LEDData[0][1][i] - ir_dc);
        red_ac[i] = (LEDData[1][0][i] - red_dc) + (LEDData[1][1][i] -red_dc);
    }

    //peak detection of IR and RED AC data
    uint8_t ir_peaks[dataPointCount];
    uint8_t red_peaks[dataPointCount];

    uint8_t ir_min_peak_ht = ir_dc;
    uint8_t red_min_peak_ht = red_dc;
    uint8_t min_dis = 4;

    uint8_t last_peak = -min_dis;

    for ( i=0;i<dataPointCount;i++){
        curr_peak_count_ir = 0;
        curr_peak_count_red = 0;
        if ((ir_ac[i] > ir_ac[i-1]) && (ir_ac[i] > ir_ac[i+1])){
            if (ir_ac[i]>ir_min_peak_ht){
                if ( i - last_peak > min_dis){
                    ir_peaks[curr_peak_count_ir++] = ir_ac[i];
                }
            }
        }

        if ((red_ac[i] > red_ac[i-1]) && (red_ac[i] > red_ac[i+1])){
            if (red_ac[i]>red_min_peak_ht){
                if ( i - last_peak > min_dis){
                    red_peaks[curr_peak_count_red++] = red_ac[i];
                }
            }
        }
    }

    float ir_ac_sum = 0; 
    float red_ac_sum = 0; 

    for(i=0;i<curr_peak_count_ir;i++){
        ir_ac_sum += ir_peaks[i];
    }

    ir_ac_avg = ir_ac_sum/curr_peak_count_ir;

    for (i=0;i<curr_peak_count_red;i++){
        red_ac_sum += red_peaks[i];
    }

    red_ac_avg = red_ac_sum/curr_peak_count_red;

    //calculation of Spo2
    float ratio = (red_ac_avg/red_dc)/(ir_ac_avg/ir_dc);
    float spo2 = 110 - 25 * ratio;
    return spo2;


}