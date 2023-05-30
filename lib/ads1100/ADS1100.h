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

#if ARDUINO >= 100
	#include "Arduino.h"
	#else
	#include "WProgram.h"
#endif

#include <Wire.h>

/**************************************************************************
	I2C ADDRESS/BITS
**************************************************************************/
	#define ADS1100_DEFAULT_ADDRESS                       (0x48)    

/**************************************************************************
	CONFIGURATION REGISTER
**************************************************************************/
	#define ADS1100_START_SINGLE_CONVERSION				(0x80)	// In single conversion mode, writing causes a conversion to start, and writing a 0 has no effect. No effect in continuous
	
	#define ADS1100_CONVERSION_MASK							(0x10)	// single conversion mode. IF not set - continuous conversion mode
	#define ADS1100_CONVERSION_SINGLE						(0x10)	// single conversion mode. IF not set - continuous conversion mode
	#define ADS1100_CONVERSION_CONTINOUS					(0x00)	// single conversion mode. IF not set - continuous conversion mode
	
	#define ADS1100_DATA_RATE_MASK							(0x0C)	// ADS1100’s data rate 128 - 8 SPS. SPS = samples per second
	#define ADS1100_DATA_RATE_128SPS							(0x00)
	#define ADS1100_DATA_RATE_32SPS							(0x04)
	#define ADS1100_DATA_RATE_16SPS							(0x08)
	#define ADS1100_DATA_RATE_8SPS							(0x0C)
	
	#define ADS1100_GAIN_MASK									(0x03)	// ADS1100’s gain 1x to 8x
	#define ADS1100_GAIN_1X										(0x00)
	#define ADS1100_GAIN_2X										(0x01)
	#define ADS1100_GAIN_4X										(0x02)
	#define ADS1100_GAIN_8X										(0x03)


class ADS1100
{
	public:
		ADS1100(byte address);
		int16_t	value;									// value of the ADC returned
		void		begin(TwoWire *wire);
		int16_t	getvalue(void);							// reading the value regardless of the fact it is available
		void		configure(uint8_t singleMode, uint8_t dataRate, uint8_t gain);
		uint8_t	conversionDone();
		void		startSingleConversion();
	private:
		uint8_t	ads1100_i2cAddress;					// I2C address of the unit
		uint8_t	currentRegister=B00001100;			// the last register used
		TwoWire* m_wire;
};
