#include "wheelController.h"
#include "LEDController.h"
#include "soundController.h"

typedef struct Stance {
  bool rightTurn; 
  bool leftTurn; 
} Stance;

bool benchmark = 1;
TickType_t currentTick;
int distanceSensor;
int trackSensor;      // 0 = black line, 1 = black line left, 2 = black line right 3 = white area

bool driveStatus;
bool avoidStatus;
bool obstacleStatus;
bool statusChanged;
int obstacleStatusTicker;

Stance stance = { false, false };



void getSensorStatus() {
  distanceSensor = zRobotGetUltraSensor();
  trackSensor = zRobotGetLineSensor();
  if (distanceSensor < 10) obstacleStatus = true;
  switch (trackSensor) {
    case 0: // black
        stance.leftTurn = false;
        stance.rightTurn = false;
        break;
    case 1: // turn left
        stance.leftTurn = true;
        break;
    case 2: // turn right
        stance.rightTurn = true;
        break;
    case 3: // out of line

        break;
  }
}

void resetSensorStatus() {
  obstacleStatus = false;
  statusChanged = false;
}

void avoidManeuver() {
  if(obstacleStatus) {
    obstacleStatusTicker += 1;
    obstacleStatus = false;
    /*Serial.print("AvoidManeuver - Object distance is ");
    Serial.print(distanceSensor);
    Serial.println(" cm");
    Serial.print(obstacleStatusTicker);
    Serial.println(" ticks since object was sighted");
    Serial.flush();*/
    LEDrobotStop();
    LEDrobotStopAlert();
    wheelRobotTurnOnSpot(true);
  } else {
    obstacleStatusTicker = 0;
    avoidStatus = false;
    driveStatus = true;
  }
}

void drive() {
  if(driveStatus) {
    if(stance.leftTurn = true) {
      LEDrobotLeft();
      wheelRobotTurnLeft();
    } else if (stance.rightTurn = true) {
      LEDrobotRight();
      wheelRobotTurnRight();
    } else {
      switch(trackSensor) {
        case 0:
        LEDrobotForward();
        wheelRobotForward();
        break;
        case 3:
        LEDrobotStop();
        wheelRobotStop();
        break;
      }
    }
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