/*
 * led.c
 *
 *  Created on: Jul 8, 2020
 *      Author: who_are_you
 */
#include "app/framework/include/af.h"
#include "led.h"


typedef struct
{
	 GPIO_Port_TypeDef   port;
	 unsigned int        pin;
}ledStruct;

//typedef struct
//{
//	ledStruct		ledArray[3];	// {{0,1},{0,1},{0,1}}
//}ledRGB;

ledStruct   ledRgbArray[LED_RGB_COUNT][LED_RGB_ELEMENT] = {LED_RGB_1,LED_RGB_2};
//ledRGB	ledRgbArray[LED_RGB_COUNT] = {LED_RGB_1,LED_RGB_2};
//ledRgbArray[0] = LED_RGB_1;
//ledRgbArray[1] =LED_RGB_2;

uint8_t ledRgbToggleColor;
uint8_t ledRgbToggleTime;
uint8_t ledRgbIndex;

EmberEventControl ledToggleEventControl;
void ledInit(void)
{
  CMU_ClockEnable(cmuClock_GPIO, true);
  for(int i=0; i<LED_RGB_COUNT; i++)
  {
	  for(int j=0; j<LED_RGB_ELEMENT; j++)
	  {
		  GPIO_PinModeSet(ledRgbArray[i][j].port,
				  	  	  ledRgbArray[i][j].pin,
						  gpioModePushPull,
						  0);
	  }
  }
  // offLed after Init
  offLed(LED_1);
  offLed(LED_2);
}

void toggleLed(ledNumber  ledIndex, ledColor color, uint8_t time)
{
	ledRgbIndex = ledIndex;
	ledRgbToggleColor = color;
	ledRgbToggleTime  = 2*time;
	emberEventControlSetActive(ledToggleEventControl);
}

void ledToggleEventHandle(void)
{
	emberEventControlSetInactive(ledToggleEventControl);
	for(int i=0; i<LED_RGB_ELEMENT; i++)
	{
		if( ((ledRgbToggleColor >> i) & 0x01) == 1)
		{
			GPIO_PinOutToggle(ledRgbArray[ledRgbIndex][i].port,ledRgbArray[ledRgbIndex][i].pin);
		}
		else
		{
			GPIO_PinOutSet(ledRgbArray[ledRgbIndex][i].port,ledRgbArray[ledRgbIndex][i].pin);
		}
	}
	ledRgbToggleTime --;
	if(ledRgbToggleTime !=0)
		emberEventControlSetDelayMS(ledToggleEventControl,LED_TOGGLE_TIME);
	else
		offLed(ledRgbIndex);
}

void testLed()
{
	for(int i=0; i<LED_RGB_COUNT; i++)
	{
		for(int j=0; j<LED_RGB_ELEMENT; j++)
		{
			GPIO_PinOutToggle(ledRgbArray[i][j].port,ledRgbArray[i][j].pin);
		}
	}
}

void offLed(ledNumber ledIndex)
{
	for(int j=0; j<LED_RGB_ELEMENT; j++)
	{
		GPIO_PinOutSet(ledRgbArray[ledIndex][j].port,ledRgbArray[ledIndex][j].pin);
	}
}


