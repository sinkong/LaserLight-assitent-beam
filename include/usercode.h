/***********************
 *  Encoding UTF-8
 *  COPYRIGHT NOTICE 
 *  Copyright (c) 2019, XinYu Lighting Co.,Ltd.
 *  @file       usercode.h
 *  @brief      All of the usually way to operate 
 *              S32K144  
 *  @version    v1.0
 *  @author     
 *  @date       2019/10/28
 */

#ifndef USERCODE_H
#define USERCODE_H

#include "Cpu.h"
#include <stdbool.h>
#include <stdint.h>

/* CAN Mailbox*/
#define LLC1_MAILBOX (0UL)
#define LLS1_MAILBOX (1UL)
/* CAN Msg ID define*/
#define LLC1_MSG_ID (0x100UL)
#define LLS1_MSG_ID (0x101UL)

typedef enum{
    LASER_OFF,
    LASER_ON
}laserstate_t;


;
// signal for this project

typedef struct{
	uint8_t laserCtl;
	uint16_t voltThresholdVal;
	uint16_t blueThresholdVal;
	uint16_t tempThresholdVal;
} *ptrLLC1; 

typedef struct{
	uint8_t laserStatus;
	uint16_t blueMeasuredVal;
	uint16_t tempMeasuredVal;
	uint16_t voltMeasuredVal;
} *ptrLLS1;


// can messages 
can_message_t LLC1_Rx_Msg; 
can_message_t LLS1_Tx_Msg;

void lptmrISR(void);
void setTask(void);
void CAN_ISR(uint32_t instance, can_event_t eventType,
			 uint32_t objIdx, void *driveState);
void adc1_callback00(const adc_callback_info_t *const callbackInfo, void *userData);
void adc1_callback01(const adc_callback_info_t *const callbackInfo, void *userData);
void adc1_callback02(const adc_callback_info_t *const callbackInfo, void *userData);
void adc1_callback03(const adc_callback_info_t *const callbackInfo, void *userData);
#endif
