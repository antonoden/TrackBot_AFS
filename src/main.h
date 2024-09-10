#include "wheelController.h"
#include "LEDController.h"
#include "soundController.h"

bool benchmark = 1;
TickType_t currentTick;
int distanceSensor;
int trackSensor;

bool driveStatus;
bool avoidStatus;
bool obstacleStatus;
bool statusChanged;
int obstacleStatusTicker;

void getSensorStatus() {
  distanceSensor = zRobotGetUltraSensor();
  trackSensor = zRobotGetLineSensor();
  if (distanceSensor < 80) obstacleStatus = true;
}

void resetSensorStatus() {
  obstacleStatus = false;
  statusChanged = false;
}

void avoidManeuver() {
  if (!obstacleStatus) {
    obstacleStatusTicker = 0;
    avoidStatus = false;
    Serial.println("Object out of sight. Avoid Maneuver aborted");
    Serial.flush();
  } else {
    obstacleStatusTicker += 1;
    obstacleStatus = false;
    Serial.print("AvoidManeuver - Object distance is ");
    Serial.print(distanceSensor);
    Serial.println(" cm");
    Serial.print(obstacleStatusTicker);
    Serial.println(" ticks since object was sighted");
    Serial.flush();
    LEDrobotStop();
    wheelRobotTurnOnSpot(true);
  }
}

void drive() {
  Serial.flush();
  switch (trackSensor) {
    case 0:
      LEDrobotForward();
      wheelRobotForward();
      break;
    case 1:
      LEDrobotLeft();
      wheelRobotTurnLeft();
      break;
    case 2:
      LEDrobotRight();
      wheelRobotTurnRight();
      break;
    case 3:
      LEDrobotStop();
      wheelRobotStop();
      ImperialMarch();
      break;
  }
}

void avoidInit() {
  Serial.print("AvoidInit - Object distance is ");
  Serial.print(distanceSensor);
  Serial.println(" cm");
  Serial.flush();
  obstacleStatus = false;
  avoidStatus = true;
  LEDrobotStop();
  wheelRobotStop();
}

void mainloop() {
  if (benchmark) currentTick = xTaskGetTickCount();

  getSensorStatus();
  if (avoidStatus) avoidManeuver();
  else if (obstacleStatus) avoidInit();
  else drive();
  resetSensorStatus();

  if (benchmark) {
    Serial.print("Benchmark: ");
    Serial.flush();
    Serial.println(xTaskGetTickCount() - currentTick);
    Serial.flush();
    benchmark = 0;
  }
}