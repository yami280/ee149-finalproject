#include "VCNL4000.h"
#define PIN_SDA I2C_SDA0
#define PIN_SCL I2C_SCL0

const int VCNL4000::VCNL4000address = 0x13;
const int VCNL4000::VCNL4000regAddr = 0x80;
const int VCNL4000::Command                 = 0x0;
const int VCNL4000::ProdIdRevision          = 0x1;
const int VCNL4000::IRLedCurrent            = 0x3;
const int VCNL4000::AmbientLightParam       = 0x4;
const int VCNL4000::AmbientLightMsb         = 0x5;
const int VCNL4000::AmbientLightLsb         = 0x6;
const int VCNL4000::ProximityMsb            = 0x7;
const int VCNL4000::ProximityLsb            = 0x8;
const int VCNL4000::ProximitySigFreq        = 0x9;
const int VCNL4000::ProxymityModulationTime = 0xa;

Serial pc(USBTX, USBRX); // tx, rx

// ---------------------------------------------------
VCNL4000::VCNL4000( PinName sda, PinName scl ) : _i2c( sda, scl ) {
      
    int prodId = getProductId();
    int prodRev = getProductRevision();
      
    if( prodId == 1 && prodRev == 1 ) {
       _status = 0;
    }
    else {
       _status = 1;
    }  
}
   
// ---------------------------------------------------   
VCNL4000::~VCNL4000( void ) {
} 

// ---------------------------------------------------   
int VCNL4000::registerRead( int reg ) {

   _bytes[0] = ( reg & 0xff );
   _status = _i2c.write( ( VCNL4000address << 1 ), _bytes, 1 );
   if( _status == 0 ) {
      _status = _i2c.read( ( ( VCNL4000address << 1 ) + 1 ), _bytes, 1 );
      return( _bytes[0] );
   }
   return( 0 ); 
}

// ---------------------------------------------------      
void VCNL4000::registerWrite( int reg, unsigned char data ) {
    
    _bytes[0] = reg & 0xff;
    _bytes[1] = data & 0xff;
    _status = _i2c.write(  VCNL4000address << 1, _bytes, 2 );
    
}

// ---------------------------------------------------        
int VCNL4000::getProximity( void ) {

    startProximityMeasurement();
    while( !proximityDataReady() ) { 
      wait(0.1);
    }
    _data = registerRead( VCNL4000regAddr + ProximityMsb ) << 8;
    int status = _status;
    _data +=  registerRead( VCNL4000regAddr + ProximityLsb );
    _status = _status | status;
    return( _data );    
}   
   
// ---------------------------------------------------        
int VCNL4000::getAmbientLight( void ) {
    startAmbientLightMeasurement();
    while( !ambientLightDataReady() ) { 
      wait(0.1);
    }
    _data = registerRead( VCNL4000regAddr + AmbientLightMsb ) << 8;
    int status = _status;
    _data +=  registerRead( VCNL4000regAddr + AmbientLightLsb );
    _status = _status | status;
    return( _data );    
}   

// ---------------------------------------------------        
int VCNL4000::getProductId( void ) {
   return( ( registerRead(  VCNL4000regAddr + ProdIdRevision ) & 0xf0 ) >> 4 );
}

// ---------------------------------------------------        
int VCNL4000::getProductRevision( void ) {
   return( registerRead(  VCNL4000regAddr + ProdIdRevision ) & 0x0f ) ;
}  

// ---------------------------------------------------       
bool VCNL4000::proximityDataReady( void ) {
   return( ( registerRead( VCNL4000regAddr + Command ) & 0x20 ) >> 5 );     
}
   
// ---------------------------------------------------     
bool VCNL4000::ambientLightDataReady( void ) {
   return( ( registerRead( VCNL4000regAddr + Command )  & 0x40 ) >> 6 );     
}

// ---------------------------------------------------     
void VCNL4000::startProximityMeasurement( void ) {
   _data = registerRead( VCNL4000regAddr + Command );
   if( _status == 0 ) {
      _data = _data | 0x08;
      registerWrite( VCNL4000regAddr + Command, _data );
   }
}

// ---------------------------------------------------        
void VCNL4000::startAmbientLightMeasurement( void ) {
   _data = registerRead( VCNL4000regAddr + Command );
   if( _status == 0 ) {
      _data = _data | 0x10;
      registerWrite( VCNL4000regAddr + Command, _data );
   }
}

// ---------------------------------------------------     
void VCNL4000::setIRLedCurrent( int milliAmps ) {
   milliAmps /= 10;
   if( milliAmps > 20 ) {
      _status = 1;
      return;
   }
   _data = registerRead( VCNL4000regAddr + IRLedCurrent );
   if( _status == 0 ) {
      _data = ( _data & 0xc0 ) | milliAmps;
      registerWrite( VCNL4000regAddr + IRLedCurrent, _data );
   }   
}
   
// ---------------------------------------------------     
void VCNL4000::enableALContinuousConversionMode( void ) {
   _data = registerRead( VCNL4000regAddr + AmbientLightParam );
   if( _status == 0 ) {
      _data = _data | 0x80;
      registerWrite( VCNL4000regAddr + AmbientLightParam, _data );
   }   
}

// ---------------------------------------------------     
void VCNL4000::disableALContinuousConversionMode( void ) {
   _data = registerRead( VCNL4000regAddr + AmbientLightParam );
   if( _status == 0 ) {
      _data = _data & 0x7f;
      registerWrite( VCNL4000regAddr + AmbientLightParam, _data );
   }   
}

// ---------------------------------------------------     
bool VCNL4000::isALContinuousConversionMode( void ) {
   _data = registerRead( VCNL4000regAddr + AmbientLightParam );
   if( ( ( _data >> 7 ) & 1 ) == 1 ) {
      return( true );
   }
   else {
      return( false );
   }
}

// ---------------------------------------------------     
void VCNL4000::enableALAutoOffsetCompensation( void ) {
   _data = registerRead( VCNL4000regAddr + AmbientLightParam );
   if( _status == 0 ) {
      _data = _data | 0x08;
      registerWrite( VCNL4000regAddr + AmbientLightParam, _data );
   }   
}

// ---------------------------------------------------     
void VCNL4000::disableALAutoOffsetCompensation( void ) {
   _data = registerRead( VCNL4000regAddr + AmbientLightParam );
   if( _status == 0 ) {
      _data = _data & 0xf7;
      registerWrite( VCNL4000regAddr + AmbientLightParam, _data );
   }   
}
   
// ---------------------------------------------------        
bool VCNL4000::iseALAutoOffsetCompensation( void ) {
   _data = registerRead( VCNL4000regAddr + AmbientLightParam );
   if( ( ( _data >> 3 ) & 1 ) == 1 ) {
      return( true );
   }
   else {
      return( false );
   }
}

// ---------------------------------------------------     
void VCNL4000::setALAveragingFunction( int measurements ) {
   if( measurements > 7 ) {
      _status = 1;
      return;
   }
   _data = registerRead( VCNL4000regAddr + AmbientLightParam );
   if( _status == 0 ) {
      _data = ( _data & 0xf8 ) | measurements;
      registerWrite( VCNL4000regAddr + AmbientLightParam, _data );
   }   
}

// ---------------------------------------------------     
int VCNL4000::gettALAveragingFunction( void ) {
   _data = registerRead( VCNL4000regAddr + AmbientLightParam );
   if( _status ==  0 ) {
      return( _data & 0xf );
   }
   else {
      return( 0 );
   }
}

// ---------------------------------------------------     
void VCNL4000::setProximityMeasurementSigFreq( int frequency ) {
   if( frequency > 3 ) {
      _status = 1;
      return;
   }
   _data = registerRead( VCNL4000regAddr + ProximitySigFreq );
   if( _status == 0 ) {
      _data = ( _data & 0xfc ) | frequency;
      registerWrite( VCNL4000regAddr + ProximitySigFreq, _data );
   }   
}

// ---------------------------------------------------     
int VCNL4000::getProximityMeasurementSigFreq( void ) {
   _data = registerRead( VCNL4000regAddr + ProximitySigFreq );
   if( _status == 0 ) {
      return( _data & 0x3 );
   }
   else {
      return( 0 );
   }
}

// ---------------------------------------------------     
void VCNL4000::setProxiModulatorDelayTime( int delayTime ) {
   if( delayTime > 7 ) {
      _status = 1;
      return;
   }
   _data = registerRead( VCNL4000regAddr + ProxymityModulationTime );
   if( _status == 0 ) {
      _data = ( _data & 0x1f ) | ( delayTime << 5 );
      registerWrite( VCNL4000regAddr + ProxymityModulationTime, _data );
   }   
}   

// ---------------------------------------------------     
int VCNL4000::getProxiModulatorDelayTime( void ) {
   _data = registerRead( VCNL4000regAddr + ProxymityModulationTime );
   if( _status ==  0 ) {
      return( ( _data >> 5 ) & 0x7 );
   }
   else {
      return( 0 );
   }
}

// ---------------------------------------------------     
void VCNL4000::setProxiModulatorDeadTime( int deadTime ) {
   if( deadTime > 7 ) {
      _status = 1;
      return;
   }
   _data = registerRead( VCNL4000regAddr + ProxymityModulationTime );
   if( _status == 0 ) {
      _data = ( _data & 0x07 ) | deadTime;
      registerWrite( VCNL4000regAddr + ProxymityModulationTime, _data );
   }   
}   

// ---------------------------------------------------     
int VCNL4000::getProxiModulatorDeadTime( void ) {
   _data = registerRead( VCNL4000regAddr + ProxymityModulationTime );
   if( _status ==  0 ) {
      return( _data & 0x7 );
   }
   else {
      return( 0 );
   }
}
   

int main() {
   pc.printf("Intializing \r\n");
   VCNL4000 vcnl( I2C_SDA0, I2C_SCL0 );
   pc.printf("Still Intializing \r\n");
   int status = vcnl.getStatus();
   
   if( status != 0 ) {
       pc.printf( "\x1b[2J\x1b[f Something went wrong with VCNL4000 access, status = %d.\n\r", status );
       pc.printf( "ProductID: %d \n\r", vcnl.getProductId());
       pc.printf( "ProductRevision: %d \n\r" , vcnl.getProductRevision());
       exit( 1 );
   }
    
   vcnl.setALAveragingFunction( 0x0f );
   vcnl.setIRLedCurrent( 2000 );
   vcnl.setProximityMeasurementSigFreq( 2 );
   vcnl.setProxiModulatorDelayTime( 0x04 );
   vcnl.setProxiModulatorDeadTime( 0x01 );
   
   while( 1 ) {
      int prox = vcnl.getProximity();
      int ambl = vcnl.getAmbientLight();
      pc.printf("\x1b[2J\x1b[f proxy: %d\n\r amb.l: %d\n\r", prox, ambl ); 
      wait( 0.1 );
   }     
}
