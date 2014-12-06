/* mbed VCNL4000 Library version 0beta1
 * Copyright (c) 2012 bengo
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
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
 */

#ifndef VCNL4000_H
#define VCNL4000_H

#include "mbed.h"

/** VCNL4000 distance/luminosity sensor controller library
 *
 * Example:
 * @code
 * #include "mbed.h"
 * #include "VCNL4000.h"
 *
 * Serial pc(USBTX, USBRX); // tx, rx
 *
 * // -----------------------------------
 * int main() {
 *
 *    VCNL4000 vcnl( p9, p10 );
 *
 *    int status = vcnl.getStatus();
 *    
 *    if( status != 0 ) {
 *        pc.printf( "\x1b[2J\x1b[f Something went wrong with VCNL4000 access, status = %d.\n\r", status );
 *        exit( 1 );
 *    }
 *  
 *    vcnl.setALAveragingFunction( 0x0f );
 *    vcnl.setIRLedCurrent( 2000 );
 *    vcnl.setProximityMeasurementSigFreq( 2 );
 *    vcnl.setProxiModulatorDelayTime( 0x04 );
 *    vcnl.setProxiModulatorDeadTime( 0x01 );
 *    
 *    while( 1 ) {
 *       int prox = vcnl.getProximity();
 *       int ambl = vcnl.getAmbientLight();
 *       pc.printf("\x1b[2J\x1b[f proxy: %d\n\r amb.l: %d\n\r", prox, ambl ); 
 *       wait( 0.1 );
 *    }     
 * }
 * @endcode
 */

class VCNL4000 {

 public:
 
   /**
    *  Create a VCNL4000 object connected to the specified I2C pins
    * @param sda I2C SDA pin
    * @param scl I2C SDL pin  
    */
   VCNL4000( PinName sda, PinName scl );
   
   ~VCNL4000( void );
   
   /**
    * Return status code of previos function call
    */
   inline int getStatus( void ) { return( _status ); }
   
   /**
    * Read VCNL4000 internal register content
    * @param reg register address 
    */
   int registerRead( int reg );
   
   /**
    * Send data to VCNL4000 internal register
    * @param reg register address
    * @param data value to be written
    */
   void registerWrite( int reg, unsigned char data );
   
   /**
    * Read VCNL4000 proximity measurement
    */
   int getProximity( void );
   
   /**
    * Read VCNL4000 ambient light measurement
    */
   int getAmbientLight( void );
   
   /**
    * Get VCNL 4000 product id (should be 1)
    */
   int getProductId( void );
   
   /**
    * Get VCNL 4000 product revision (should be 1)
    */
   int getProductRevision( void );

   /**
    * Return True when a proximity data measurement completed
    */ 
   bool proximityDataReady( void );
   
   /**
    * Return True when an ambient light measurement completed
    */
   bool ambientLightDataReady( void );

   /**
    * Tell VCNL4000 to start a proximity measurement
    */
   void startProximityMeasurement( void );
   
   /**
    * Tell VCNL4000 to start an ambient light measurement
    */
   void startAmbientLightMeasurement( void );
   
   /** 
    * Set VCNL4000 infrared led current
    * @param milliAmps current in mA
    */
   void setIRLedCurrent( int milliAmps );
   
   /*
    * Enable VCNL4000 continuous conversion mode
    */
   void enableALContinuousConversionMode( void );

   /*
    * Disable VCNL4000 continuous conversion mode
    */
   void disableALContinuousConversionMode( void );
   
   /*
    * Return True if VCNL4000 is set in continuous conversion mode
    */ 
   bool isALContinuousConversionMode( void );

   /*
    * Enable ambient light auto offset compensation
    */
   void enableALAutoOffsetCompensation( void );
   
   /*
    * Disable ambient light auto offset compensation
    */
   void disableALAutoOffsetCompensation( void );
   
   /*
    * Return True if ambient light auto offset compensation is set
    */
   bool iseALAutoOffsetCompensation( void );

   /*
    * Set ambient light averaging function
    * @param measurements [0-7] 2^(measurements) = number of conversions 
    */
   void setALAveragingFunction( int measurements );
   
   /*
    * Return the averaging functions. Number of conversions = 2^value
    */
   int gettALAveragingFunction( void );

   /*
    * Set proximity measurement signal frequency
    * @param frequency [0-3] IR signal frequency 3.125/(frequency+1) 
    */
   void setProximityMeasurementSigFreq( int frequency );
   
   /*
    * Return IR signal frequency. Frequency = 3.125/(value+1) MHz
    */
   int getProximityMeasurementSigFreq( void );

   /*
    * Set proximity modulator delay time 
    * @param delayTime delay time. This value should be provided by Vishay 
    */ 
   void setProxiModulatorDelayTime( int delayTime );
   
   /*
    * Return proximity modulator delay time 
    */   
   int getProxiModulatorDelayTime( void );
   
   /*
    * Set proximity modulator dead time 
    * @param deadTime dead time. This value should be provided by Vishay 
    */ 
   void setProxiModulatorDeadTime( int deadTime );

   /*
    * Return proximity modulator dead time 
    */
   int getProxiModulatorDeadTime( void );   

 private:

   int _status;
   char _bytes[2];
   int _data;
   I2C _i2c;
   
   static const int VCNL4000address;
   static const int VCNL4000regAddr;
   static const int Command;
   static const int ProdIdRevision;
   static const int IRLedCurrent;
   static const int AmbientLightParam;
   static const int AmbientLightMsb;
   static const int AmbientLightLsb;
   static const int ProximityMsb;
   static const int ProximityLsb;
   static const int ProximitySigFreq;
   static const int ProxymityModulationTime;
          
 };

#endif