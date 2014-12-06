#include "mbed.h"
#define GATE_IN p17
#define ANALOG_IN A0

class SoundDetector{
    public:
        SoundDetector(PinName envelope_pin, PinName gate_pin);

        float raw_envelope_read();
        float normalized_read(int gain);
        float ambient_average();

        
    private:
        AnalogIn *input_pin;
};