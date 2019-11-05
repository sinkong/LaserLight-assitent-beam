/***********************
 *  Encoding UTF-8
 *  COPYRIGHT NOTICE 
 *  Copyright (c) 2019, XinYu Lighting Co.,Ltd.
 *  @file       usercode.c
 *  @brief      All of the usually way to operate 
 *              S32K144  
 *  @version    v1.0
 *  @author     EricXQ 
 *  @date       2019/10/28
 */

// Including

#include "Cpu.h"
#include "pin_mux.h"
#include "usercode.h"
#include <string.h>
#include <stdbool.h>

#include "task.h"

// Global variables
//Definition of variables
uint8_t CAN_RCV_Flag = 0;
// Global variable start
// for LLC1
uint16_t g_voltThresholdValH = 72;
uint16_t g_voltThresholdValL = 141;
uint16_t g_blueThresholdVal = 229;
uint16_t g_tempThresholdValH = 237;
uint16_t g_tempThresholdValL = 13;
bool groupConvDone = false;

// for LLS1
uint16_t g_blueVal = 0;
uint16_t g_ambientVal = 0;
uint16_t g_tempVal = 0;
uint16_t g_voltVal = 0;

// Global variable end

// config can buf, and set the messages
can_buff_config_t can0_bufCfg = {
    .enableFD = true,
    .enableBRS = true,
    .fdPadding = 0UL,
    .idType = CAN_MSG_ID_STD,
    .isRemote = false};

// message data domain
uint8_t LLC1_BUF[7] = {0};
ptrLLC1 LLC1_DATA = (ptrLLC1)LLC1_BUF;

uint8_t LLS1_BUF[7] = {0};
ptrLLS1 LLS1_DATA = (ptrLLS1)LLS1_BUF;

/***********************************************************************
 *  Function : void lptmrISR(void) 
 * @brief LPTMR0 IRQ handler provides 20ms intervals (50fps)   
 * @param none
 * @return none
 ***********************************************************************/
void lptmrISR(void)
{
    //    static uint32_t interruptCount = 0UL;

    /* Clear compare flag */
    LPTMR_DRV_ClearCompareFlag(INST_LPTMR1);

    tick1msTaskRoutines(); //task

    /*****  LIN BUS *****/
    // /* Timer Interrupt Handler */
    // lin_lld_timeout_service(LI0);
    // /* If 5 ms have passed, provide the required tick */
    // interruptCount++;
    // if (interruptCount == 5)
    // {
    //     l_sch_tick(LI0);
    //     interruptCount = 0UL;
    // }
    // /* Increment overflow count */
    // timerOverflowInterruptCount++;
    /*****  LIN BUS END*****/
}
void CAN_RCV_Process(void)
{
    switch (LLC1_Rx_Msg.id)
    {
    case LLC1_MSG_ID:
        memcpy(LLC1_DATA, (LLC1_Rx_Msg.data), 7);
        if (LLC1_DATA->laserCtl == 0x00)
            PINS_DRV_ClearPins(PTE, 1 << 9);
        if (LLC1_DATA->laserCtl == 0x01)
            PINS_DRV_SetPins(PTE, 1 << 9);
        g_tempThresholdValH = LLC1_DATA->tempThresholdVal;
        g_voltThresholdValH = LLC1_DATA->voltThresholdVal;
        g_blueThresholdVal = LLC1_DATA->blueThresholdVal;
        break;

    default:
        break;
    }
};

/***********************************************************************
 *  Function:   CAN_ISR 
 *  @brief:   CAN interrupt service routines
 *  @param instance     :   can instance
 *  @param eventType    :   event type 
 *  @param objIdx       :   object index
 *  @param driveState   :   
 ***********************************************************************/
void CAN_ISR(uint32_t instance, can_event_t eventType,
             uint32_t objIdx, void *driveState)
{
    if (instance == can_pal1_instance.instIdx)
    {
        switch (objIdx)
        {
        case LLC1_MAILBOX:
            while (CAN_Receive(&can_pal1_instance, LLC1_MAILBOX, &LLC1_Rx_Msg) != STATUS_SUCCESS);
            CAN_RCV_Process();
            break;
        default:
            break;
        }
    }
}

void adc1_callback00(const adc_callback_info_t *const callbackInfo, void *userData)
{
    uint32_t sum = 0;
    (void)userData;
    (void)callbackInfo;
    for(int i = 0; i < 5; i++)
    {
        sum += adc_pal1_Results00[i];
    }
    g_blueVal = sum / 5; 
    ADC_StartGroupConversion(&adc_pal1_instance, 1UL);
}

void adc1_callback01(const adc_callback_info_t *const callbackInfo, void *userData)
{
    uint32_t sum = 0;
    (void)userData;
    (void)callbackInfo;
    for(int i = 0; i < 5; i++)
    {
        sum += adc_pal1_Results01[i];
    }
    g_ambientVal = sum / 5; 
    ADC_StartGroupConversion(&adc_pal1_instance, 2UL);
}
void adc1_callback02(const adc_callback_info_t *const callbackInfo, void *userData)
{
    uint32_t sum = 0;
    (void)userData;
    (void)callbackInfo;
    for(int i = 0; i < 5; i++)
    {
        sum += adc_pal1_Results02[i];
    }
    g_tempVal  = sum / 5; 
    ADC_StartGroupConversion(&adc_pal1_instance, 3UL);
}
void adc1_callback03(const adc_callback_info_t *const callbackInfo, void *userData)
{
    uint32_t sum = 0;
    (void)userData;
    (void)callbackInfo;
    for(int i = 0; i < 5; i++)
    {
        sum += adc_pal1_Results03[i];
    }
    g_voltVal  = sum / 5; 
    groupConvDone = true;
}

void taskCANBus(void)
{
    static uint8_t p100Cnt = 0;

    p100Cnt++;
    if (p100Cnt >= 100)
    {
        
        LLS1_DATA -> laserStatus = (uint8_t)((PINS_DRV_GetPinsOutput(PTE) & (1 << 9)) >> 9);
        LLS1_DATA -> blueMeasuredVal = g_blueVal;
        LLS1_DATA -> tempMeasuredVal = g_tempVal;
        LLS1_DATA -> voltMeasuredVal = g_voltVal;
        memcpy(LLS1_Tx_Msg.data, LLS1_DATA, 7);
        LLS1_Tx_Msg.cs = 0u;
        LLS1_Tx_Msg.id = LLS1_MSG_ID;
        LLS1_Tx_Msg.length = 7;
        CAN_Send(&can_pal1_instance, LLS1_MAILBOX, &LLS1_Tx_Msg);
        //        p100Cnt = 0;
    }
    //    static uint16_t p1000_t = 0;
    //    p1000_t++;
    //    if(p1000_t >= 1000)
    //    {
    //        PINS_DRV_TogglePins(PTE, (1u << 9u));
    //        p1000_t = 0;
    //    }
}

void taskADC(void)
{
    if(groupConvDone == true)
    {
        groupConvDone = false;
        ADC_StartGroupConversion(&adc_pal1_instance, 0UL);
    }
}

void taskLinBus(void)
{
}

void setTask(void)
{
    taskInit();
    addTask(taskADC, 10, ID_SENSOR);
    addTask(taskCANBus, 100, ID_CANBUS);
}
