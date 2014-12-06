#include "ITG3200.h"

Serial pc(USBTX, USBRX);
#define PIN_SDA I2C_SDA0
#define PIN_SCL I2C_SCL0

ITG3200 gyro(PIN_SDA, PIN_SCL);

int main() {

    pc.printf("Now starting ITG-3200 test...\n");
    
    //Set highest bandwidth.
    gyro.setLpBandwidth(LPFBW_42HZ);

    while (1) {

        //Arbitrary wait for printf clarity.
        wait(0.1);
        pc.printf("%i, %i, %i\n", gyro.getGyroX(),
                  gyro.getGyroY(), gyro.getGyroZ());

    }

}
