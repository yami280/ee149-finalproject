/** Hook Up Guide
* an to a0
* 5v to 5v
* GND to GND
* todo? tx to any GPO for interrupt
*/

#include "mbed.h"
#include "SoundDetector.h"
#define GATE_IN p17
#define ANALOG_IN A0

Serial pc(USBTX, USBRX);
SoundDetector sd1(ANALOG_IN, GATE_IN);

SoundDetector::SoundDetector(PinName envelope_pin, PinName gate_pin) {
    input_pin = new AnalogIn(envelope_pin);
}

float SoundDetector::raw_envelope_read(){
    return input_pin->read();
}

float SoundDetector::normalized_read(int gain){
    return input_pin->read() * gain;
}

float SoundDetector::ambient_average(){
    //requires total of roughly size*wait second wait period
    float size = 10;
    float sum;
    for (int i = 0; i < size; i++){
        sum += this->raw_envelope_read()*100;
        wait(0.2); //wait 1 second
    }
    sum = sum / 10.0;
    return sum;
}

void setup(){
    pc.baud(9600);
    //more initialization
    pc.printf("Initialized");
}

//void soundISR(){
//  int pin_val;
//}

int main(){
    setup();
    float value;
    // Check the envelope input
    float noise = sd1.ambient_average();
    pc.printf("Enviornment Noise: %8.2f \r\n",noise);
    int recalibration_count = 0;
    int recalibration_point = 20; //recalibration every r_p seconds
    while(1){
        if (recalibration_count == recalibration_point){
            pc.printf("Enviornment Noise: %8.2f \r\n",sd1.ambient_average());
        }
        value = sd1.raw_envelope_read()*100;
        
        // Convert envelope value into a message
        pc.printf("Raw Input: %8.2f  || Status: ",value);
        if(value <= 10){
            pc.printf("Quiet. \r\n");
        }
        else if( (value > 10) && ( value <= 30) ){
            pc.printf("Moderate.\r\n");
        }
        else if(value > 30){
            pc.printf("Loud.\r\n");
        }
        
        // pause for 1 second
        wait(1.0);
        recalibration_count ++;
    }
}