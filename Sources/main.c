/* ###################################################################
**     Filename    : main.c
**     Processor   : S32K1xx
**     Abstract    :
**         Main module.
**         This module contains user's application code.
**     Settings    :
**     Contents    :
**         No public methods
**
** ###################################################################*/
/*!
** @file main.c
** @version 01.00
** @brief
**         Main module.
**         This module contains user's application code.
*/
/*!
**  @addtogroup main_module main module documentation
**  @{
*/
/* MODULE main */

/* Including necessary module. Cpu.h contains other modules needed for compiling.*/
#include "Cpu.h"
#include <stdint.h>
#include <stdbool.h>
#include "usercode.h"

#include "task.h"

volatile int exit_code = 0;
extern can_buff_config_t can0_bufCfg;
extern can_buff_config_t can0_Rx_bufCfg;
extern bool g_laserCtl;


/*! 
  \brief The main function for the project.
  \details The startup initialization sequence is the following:
 * - startup asm routine
 * - main()
*/
int main(void)
{
/* Write your local variable definition here */

/*** Processor Expert internal initialization. DON'T REMOVE THIS CODE!!! ***/
#ifdef PEX_RTOS_INIT
  PEX_RTOS_INIT(); /* Initialization of the selected RTOS. Macro is defined by the RTOS component. */
#endif
  /*** End of Processor Expert internal initialization.                    ***/

  /* Write your code here */
  /* For example: for(;;) { } */

// General initialization
  CLOCK_SYS_Init(g_clockManConfigsArr, CLOCK_MANAGER_CONFIG_CNT,
                 g_clockManCallbacksArr, CLOCK_MANAGER_CALLBACK_CNT);
  CLOCK_SYS_UpdateConfiguration(0u, CLOCK_MANAGER_POLICY_FORCIBLE);
  PINS_DRV_Init(NUM_OF_CONFIGURED_PINS, g_pin_mux_InitConfigArr);

  PWM_Init(&pwm_pal1Instance, &pwm_pal1Configs);

  LPTMR_DRV_Init(INST_LPTMR1, &lpTmr1_config0, false);
  INT_SYS_InstallHandler(LPTMR0_IRQn, &lptmrISR, NULL);
  INT_SYS_EnableIRQ(LPTMR0_IRQn);


  ADC_Init(&adc_pal1_instance, &adc_pal1_InitConfig0);
  for(int i = 0; i < 4; i++)
  {
    status_t status = ADC_StartGroupConversion(&adc_pal1_instance, 0);
    if(status);
    else;
    if(i == 0)
    {
      DEV_ASSERT(STATUS_SUCCESS == status);
    }
    else
    {
      DEV_ASSERT(STATUS_BUSY == status);
    }
  }

//  CAN bus initialization
  PINS_DRV_ClearPins(PTE, 1 << 6u);
  CAN_Init(&can_pal1_instance, &can_pal1_Config0);

  CAN_ConfigRxBuff(&can_pal1_instance, LLC1_MAILBOX,
                   &can0_bufCfg, LLC1_MSG_ID);
  CAN_ConfigTxBuff(&can_pal1_instance, LLS1_MAILBOX, &can0_bufCfg);
  CAN_InstallEventCallback(&can_pal1_instance, CAN_ISR, NULL);
  CAN_Receive(&can_pal1_instance, LLC1_MAILBOX, &LLC1_Rx_Msg);

  LPTMR_DRV_StartCounter(INST_LPTMR1);

  setTask();

  while(1)
  {
	  taskSchedule();
  }

/*** Don't write any code pass this line, or it will be deleted during code generation. ***/
  /*** RTOS startup code. Macro PEX_RTOS_START is defined by the RTOS component. DON'T MODIFY THIS CODE!!! ***/
  #ifdef PEX_RTOS_START
    PEX_RTOS_START();                  /* Startup of the selected RTOS. Macro is defined by the RTOS component. */
  #endif
  /*** End of RTOS startup code.  ***/
  /*** Processor Expert end of main routine. DON'T MODIFY THIS CODE!!! ***/
  for(;;) {
    if(exit_code != 0) {
      break;
    }
  }
  return exit_code;
  /*** Processor Expert end of main routine. DON'T WRITE CODE BELOW!!! ***/
} /*** End of main routine. DO NOT MODIFY THIS TEXT!!! ***/

/* END main */
/*!
** @}
*/
/*
** ###################################################################
**
**     This file was created by Processor Expert 10.1 [05.21]
**     for the NXP S32K series of microcontrollers.
**
** ###################################################################
*/
