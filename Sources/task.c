/** 
 *  @file       task.c
 *  @brief      The implementation of task operation method
 * 
 *  @version    V1.0 
 *  @author     EricXQ
 *  @date       2019/10/28
 */
#include "task.h"

taskId_t currentTaskID = (taskId_t)1;
uint16_t currentTaskNum = 0u;
uint16_t taskNum = 0u;
task_t taskList[TASK_NUM_MAX];
// task tatus flag
uint8_t fgTaskCritical = 0;

void taskEnterCritical(void)
{
    fgTaskCritical = 1;
}

void taskExitCritical(void)
{
    fgTaskCritical = 0;
}

uint8_t getTaskCritical(void)
{
    return fgTaskCritical;
}

/**
 * @brief           Set the num of tasks in list
 * @param    num    task quantity
 * @return          void 
 */
void setTaskListNum(unsigned short num)
{
    taskNum = num;

}

/** 
 * @brief       Get the num of tasks in list
 * @param       none 
 * @return      void 
 */
uint16_t getTaskListNum(void)
{
    return taskNum;
}

/**
 * @brief    :   Initial task list
 * @param    :   num 
 * @return   :   None 
 *********************************************/
void taskInit(void)
{
    for(uint16_t i = 0; i < TASK_NUM_MAX; i++)
    {
       taskList[i].taskHandle = NULL;
       taskList[i].timer = 0xFFFF;
       taskList[i].realTimer = 0xFFFF;
       taskList[i].taskID = 0xFFFF;
    }
} 
/**
 * @brief   Add task to task list
 * @param   pFunc      task function pointer to be added
 * @param   timer               timer
 * @param   taskID              id
 * @return  0 == failed, 1 == success
 */
uint8_t addTask(void (*pFunc)(void), uint16_t timer, uint16_t taskID)
{
    uint16_t i = 0;
    if(taskNum >= TASK_NUM_MAX)
    return 0;     
    // tasks no repeat
    for(i = 0; i < taskNum; i++)
    {
        if(taskList[i].taskHandle == pFunc)
        {
            break;
        }
    }
    if(i < taskNum)
    return  0;

    taskList[i].taskHandle = pFunc; 
    taskList[i].timer = timer;
    taskList[i].realTimer = timer;
    taskList[i].taskID = taskID;
    
    taskNum++;
    return 1;
}

/**
 * @brief
 * @param
 * @return
 */
uint8_t delTask(void (*pFunc)(void))
{
    uint16_t i = 0;
    if(taskNum == 0)
        return 0;
    taskEnterCritical();
    for(i = 0; i < taskNum; i++)
    {
        if(taskList[i].taskHandle == pFunc)
        {
            taskList[i].taskHandle = NULL;
            taskList[i].timer = 0xFFFF;
            taskList[i].realTimer = 0xFFFF;
            taskList[i].taskID = 0xFFFF;
            break;
        }
    }
    if(i >= taskNum)
    {
        taskExitCritical();
        return 1;
    }
    else 
    {
        for(; i < taskNum; i++)
        {
            taskList[i].taskHandle = taskList[i+1].taskHandle;
            taskList[i].timer = taskList[i+1].timer;
            taskList[i].realTimer = taskList[i+1].realTimer;
            taskList[i].taskID = taskList[i+1].taskID;

            taskList[i+1].taskHandle = NULL;
            taskList[i+1].timer = 0xFFFF;
            taskList[i+1].realTimer = 0xFFFF;
            taskList[i+1].taskID = 0xFFFF; 
        }
    }
    taskNum--;
    taskExitCritical();
    return 1;
}
void taskSchedule(void)        
{
    uint16_t i = 0;
    for(i = 0; i < taskNum; i++ )
    {
        if(0 == taskList[i].timer && taskList[i].taskHandle != NULL)
        {
            currentTaskID = (taskId_t)taskList[i].taskID;
            currentTaskNum = i;
            taskList[i].taskHandle();
            taskList[i].timer = taskList[i].realTimer;
        }
    }
}

void tick1msTaskRoutines(void)
{
    uint8_t timer1msId = 0;
    if (1 == getTaskCritical())
        return;

    for (timer1msId = 0; timer1msId < taskNum; timer1msId++)
    {
        if (0xFFFF > taskList[timer1msId].timer)
        {
            if (0 < taskList[timer1msId].timer)
            {
                taskList[timer1msId].timer--;
            }
        }
    }
}








