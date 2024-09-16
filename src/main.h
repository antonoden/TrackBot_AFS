#include "wheelController.h"
#include "LEDController.h"
#include "soundController.h"

typedef struct DrivingStance {
  bool rightTrackTurn; 
  bool leftTrackTurn; 
  bool avoidObject;
  int avoidTicks;
  bool objectDetected;
  bool driveTrack;
} DrivningStance;

/* Variables not in use */

/* Variables in use */
int distanceSensor;
int trackSensor;      // 0 = black line, 1 = black line left, 2 = black line right 3 = white area

DrivingStance stance = { false, false, false, 0, false, true };

void getSensorStatus() {
  distanceSensor = zRobotGetUltraSensor();
  trackSensor = zRobotGetLineSensor();
  if (distanceSensor < 40) {
    stance.objectDetected = true;
    stance.avoidObject = true;
    stance.driveTrack = false;
  }
  switch (trackSensor) {
    case 0: // black
        stance.leftTrackTurn = false;
        stance.rightTrackTurn = false;
        break;
    case 1: // turn left
        stance.leftTrackTurn = true;
        break;
    case 2: // turn right
        stance.rightTrackTurn = true;
        break;
    case 3: // out of line
        
        break;
  }
}

void drive() {
  /* TRACK driving */
  if(stance.driveTrack) {       
    if(stance.leftTrackTurn) {
      LEDrobotLeft();
      wheelRobotTurnLeft();
    } else if (stance.rightTrackTurn) {
      LEDrobotRight();
      wheelRobotTurnRight();
    } else {
      LEDrobotForward();
      wheelRobotForward();
    }
  /* AVOID driving */
  } else if(stance.avoidObject) {
    if(stance.avoidTicks < 2) {
      LEDrobotAvoid();
      wheelRobotTurnRight();
    } else {
      LEDrobotAvoid();
      wheelRobotTurnLeft();
    }
    stance.avoidTicks += 1;
  } else if(stance.objectDetected) {
    LEDrobotObstacle();
    wheelRobotStop();
  } else {
    zSetAllLed(40,40,40);
  }
}

