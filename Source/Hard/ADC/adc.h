/*
 * adc.h
 *
 *  Created on: Jul 10, 2020
 *      Author: who_are_you
 */

#ifndef SOURCE_HARD_ADC_ADC_H_
#define SOURCE_HARD_ADC_ADC_H_

#include "em_device.h"
#include "em_chip.h"
#include "em_cmu.h"
#include "em_adc.h"
#include "em_iadc.h"
#include "em_bus.h"
/*******************************************************************************
 *******************************   DEFINES   ***********************************
 ******************************************************************************/

// Set HFRCOEM23 to lowest frequency (1MHz)
#define HFRCOEM23_FREQ          cmuHFRCOEM23Freq_1M0Hz

// Set CLK_ADC to 10kHz (this corresponds to a sample rate of 1ksps)
#define CLK_SRC_ADC_FREQ        1000000 // CLK_SRC_ADC
#define CLK_ADC_FREQ            10000   // CLK_ADC

// When changing GPIO port/pins above, make sure to change xBUSALLOC macro's
// accordingly.
#define IADC_INPUT_BUS          CDBUSALLOC
#define IADC_INPUT_BUSALLOC     GPIO_CDBUSALLOC_CDEVEN0_ADC0


void adcInit(void);
//void readADC(uint32_t* data);
#endif /* SOURCE_HARD_ADC_ADC_H_ */
