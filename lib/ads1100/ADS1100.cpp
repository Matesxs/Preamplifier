/**************************************************************************/
/*
	Library for ADS1100 Analog to digital converter
	It uses VCC as a reference voltage making it ideal to measure
	resistance - thermistors are an example.
	
		The unit is made by Texas Instruments, 
	16-Bit, 128SPS, 1-Ch Delta-Sigma ADC w/ PGA, Oscillator & I2C
	http://www.ti.com/product/ADS1100#
	
	The library updated on 18 May 2019 by Nikolay Cherkasov
	Github: https://github.com/cherkasovn/ADS1100
	
	GNU General Public License v3.0
	Distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
	without even the implied warranty of 
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*/
/**************************************************************************/

#include "ADS1100.h"
#if ARDUINO >= 100
	#include "Arduino.h"
	#else
	#include "WProgram.h"
#endif

#include <Wire.h>

/**************************************************************************/
/*
	Initializing the ADS1100 class
*/
/**************************************************************************/
ADS1100::ADS1100(byte address) 
{
	ads1100_i2cAddress=address;
}


/**************************************************************************/
/*
	Starts the I2C communication (Wire.begin) with the ADS1100 unit
*/
/**************************************************************************/
void ADS1100::begin(TwoWire *wire)
{
	m_wire = wire;
}

/**************************************************************************/
/*
	Reads the ADC value data regardless of the fact it is recent or not. Works (obligatory) in continuous reading mode, optional in the single mode. Read the value int16_t instead
*/
/**************************************************************************/
int16_t ADS1100::getvalue(void)
{
	int8_t highbyte;
	uint8_t lowbyte;
	Wire.requestFrom((uint8_t) ads1100_i2cAddress, (uint8_t) 2);  
	while(Wire.available())						// ensure all the data comes in
	{ 
		highbyte  = Wire.read();				// high byte * B11111111
		lowbyte   = Wire.read();				// low byte
	}
	value=(highbyte*256 + lowbyte);
	return value;
}

/**************************************************************************/
/*
	Parameters: None Description: Configures the operation mode of the ADS1100 unit.
	Possible options:  (ADS1100_CONVERSION_SINGLE or ADS1100_CONVERSION_CONTINOUS, ADS1100_DATA_RATE_128SPS or ADS1100_DATA_RATE_32SPS or ADS1100_DATA_RATE_16SPS or ADS1100_DATA_RATE_8SPS, ADS1100_GAIN_1X or ADS1100_GAIN_2X or ADS1100_GAIN_4X or ADS1100_GAIN_8X)
	
*/
/**************************************************************************/
void ADS1100::configure(uint8_t singleMode, uint8_t dataRate, uint8_t gain) {
	singleMode	&= ADS1100_CONVERSION_MASK;
	dataRate		&= ADS1100_DATA_RATE_MASK;
	gain			&= ADS1100_GAIN_MASK;
	currentRegister = singleMode+dataRate+gain;
	Wire.beginTransmission(ads1100_i2cAddress);
	#if ARDUINO >= 100
		Wire.write((uint8_t)currentRegister);
		#else
		Wire.send(currentRegister);
	#endif
	Wire.endTransmission();
}

/**************************************************************************/
/*
	Checks (i) if the conversion is completed and ii) reads the ADC value. 
	If the conversion is not completed yet, the ADC value is the old one.
*/
/**************************************************************************/
uint8_t ADS1100::conversionDone() { 	// check if conversion was completed. Reasonable only in the single conversion mode
	int8_t highbyte;
	uint8_t lowbyte,regbyte;
	Wire.requestFrom((uint8_t) ads1100_i2cAddress, (uint8_t) 3);
	while(Wire.available()) // ensure all the data comes in
	{   
		highbyte = Wire.read(); // high byte * B11111111
		lowbyte  = Wire.read(); // low byte
		regbyte  = Wire.read(); // register byte
	}
	value=(highbyte*256+lowbyte);
	regbyte &= ADS1100_START_SINGLE_CONVERSION;
	return (regbyte==0);
}   

/**************************************************************************/
/*
	Requests the unit to start the reading the ADC value. The reading actually takes the whole reading time.
*/
/**************************************************************************/
void ADS1100::startSingleConversion() {
	// start the conversion mode
	currentRegister=currentRegister|ADS1100_START_SINGLE_CONVERSION;
	Wire.beginTransmission(ads1100_i2cAddress);
	#if ARDUINO >= 100
		Wire.write((uint8_t) currentRegister);
		#else
		Wire.send(currentRegister);
		#endif
	Wire.endTransmission();
}
