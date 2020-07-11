/*
 * button.h
 *
 *  Created on: Jul 8, 2020
 *      Author: who_are_you
 */
#include "stddef.h"
#include "em_cmu.h"
#include "em_gpio.h"
#include "gpiointerrupt.h"

#ifndef SOURCE_HARD_BUTTON_BUTTON_H_
#define SOURCE_HARD_BUTTON_BUTTON_H_


#define RELEASE						1
#define PRESS						0
#define BUTTON_COUNT				2
#define GPIO_DOUT					0
#define BUTTON_DEBOUNCE				5
#define BUTTON_CHECK_HOLD_MS		200
#define BUTTON_RELEASE_DELAY_MS		500

#define BUTTON_1_PORT				gpioPortD
#define BUTTON_1_PIN				(4U)

#define BUTTON_2_PORT				gpioPortD
#define BUTTON_2_PIN				(3U)

#define BUTTON_INIT					{{BUTTON_1_PORT,BUTTON_1_PIN},{BUTTON_2_PORT,BUTTON_2_PIN}}

#define HOLD_TIME_MIN				5
typedef enum
{
	hold_1s	= 5,
	hold_2s	= 10,
	hold_3s	= 15,
	hold_4s	= 20,
	hold_5s = 25
}holdHoldTime;

typedef enum
{
	press_1	= 1,
	press_2 = 2,
	press_3 = 3,
	press_4 = 4
}buttonPressTime;
typedef enum
{
	BUTTON_0 = 0,
	BUTTON_1 = 1
}buttonNumber;

typedef void (*buttonHoldCallbackIndicateFunction) (buttonNumber, uint8_t);
typedef void (*buttonPressAndHoldCallbackFunction) (buttonNumber, uint8_t, uint8_t);

void buttonInit();
void buttonHoldCallbackInit(buttonHoldCallbackIndicateFunction holdCallbackInit);
void buttonPressTimeCallbackInit(buttonPressAndHoldCallbackFunction pressTimeCallbackInit);

#endif /* SOURCE_HARD_BUTTON_BUTTON_H_ */
