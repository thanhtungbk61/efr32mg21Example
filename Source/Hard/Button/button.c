/*
 * button.c
 *
 *  Created on: Jul 8, 2020
 *      Author: who_are_you
 */
#include "app/framework/include/af.h"
#include "button.h"




typedef struct {
  GPIO_Port_TypeDef		port;
  unsigned int			pin;
  bool					state;
  uint8_t				pressCount;
  uint8_t				holdTime;
  bool					release;
  bool					press;
} buttonArray_t;


buttonHoldCallbackIndicateFunction holdCallbackFunc = NULL;
buttonPressAndHoldCallbackFunction pressTimeCallbackFunc = NULL;

buttonArray_t buttonArray[BUTTON_COUNT] = {{BUTTON_1_PORT,BUTTON_1_PIN,0,0,0,0},{BUTTON_2_PORT,BUTTON_2_PIN,0,0,0,0}};

// Event control struct declaration
EmberEventControl buttonPressAndHoldEventControl;
EmberEventControl buttonReleaseEventControl;

void buttonPressAndHoldEventHandle(void);
void buttonReleaseEventHandle(void);

static void halInternalButtonIsr(uint8_t pin);
static uint8_t getButtonIndex(uint8_t pin);
static void resetButtonParameter(uint8_t index);

static void resetButtonParameter(uint8_t index)
{
	buttonArray[index].holdTime 	= 0;
	buttonArray[index].pressCount	= 0;
	buttonArray[index].press		= false;
	buttonArray[index].release		= false;
	buttonArray[index].state		= 0;
}
static uint8_t getButtonIndex(uint8_t pin)
{
	for(int i=0; i<BUTTON_COUNT; i++)
	{
		if(buttonArray[i].pin == pin)
			return i;
	}
	return -1;
}
void buttonInit(void)
{
  GPIOINT_Init();
  CMU_ClockEnable(cmuClock_GPIO, true);
  uint8_t i;
  for ( i = 0; i < BUTTON_COUNT; i++ ) {
    /* Configure pin as input */
    GPIO_PinModeSet(buttonArray[i].port,
                    buttonArray[i].pin,
					gpioModeInput,
					GPIO_DOUT);
    /* Register callbacks before setting up and enabling pin interrupt. */
    GPIOINT_CallbackRegister(buttonArray[i].pin,
                             halInternalButtonIsr);
    /* Set rising and falling edge interrupts */
    GPIO_ExtIntConfig(buttonArray[i].port,
                      buttonArray[i].pin,
                      buttonArray[i].pin,
                      true,
                      true,
                      true);
  }
}

void halInternalButtonIsr(uint8_t pin)
{
  uint8_t buttonStateNow;
  uint8_t buttonStatePrev;
  uint32_t debounce;
  uint8_t buttonIndex;

  buttonIndex = getButtonIndex(pin);
  // check valid index
  if(buttonIndex==-1)
	  return;

  buttonStateNow = GPIO_PinInGet(buttonArray[buttonIndex].port, buttonArray[buttonIndex].pin);
  for ( debounce = 0;
        debounce < BUTTON_DEBOUNCE;
        debounce = (buttonStateNow == buttonStatePrev) ? debounce + 1 : 0 ) {
    buttonStatePrev = buttonStateNow;
    buttonStateNow = GPIO_PinInGet(buttonArray[buttonIndex].port, buttonArray[buttonIndex].pin);
  }

  // update state
  buttonArray[buttonIndex].state = buttonStateNow;

  if(buttonStateNow == PRESS)
  {
	  buttonArray[buttonIndex].pressCount ++;
	  if(buttonArray[buttonIndex].press != true)
		  emberEventControlSetActive(buttonPressAndHoldEventControl);
	  buttonArray[buttonIndex].press = true;
	  buttonArray[buttonIndex].release = false;
  }
  else
  {
	  buttonArray[buttonIndex].release = true;
	  buttonArray[buttonIndex].press = false;
	  emberEventControlSetDelayMS(buttonReleaseEventControl,BUTTON_RELEASE_DELAY_MS);
  }

}


void buttonPressAndHoldEventHandle(void)
{
	emberEventControlSetInactive(buttonPressAndHoldEventControl);
	bool holdTrigger =false;
	for(int i=0; i<BUTTON_COUNT; i++)
	{
		if(buttonArray[i].press ==true)
		{
			holdTrigger = true;
			buttonArray[i].holdTime ++;
			if(holdCallbackFunc != NULL)
			{
				switch(buttonArray[i].holdTime)
				{
					case hold_1s:
						holdCallbackFunc(i,hold_1s);
						break;
					case hold_2s:
						holdCallbackFunc(i,hold_2s);
						break;
					case hold_3s:
						holdCallbackFunc(i,hold_3s);
						break;
					case hold_4s:
						holdCallbackFunc(i,hold_4s);
						break;
					case hold_5s:
						holdCallbackFunc(i,hold_5s);
						break;
					default:
						break;
				}
			}
		}
	}
	if(holdTrigger == true)
		emberEventControlSetDelayMS(buttonPressAndHoldEventControl,BUTTON_CHECK_HOLD_MS);
}

void buttonReleaseEventHandle(void)
{
	emberEventControlSetInactive(buttonReleaseEventControl);
	for(int i=0; i<BUTTON_COUNT; i++)
	{
		if(buttonArray[i].release == true)
		{
			if(pressTimeCallbackFunc != NULL)
				pressTimeCallbackFunc(i, buttonArray[i].pressCount, buttonArray[i].holdTime);
			resetButtonParameter(i);
		}
	}
}

void buttonHoldCallbackInit(buttonHoldCallbackIndicateFunction holdCallbackInit)
{
	if(holdCallbackInit != NULL)
	{
		holdCallbackFunc = holdCallbackInit;
	}
}
void buttonPressTimeCallbackInit(buttonPressAndHoldCallbackFunction pressTimeCallbackInit)
{
	if(pressTimeCallbackInit != NULL)
	{
		pressTimeCallbackFunc = pressTimeCallbackInit;
	}
}
