/*
 * i2c.c
 *
 *  Created on: Jul 9, 2020
 *      Author: who_are_you
 */
#include "i2c.h"

void i2cInit(void)
{
  // Using default settings
  I2C_Init_TypeDef i2cInit = I2C_INIT_DEFAULT;

  // Using PB1 (SDA) and PB0 (SCL)
  GPIO_PinModeSet(gpioPortB, 1, gpioModeWiredAndPullUpFilter, 1);
  GPIO_PinModeSet(gpioPortB, 0, gpioModeWiredAndPullUpFilter, 1);

	/* In some situations, after a reset during an I2C transfer, the slave
	 device may be left in an unknown state. Send 9 clock pulses to
	 set slave in a defined state. */

  for (uint8_t i = 0; i < 9; i++)
  {
	  GPIO_PinOutSet(gpioPortB, 1);
	  GPIO_PinOutClear(gpioPortB, 0);
  }
  // Route GPIO pins to I2C module
  GPIO->I2CROUTE[0].SDAROUTE = (GPIO->I2CROUTE[0].SDAROUTE & ~_GPIO_I2C_SDAROUTE_MASK)
                        | (gpioPortB << _GPIO_I2C_SDAROUTE_PORT_SHIFT
                        | (1 << _GPIO_I2C_SDAROUTE_PIN_SHIFT));
  GPIO->I2CROUTE[0].SCLROUTE = (GPIO->I2CROUTE[0].SCLROUTE & ~_GPIO_I2C_SCLROUTE_MASK)
                        | (gpioPortB << _GPIO_I2C_SCLROUTE_PORT_SHIFT
                        | (0 << _GPIO_I2C_SCLROUTE_PIN_SHIFT));
  GPIO->I2CROUTE[0].ROUTEEN = GPIO_I2C_ROUTEEN_SDAPEN | GPIO_I2C_ROUTEEN_SCLPEN;

  // Initializing the I2C
  i2cInit.enable = true;
  i2cInit.master = true;

  i2cInit.freq    = I2C_FREQ_STANDARD_MAX;
  i2cInit.refFreq = 0;
  i2cInit.clhr    = i2cClockHLRStandard;
  I2C_Init(I2C0, &i2cInit);

  // Setting the status flags and index
//  i2c_startTx = false;

  I2C0->CTRL = I2C_CTRL_AUTOSN;
}


