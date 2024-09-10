/* This file is a copy from a KAU repository on github*/

#include "MeAuriga.h"
#include "Arduino_FreeRTOS.h"
#include "semphr.h"

#define MAXIMUM_TASKS 10
#define ZSCHED_TRACE

int ztask_count;
struct zTask {
  int interval, runtime, lastrun, nextrun, actualtime;
  void *func;
  int id;
  TaskHandle_t *handle;
};

struct zTask zTaskList[MAXIMUM_TASKS];
int zTaskTrace = 0;
MeRGBLed zLedRing(0, 12);
void zTaskRunner(void *);
MeLineFollower zLineFollower(PORT_9);
MeUltrasonicSensor zUltraSonicSensor(PORT_7);
MeEncoderOnBoard zMotor1(SLOT1);
MeEncoderOnBoard zMotor2(SLOT2);

/**
 * Fetches information from the line follower
 * sensor. Can return values 0-3 depending on
 * senror state. 
 */
uint8_t zRobotGetLineSensor() {
	return zLineFollower.readSensors();
}

/**
 * Fetches information from the ultrasonic distance
 * sensor. Returns a value from 0 - 400. 
 */
uint8_t zRobotGetUltraSensor() {
	return zUltraSonicSensor.distanceCm();
}

/**
 * Sets the motor speed for motor 1 or 2
 * 
 * @param speed set the motor speed to this
 * @param motor the motor which speed should change
 */
void zRobotSetMotorSpeed(int motor, int speed) {
	if (motor == 1) zMotor1.setMotorPwm(speed);
	if (motor == 2) zMotor2.setMotorPwm(speed);
}

/* Task Management */
int zTaskCompare(const void *cmp1, const void *cmp2) {
  struct zTask *a = (struct zTask *)cmp1;
  struct zTask *b = (struct zTask *)cmp2;
  return a->interval - b->interval;
}

/**
 * Delay time milliseconds, blocking other process execution. 
 * Do not use any other delay method as it will break
 * scheduling. 
 *
 * @param time delay time in milliseconds
 */
void zBlockingDelay(int time) {
  TickType_t currentTick = xTaskGetTickCount();
  while (xTaskGetTickCount() - currentTick < time)
    ;
}

void zSetAllLed(int r, int g, int b) {
  for (int i = 0; i < 12; i++)
    zLedRing.setColorAt(i, r, g, b);
  zLedRing.show();
}

void zSetOneLed(int led, int r, int g, int b) {
  for (int i = 0; i < 12; i++)
    if (led == i)
      zLedRing.setColorAt(i, r, g, b);
    else
      zLedRing.setColorAt(i, 0, 0, 0);
}

void zDie(int taskno, const char *reason) {
  vTaskSuspendAll();
  Serial.print("Task ");
  Serial.print(taskno);
  Serial.print(" hard failure due to: ");
  Serial.println(reason);
  Serial.println("HALT");
  zSetAllLed(0, 0, 0);
  zLedRing.setColorAt(taskno + 1, 255, 0, 0);
  zLedRing.setColorAt(0, 64, 64, 64);
  zLedRing.show();
  for (;;) delay(500);
}

/**
 * Schedule a task for periodic execution. 
 *
 * @param func pointer to function that should run
 * @param interval the period of the function (T)
 * @param runtime the runtime of the function (C)
 */
void zScheduleTask(void *func, int interval, int runtime) {
  zTaskList[ztask_count].interval = interval;
  zTaskList[ztask_count].runtime = runtime;
  zTaskList[ztask_count].func = func;
  zTaskList[ztask_count].id = ztask_count;
  zTaskList[ztask_count].nextrun = interval;
  ztask_count = ztask_count + 1;
}

/**
 * Initialize the scheduler and robot. Run this before
 * any other functions. Will initialize Serial system
 * at 9600 baud. 
 */
void zInitialize() {
  Serial.begin(9600);

  TCCR1A = _BV(WGM10);
  TCCR1B = _BV(CS11) | _BV(WGM12);
  TCCR2A = _BV(WGM21) | _BV(WGM20);
  TCCR2B = _BV(CS21); 

  zMotor1.setMotionMode(DIRECT_MODE);
  zMotor2.setMotionMode(DIRECT_MODE);

  ztask_count = 0;
  zLedRing.setpin(44);
  zSetAllLed(0, 0, 0);
  zSetOneLed(0, 0, 128, 0);
  memset(zTaskList, 0, 10 * sizeof(struct zTask));
}

/**
 * Toggle debug information printing
 * 
 * @param trace 1 for enable, 0 for disable
 */
void zTaskSetTrace(int trace) {
	zTaskTrace = trace;
}

SemaphoreHandle_t zLock;

/**
 * Start scheduler. After running this function
 * execution is yielded to the scheduler and normal
 * program function is stopped. 
 */
void zStart() {
  zLock = xSemaphoreCreateBinary();
  xSemaphoreGive(zLock);
  qsort(zTaskList, ztask_count, sizeof(struct zTask), zTaskCompare);
  for (int i = 0; i < ztask_count; i++) {
    xTaskCreate(zTaskRunner, "MainTask", 128, &zTaskList[i], i, zTaskList[i].handle);
  }
}

void zPrintDebugEvent(struct zTask * task, int time, const char * text) {
  #ifdef ZSCHED_TRACE
	if (!zTaskTrace) return;
    Serial.print(time);
    Serial.print(": task ");
    Serial.print(task->id);
    Serial.print(" ");
    Serial.print(text);
    Serial.print(" { runtime: ");
    Serial.print(task->actualtime);
    Serial.println(" } ");
  #endif
}

void zTaskRunner(void *parm) {
  struct zTask *task = (struct zTask *)parm;
  for (;;) {
    //Before Running
    xSemaphoreTake(zLock, 0);
    int starttime = xTaskGetTickCount();
    zPrintDebugEvent(task, starttime, "start");
    if (starttime > task->nextrun) {
      zDie(task->id, "Did not start on time");
    }
    task->lastrun = starttime;
    xSemaphoreGive(zLock);

    //Run the code
    ((void (*)())task->func)();

    //After running
    xSemaphoreTake(zLock, 0);
    int endtime = xTaskGetTickCount();
    task->actualtime = endtime - starttime;
    if (endtime > task->nextrun) {
      zDie(task->id, "Finished after period end");
    }
    if (task->actualtime > task->runtime) {
      zDie(task->id, "Runtime exceeded computation time");
    }
    zPrintDebugEvent(task, starttime, "finished");
    task->nextrun = task->nextrun + task->interval;
    xSemaphoreGive(zLock);
    vTaskDelayUntil(&starttime, task->interval);
  }
}
