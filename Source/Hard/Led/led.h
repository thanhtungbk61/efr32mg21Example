/*
 * led.h
 *
 *  Created on: Jul 8, 2020
 *      Author: who_are_you
 */

#include "stddef.h"
#include "em_cmu.h"
#include "em_gpio.h"


#ifndef SOURCE_HARD_LED_LED_H_
#define SOURCE_HARD_LED_LED_H_


#define LED_RGB_COUNT			2
#define LED_RGB_ELEMENT			3
#define LED_TOGGLE_TIME			500

#define LED_PORT_1				gpioPortA
#define LED_BLUE_PIN_1			(0U)
#define LED_GREEN_PIN_1			(3U)
#define LED_RED_PIN_1			(4U)

#define LED_PORT_2				gpioPortD
#define LED_BLUE_PIN_2			(2U)
#define LED_GREEN_PIN_2			(1U)
#define LED_RED_PIN_2			(0U)

//#define LED_RGB_1				{{ {{LED_PORT_1,LED_BLUE_PIN_1}} , {{LED_PORT_1,LED_GREEN_PIN_1}} , {{LED_PORT_1,LED_RED_PIN_1}} }}
//#define LED_RGB_2				{{ {{LED_PORT_2,LED_BLUE_PIN_2}} , {{LED_PORT_2,LED_GREEN_PIN_2}} , {{LED_PORT_2,LED_RED_PIN_2}} }}

#define LED_RGB_1				{ {LED_PORT_1,LED_BLUE_PIN_1} , {LED_PORT_1,LED_GREEN_PIN_1} , {LED_PORT_1,LED_RED_PIN_1} }
#define LED_RGB_2				{ {LED_PORT_2,LED_BLUE_PIN_2} , {LED_PORT_2,LED_GREEN_PIN_2} , {LED_PORT_2,LED_RED_PIN_2} }

typedef enum
{
	ledOff		= 0x000,
	ledBlue		= BIT(0),
	ledGreen	= BIT(1),
	ledRed		= BIT(2),
	ledPink	    = BIT(0) | BIT(2), // ledBlue + ledRed
	ledYellow	= BIT(1) | BIT(2)  // ledGreen +ledRed
}ledColor;

typedef enum
{
	LED_1,
	LED_2
}ledNumber;
void ledInit(void);
void toggleLed(ledNumber  ledIndex, ledColor color, uint8_t time);
void testLed();
void offLed(ledNumber ledIndex);
#endif /* SOURCE_HARD_LED_LED_H_ */
