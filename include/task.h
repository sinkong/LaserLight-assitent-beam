/**
 * @file task.h
 * @brief method of operat tasks
 * 
 * @version     V1.0
 * @author      liulian 
 * @date        2019/09/28
 */
#ifndef TASK_H
#define TASK_H

// Includings
#include "stddef.h"
#include "stdint.h"

#define TASK_NUM_MAX 20
/**
 * @brief
 * 
 */
typedef enum
{
    ID_LASER,
    ID_CANBUS,
    ID_SENSOR
}taskId_t;

typedef struct
{
    void (*taskHandle)(void);
    uint16_t timer;
    uint16_t realTimer;
    uint16_t taskID;
}task_t;

void setTaskListNum(uint16_t num);
uint16_t getTaskListNum(void);
void taskInit(void);
uint8_t addTask(void (*pFunc)(void), uint16_t timer, uint16_t taskID);
uint8_t delTask();
void taskSchedule(void);        
void tick1msTaskRoutines(void); 


#endif