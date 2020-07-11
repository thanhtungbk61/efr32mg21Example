/*
 * main.c
 *
 *  Created on: Jul 8, 2020
 *      Author: who_are_you
 */
#include "app/framework/include/af.h"
#include EMBER_AF_API_NETWORK_STEERING

// include user lib
#include "Source/Hard/Button/button.h"
#include "Source/Hard/Led/led.h"
#include "Source/Hard/Si7020/si7020.h"
#include "Source/Hard/I2C/i2c.h"
#include "Source/Hard/ADC/adc.h"

// Event control struct declaration
EmberEventControl myDelayEventControl;
// Event function forward declaration
void myDelayEventHandle(void);


void buttonPressAndHoldCallbackHandle(buttonNumber button, uint8_t pressTime, uint8_t holdTime);
void buttonHoldCallbackIndicate(buttonNumber button, uint8_t holdTime);

// main init
void emberAfMainInitCallback(void)
{
	bool si7020Check;
	//
	buttonInit();
	ledInit();
	i2cInit();
	adcInit();
	//
	si7020Check = Si7020_CheckInit();

	buttonPressTimeCallbackInit(buttonPressAndHoldCallbackHandle);
	buttonHoldCallbackInit(buttonHoldCallbackIndicate);
	emberAfCorePrintln("emberAfMainInitCallback---si7020Check:%d",si7020Check);

//	toggleLed(LED_1,ledBlue,3);
	toggleLed(LED_2,ledBlue,3);
}


// Event function stub
void myDelayEventHandle(void)
{

}

void buttonPressAndHoldCallbackHandle(buttonNumber button, uint8_t pressTime, uint8_t holdTime)
{
	uint32_t humi, temp;
	uint32_t adcRead;
	if(holdTime > HOLD_TIME_MIN)
	{
		emberAfCorePrintln("buttonHoldTimeCallbackHandle--buttonNumber:%d----holdTime:%d",button,holdTime);
	}
	else
	{
		emberAfCorePrintln("buttonPressTimeCallbackHandle--buttonNumber:%d----pressTime:%d",button,pressTime);
		switch (pressTime)
		{
			case press_1:
				Si7020_MeasureTempAndHumi(&humi, &temp);
				emberAfCorePrintln("humi:%d---temp:%d",humi,temp);
				toggleLed(LED_2,ledOff,1);
				break;
			case press_2:
//				readADC(&adcRead);
				emberAfCorePrintln("ADC--READ:%d",adcRead);
				toggleLed(LED_2,ledGreen,2);
				break;
			case press_3:
				toggleLed(LED_2,ledRed,2);
				break;
			case press_4:
				toggleLed(LED_2,ledBlue,2);
		}
	}


}

void buttonHoldCallbackIndicate(buttonNumber button, uint8_t holdTime)
{
//	offLed();
	emberAfCorePrintln("buttonHoldCallbackHandle--buttonNumber:%d----holdTime:%d",button,holdTime);
}

