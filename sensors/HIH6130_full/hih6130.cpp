/*
  @file HIH6130.cpp
  
  @brief Humidity and Temperature Sensor HIH6130 Breakout I2C Library      

  @Author spiridion (http://mailme.spiridion.net)
  
  @Edited by Minyoung
  
  Tested on LPC1768 and FRDM-KL25Z
  (Edit: nRF51822)
  
  Copyright (c) 2014 spiridion
  Released under the MIT License (see http://mbed.org/license/mit)

  Documentation regarding I2C communication with HIH6130 can be found here: 
  http://mbed.org/media/uploads/spiridion/i2c_comms_humidicon_tn_009061-2-en_final_07jun12.pdf
*/

/** Hook Up guide for nRF51822
*   3.3 V => 3.3
*   GND   => GND
*   SCL   => SCL (P28)
*   SDA   => SDA (P29)
*/

#include "hih6130.h"
#include "mbed.h"

#define PIN_SDA I2C_SDA0
#define PIN_SCL I2C_SCL0

Serial pc(USBTX, USBRX);

HIH6130::HIH6130(PinName sda, PinName scl): i2c(sda,scl){
    temperature = UNSET_HI6130_TEMPERATURE_VALUE;
    humidity = UNSET_HI6130_HUMIDITY_VALUE;
    m_addr = HIH6130_I2C_ADDRESS;
}

int HIH6130::ReadData(float* pTemperature, float* pHumidity){
    int is_connected = Measurement();
    
    if (is_connected) { 
        temperature = TrueTemperature();
        humidity = TrueHumidity();
    }else {
        temperature = UNSET_HI6130_TEMPERATURE_VALUE;
        humidity = UNSET_HI6130_HUMIDITY_VALUE;
    }
    //store value at corresponding pointer    
    if (pTemperature)
        *pTemperature = temperature;
    if (pHumidity)
        *pHumidity = humidity;
        
    return is_connected;
}

int HIH6130::ReadTemperature(float* pTemperature){
    
    int is_connected = Measurement();
    
    if (is_connected) { 
        temperature = TrueTemperature();
    }
    
    if (pTemperature){
        *pTemperature = temperature;
    }
    return is_connected;
}

int HIH6130::ReadHumidity(float* pHumidity){
    
    int is_connected = Measurement();
    
    if (is_connected) { 
        humidity = TrueHumidity();
    }
    
    if (pHumidity){
        *pHumidity = humidity;
    }
    return is_connected;
}


float HIH6130::TrueTemperature(){   
    // T = T_output / (2^14-2)*165-40
    return ( ( ((unsigned int)m_data[2] << 8) | (unsigned int)m_data[3] ) >> 2 ) * 0.010072F - 40;  
}

float HIH6130::TrueHumidity(){
    // H = H_output /(2^14-2)*100
    return ( (((unsigned int)m_data[0] & 0x3f) << 8) | ((unsigned int)m_data[1] & 0xff) ) * 0.006104F;
}


int HIH6130::Measurement()
{
    int valid_check;

    // Humidity and temperature measurement request
    valid_check = i2c.write(m_addr, m_data, 1);
    
    wait_ms(10);
    
    // Humidity and temperature data fetch
    valid_check += i2c.read(m_addr, m_data, 4);

    //  Check data validity
    if ( valid_check || !(m_data[0] & 0xC0)) 
        return 0;
        
    return 1;
}

int main(){
    pc.baud(9600);
    HIH6130 hih6130(I2C_SDA0,I2C_SCL0);
    float humidity, temperature;
    float justhumidity;
    float justtemperature;
    while(1) {
        hih6130.ReadData(&temperature,&humidity);
        pc.printf("Humidity(%%RH): %8.2f \t Temperature(C): %8.2f\n", humidity, temperature);  
        wait(3.0);
        hih6130.ReadHumidity(&justhumidity);
        pc.printf("Humidity(%%RH): %8.2f \r\n", justhumidity);
        hih6130.ReadTemperature(&justtemperature);
        pc.printf("Temperature(%%RH): %8.2f \r\n", justtemperature);
        wait(3.0);
    }
}


