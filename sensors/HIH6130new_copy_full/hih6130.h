#define HIH6130_H

#include "mbed.h"

///  default address is not 0X27, as stated in the documentation, but 0x4E (0x27<<1 ?) 
#define HIH6130_I2C_ADDRESS 0x4E 

#define UNSET_HI6130_HUMIDITY_VALUE -100.F
#define UNSET_HI6130_TEMPERATURE_VALUE -273.15F // absolute zero
//make it cleaner later
class HIH6130 {
    public:
        HIH6130(PinName sda, PinName scl);
        int ReadData(float* pTemperature=NULL, float* pHumidity=NULL);
        float TrueTemperature();
        float TrueHumidity();
        int ReadTemperature(float* pTemperature=NULL);
        int ReadHumidity(float* pHumidity=NULL);
        int Measurement();
        char m_data[4];
        float temperature;
        float humidity;
        I2C i2c;
        int m_addr;
};