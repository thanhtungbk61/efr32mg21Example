/*
 * adc.c
 *
 *  Created on: Jul 10, 2020
 *      Author: who_are_you
 */
#include "adc.h"


void adcInit(void)
{
  // Declare init structs
  IADC_Init_t init = IADC_INIT_DEFAULT;
  IADC_AllConfigs_t initAllConfigs = IADC_ALLCONFIGS_DEFAULT;
  IADC_InitSingle_t initSingle = IADC_INITSINGLE_DEFAULT;
  IADC_SingleInput_t initSingleInput = IADC_SINGLEINPUT_DEFAULT;

  // Reset IADC to reset configuration in case it has been modified
  IADC_reset(IADC0);

  // Configure IADC clock source for use while in EM2
  // Note that HFRCOEM23 is the lowest frequency source available for the IADC
  CMU_ClockSelectSet(cmuClock_IADCCLK, cmuSelect_HFRCOEM23);

  // Modify init structs and initialize
  init.warmup = iadcWarmupKeepWarm;

  // Set the HFSCLK prescale value here
  init.srcClkPrescale = IADC_calcSrcClkPrescale(IADC0, CLK_SRC_ADC_FREQ, 0);

  // Configuration 0 is used by both scan and single conversions by default
  // Use unbuffered AVDD as reference
  initAllConfigs.configs[0].reference = iadcCfgReferenceVddx;

  // Divides CLK_SRC_ADC to set the CLK_ADC frequency for desired sample rate
  initAllConfigs.configs[0].adcClkPrescale = IADC_calcAdcClkPrescale(IADC0,
                                                                    CLK_ADC_FREQ,
                                                                    0,
                                                                    iadcCfgModeNormal,
                                                                    init.srcClkPrescale);

  // Single initialization
  //initSingle.dataValidLevel = _IADC_SCANFIFOCFG_DVL_VALID1;

  // Set conversions to run continuously
  //initSingle.triggerAction = iadcTriggerActionContinuous;

  // Configure Input sources for single ended conversion
  initSingleInput.posInput = iadcPosInputPortDPin4;
  initSingleInput.negInput = iadcNegInputGnd;

  // Initialize IADC
  IADC_init(IADC0, &init, &initAllConfigs);

  // Initialize Scan
  IADC_initSingle(IADC0, &initSingle, &initSingleInput);

  // Allocate the analog bus for ADC0 inputs
  GPIO->IADC_INPUT_BUS |= IADC_INPUT_BUSALLOC;
  }

  // Enable interrupts on data valid level
  //IADC_enableInt(IADC0, IADC_IF_SINGLEFIFODVL);

  // Enable ADC interrupts
 // NVIC_ClearPendingIRQ(IADC_IRQn);
  //NVIC_EnableIRQ(IADC_IRQn);

//void readADC(uint32_t* data)
//{
////	IADC_Result_t iadcResult;
//	IADC_command(IADC0, iadcCmdStartSingle);
//	// Read data from the FIFO
//    while(!(IADC0->STATUS & (_IADC_STATUS_CONVERTING_MASK \
//            				| _IADC_SINGLEDATA_MASK))); // _IADC_SINGLEDATA_MASK   _IADC_STATUS_SINGLEFIFODV_MASK
//    *data = IADC_readSingleData(IADC0);
////    *data = iadcResult.data;
//
//}
