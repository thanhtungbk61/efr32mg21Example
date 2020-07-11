/*
 * Si7013.h
 *
 *  Created on: Oct 1, 2018
 *      Author: LongHD
 */

#ifndef SOURCE_HARD_SI7020_SI7020_H_
#define SOURCE_HARD_SI7020_SI7020_H_

/******************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                         */
/******************************************************************************/

/* I2C device address for Si7020 */
#define SI7020_ADDR            0x80

/* Device ID value for Si7020 */
#define SI7020_DEVICE_ID       0x14

/* Si7013 Read Temperature Command */
#define SI7020_READ_TEMP       0xE0

/* Si7013 Read RH Command */
#define SI7020_READ_RH         0xE5

/* SI7013 ID */
#define SI7020_READ_ID_1       0xFC
#define SI7020_READ_ID_2       0xC9

/******************************************************************************/
/*                              PRIVATE DATA                                  */
/******************************************************************************/



/******************************************************************************/
/*                              EXPORTED DATA                                 */
/******************************************************************************/



/******************************************************************************/
/*                            FUNCTIONS                              */
/******************************************************************************/

boolean Si7020_CheckInit (void);
boolean Si7020_MeasureTempAndHumi (uint32_t* humiData, uint32_t* tempData);

/******************************************************************************/

#endif /* SOURCE_HARD_SI7020_SI7020_H_ */
