/*
 * Si7013.c
 *
 *  Created on: Oct 1, 2018
 *      Author: LongHD
 */

/******************************************************************************/

/******************************************************************************/
/*                              INCLUDE FILES                                 */
/******************************************************************************/

#include "app/framework/include/af.h"
#include "stdbool.h"
#include "em_i2c.h"
//#include "Source/Utility/Debug.h"
#include "Source/Hard/I2C/i2c.h"
#include "si7020.h"

/******************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                         */
/******************************************************************************/



/******************************************************************************/
/*                              PRIVATE DATA                                  */
/******************************************************************************/



/******************************************************************************/
/*                              EXPORTED DATA                                 */
/******************************************************************************/



/******************************************************************************/
/*                            FUNCTIONS                              */
/******************************************************************************/



/******************************************************************************/


boolean Si7020_CheckInit (void){
	I2C_TransferSeq_TypeDef    seq;
	I2C_TransferReturn_TypeDef ret;
	uint8_t                    i2c_read_data[3];
	uint8_t                    i2c_write_data[2];

	seq.addr  = SI7020_ADDR;
	seq.flags = I2C_FLAG_WRITE_READ;
	/* Select command to issue */
	i2c_write_data[0] = SI7020_READ_ID_1;
	i2c_write_data[1] = SI7020_READ_ID_2;
	seq.buf[0].data   = i2c_write_data;
	seq.buf[0].len    = 2;
	/* Select location/length of data to be read */
	seq.buf[1].data = i2c_read_data;
	seq.buf[1].len  = 3;

	ret = I2C_TransferInit(I2C0, &seq);
	// Sending data
	while (ret == i2cTransferInProgress)
	{
		ret = I2C_Transfer(I2C0);
	}
	if ((ret != i2cTransferDone) | (i2c_read_data[0] != SI7020_DEVICE_ID)) {

		emberAfCorePrintln (" Detect Si7020 failure!--Device Id:%d ",i2c_read_data[0]);
		return false;
	}

	emberAfCorePrintln (" Detect Si7020 success! ");
	return ret;
}

static boolean Si7020_Measure (uint32_t *buffer, uint8_t command){
	I2C_TransferSeq_TypeDef    seq;
	I2C_TransferReturn_TypeDef retVal;
	uint8_t                    i2c_read_data[2];
	uint8_t                    i2c_write_data[1];

	seq.addr  = SI7020_ADDR;
	seq.flags = I2C_FLAG_WRITE_READ;
	/* Select command to issue */
	i2c_write_data[0] = command;
	seq.buf[0].data   = i2c_write_data;
	seq.buf[0].len    = 1;
	/* Select location/length of data to be read */
	seq.buf[1].data = i2c_read_data;
	seq.buf[1].len  = 2;

	retVal = I2C_TransferInit(I2C0, &seq);
	// Sending data
	while (retVal == i2cTransferInProgress)
	{
		retVal = I2C_Transfer(I2C0);
	}

	*buffer = ((uint32_t) i2c_read_data[0] << 8) + (i2c_read_data[1] & 0xfc);

	return true;
}

boolean Si7020_MeasureTempAndHumi (uint32_t* humiData, uint32_t* tempData){
	boolean retVal = Si7020_Measure (humiData, SI7020_READ_RH);

	if (retVal)
		*humiData = (((*humiData) * 15625L) >> 13) - 6000;
	else
		return false;

	retVal = Si7020_Measure ((uint32_t*) tempData, SI7020_READ_TEMP);

	if (retVal)
		*tempData = (((*tempData) * 21965) >> 13) - 46850;
	else
		return false;

	return true;

}

































