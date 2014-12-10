/**
 * @file MaxSonar.h
 * @section LICENSE
 * Copyright (c) 2010 Mustafa Ozgur Kanli.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"),to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 * @section Description
 * This class provides an object to obtain range measurements from a
 * Maxbotix MaxSonar ultrasonic range finder.
 *
 * Supported devices:
 * 1) LV-MaxSonar: -EZ0 MB1000, -EZ1 MB1010, -EZ2 MB1020,
 *                 -EZ3 MB1030, -EZ4 MB1040
 *
 * Supported modes:
 * 1) Analog.
 */
#ifndef MAXSONAR_H_
#define MAXSONAR_H_

#include "mbed.h"

//! Define units for returning range.
enum MSUnits {
    MS_CM = 0,      //!< centimeters.
    MS_INCH         //!< inches.
};

class MaxSonar {

private:
    enum MSUnits units;         //!< Range units.
    float voltage;              //!< Supply/reference voltage (V).
    int analog_scale;           //!< resolution = voltage/scale.
    float analog_resolution;    //!< V/inch to compute range.
    AnalogIn *ain;              //!< For analog reads.
    DigitalOut *rx_req;         //!< For triggered reads (PWM or AIN).

public:
    /**
     * Constructor.
     *
     * @param   pin1    MS_ANALOG: Pin connected to RX of device.
     *                  MS_SERIAL: Pin connected to RX of device.
     *                  MS_PWM: Pin connected to RX of device.
     * @param   pin2    MS_ANALOG: Pin connected to AN of device.
     *                  MS_SERIAL: Pin connected to TX of device.
     *                  MS_PWM: Pin connected to PW of device.
     * @note    pin1 may be NC if only continuous reading is desired
     *          when in analog or pwm mode.
     * @note    Default units are in cm (MS_CM).
     */
    MaxSonar(PinName pin1, PinName pin2);

    /**
     * Destructor.
     */
    ~MaxSonar(void);

    /**
     * Set the units for reading range value. Default cm (MS_CM).
     *
     * @param   units   The specified units option.
     */
    void setUnits(enum MSUnits units);

    /**
     * Specify the supply voltage used by the device. 
     * 
     * @param   voltage The specified voltage (default 3.3)
     *
     * @note    This is important for correct conversion of the voltage 
     *          from pin AN of device into the range value.
     */
    void setVoltage(float voltage);

    /**
     * Trigger a reading of range. The reading will be ready 49 ms
     * after the trigger, accessable by the read() function. 
     * 
     * @param   none
     *
     * @note    Triggered reading is only possible if a pin connected to 
     *          RX of the device was specified when calling the constructor. 
     */
    void triggerRead(void);

    /**
     * Read range value.
     *
     * @return  Range value in the set units (default cm).
     */
    float read(void);
};

#endif
