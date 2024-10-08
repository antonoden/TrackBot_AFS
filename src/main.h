typedef struct DrivingStance {
  bool clockwise;             // value to know if robot is driving clockwise or not. Good to know when avoiding object. 
  bool isClockwiseCalculated; // starts as false but is set to true after first loop. 
  bool rightTrackTurn; 
  bool leftTrackTurn;
  bool reverse; 
  bool avoidObject;
  int avoidTicks;
  bool objectDetected;
  bool driveTrack;
  bool outOfLineTicks;    // hur många ticks har roboten varit borta från linjen. 
  /*Test*/
  int avoidState;        // value used to set what state in the avoid function we are.
  int avoidMoveOneTicks;
  int avoidMoveThirdTicks;
  int rightTurnTicks;    // value used in both calc. clockwise and in avoidfunction
  int leftTurnTicks;     // value used in both calc. clockwise and in avoidfunction
} DrivningStance;

typedef struct Sensor {
  int distance;
  int track;      // 0 = black line, 1 = black line left, 2 = black line right 3 = white area
} Sensor;

DrivingStance stance = { };
Sensor sensor = { };

#include "wheelController.h"
#include "LEDController.h"
#include "soundController.h"

void driveInTrack();
void avoidObstacle();
void setSensorStatus();
void setSurroundingStance();
void setTrackingStance();

/* 
  Robot setup function
*/

void robotSetup() {
  stance.clockwise = false;              
  stance.isClockwiseCalculated = false; 
  stance.rightTurnTicks = 0;          
  stance.leftTurnTicks = 0; 
  stance.rightTrackTurn = false; 
  stance.leftTrackTurn = false; 
  stance.avoidObject = false;
  stance.avoidTicks = 0;
  stance.objectDetected = false;
  stance.driveTrack = true;
  stance.avoidState = 0;                     
  stance.outOfLineTicks = 0;
  stance.reverse = false;
}

/*
  ROBOT Stance and sensor functions
*/

void setRobotStance() {
  setSensorStatus();

  setSurroundingStance();

  setTrackingStance();
}

void calculateClockwise() {
  if(stance.leftTurnTicks > stance.rightTurnTicks) {
      stance.clockwise = false;
    } else {
      stance.clockwise = true;
    }
    stance.isClockwiseCalculated = true;
    stance.leftTurnTicks = 0;
    stance.rightTurnTicks = 0;
}

void setSensorStatus() {
  sensor.distance = zRobotGetUltraSensor();
  sensor.track =  zRobotGetLineSensor();
}

void setSurroundingStance() {
  if (sensor.distance < 10) {
    stance.avoidObject = true;
    stance.objectDetected = true;
    stance.driveTrack = false;
  } else {
    stance.objectDetected = false;
  }
}

void setTrackingStance() {
  switch (sensor.track) {
    case 0: // black
        stance.leftTrackTurn = false;
        stance.rightTrackTurn = false;
        stance.outOfLineTicks = 0;
        break;
    case 1: // turn left
        stance.leftTrackTurn = true;
        break;
    case 2: // turn right
        stance.rightTrackTurn = true;
        break;
    case 3: // out of line
        /*
          stance.reverse = true;
          stance.leftTrackTurn = false;
          stance.rightTrackTurn = false;*/
        break;
  }
}

/*
  ROBOT action functions
*/

void drive()  {
  if(stance.driveTrack)  
    driveInTrack();
  else if(stance.avoidObject) 
    avoidObstacle();  
  else 
    zSetAllLed(40, 40, 40);  // Standard LED om inget annat
}

void driveInTrack() 
{
  if (stance.leftTrackTurn) {
    LEDrobotLeft();
    wheelRobotTurnLeft();
    if(!stance.isClockwiseCalculated) stance.leftTurnTicks++; 
  } else if (stance.rightTrackTurn) {
    LEDrobotRight();
    wheelRobotTurnRight();
    if(!stance.isClockwiseCalculated) stance.rightTurnTicks++;
  } else if (stance.reverse) {
    Serial.println("Reverse 3");
    Serial.flush();
    zRobotSetMotorSpeed(1, speed/2);
    zRobotSetMotorSpeed(2, -speed/2);
    LEDrobotReverse();
  } else {
    LEDrobotForward();
    wheelRobotForward();
  }
  if(!stance.isClockwiseCalculated) {
    if(stance.leftTurnTicks || stance.rightTurnTicks > 2) {
      calculateClockwise();
    }
  }
}

void avoidFirstMove() {
  if (stance.avoidMoveOneTicks < 5) {
    if(stance.clockwise) {
      wheelRobotTurnRight();  
      LEDrobotRight();
    } else {
      wheelRobotTurnLeft();  
      LEDrobotLeft();
    }
    stance.avoidMoveOneTicks++;
  } else {
      stance.avoidTicks = 0;  // Nollställ för kör framåt
      stance.avoidState = 1; 
  }
}

void avoidSecondMove() {
  if (stance.avoidTicks < 7) {  
    wheelRobotForward();
    LEDrobotForward();
    stance.avoidTicks++;
  } else
    stance.avoidState = 2;
}

void avoidThirdMove() {
  if(stance.clockwise) {
    wheelRobotTurnLeft();  
    LEDrobotLeft();
  } else {
    wheelRobotTurnRight();  
    LEDrobotRight();
  }
  stance.avoidMoveThirdTicks++;
  if (stance.avoidMoveThirdTicks > stance.avoidMoveOneTicks+4) {
    stance.avoidState = 3;
    stance.avoidTicks = 0;
  }
}

void avoidLeftTurn(int timer) {
    wheelRobotTurnLeft();  
    LEDrobotLeft();
    stance.avoidTicks++;
    if(stance.avoidTicks > timer) {
      stance.avoidState++;
      stance.avoidTicks = 0;
    }
}

void avoidRightTurn(int timer) {
    wheelRobotTurnRight();  
    LEDrobotRight();
    stance.avoidTicks++;
    if(stance.avoidTicks > timer) {
      stance.avoidState++;
      stance.avoidTicks = 0;
    }
}

/* Drived forward until track is found. Then makes one turn and gets into drive-mode */
void avoidWrapUp() {
    stance.avoidObject = false;
    stance.driveTrack = true;  
    stance.avoidState = 0;     
    stance.rightTurnTicks = 0; 
    stance.leftTurnTicks = 0;
    stance.avoidMoveThirdTicks = 0;
    stance.avoidMoveOneTicks = 0;
}


void avoidForward(int timer) {
  if (stance.avoidTicks < timer) {  
    wheelRobotForward();
    LEDrobotForward();
    stance.avoidTicks++;
  } else {
    stance.avoidTicks = 0;
    stance.avoidState++;
  }
}

void avoidBackward(int timer) {
  if (stance.avoidTicks < timer) {  
    wheelRobotbackward();
    LEDrobotReverse();
    stance.avoidTicks++;
  } else {
    stance.avoidTicks = 0;
    stance.avoidState++;
  }
}

void avoidObstacle() 
{
  switch (stance.avoidState) {
    case 0:
      avoidBackward(9);
      break;
    case 1: // Sväng in i cirkeln
      if(stance.clockwise) {
        avoidRightTurn(2);
      } else {
        avoidLeftTurn(2);
      }  
      break;
      
    case 2: // Kör rakt fram en liten stund
      avoidForward(11);
      break;
      
    case 3: // Sväng tillbaka till linjen
      if(stance.clockwise) {
          avoidLeftTurn(2);
        } else {
          avoidRightTurn(2);
        }  
      break;

    case 4: 
      avoidForward(18);
      break;
    
    case 5:
      if(stance.clockwise) {
          avoidLeftTurn(2);
        } else {
          avoidRightTurn(2);
        }  
      break;

    case 6:
      if(sensor.track == 3) {
        avoidForward(11);
      } else {        
        avoidForward(1);
        stance.avoidTicks = 0;
        stance.avoidState++;
      }
      break;

    case 7:
      if (stance.avoidTicks == 0) 
        avoidForward(1);
      else if (stance.clockwise) {
        avoidRightTurn(1);
      } else {
        avoidLeftTurn(1);
      }    
      break;

    case 8:
      avoidWrapUp();
      break;
  }
}


/*
case 3: // Åk rakt för att komma till linjen igen. 
      if(stance.avoidTicks < 3 || sensor.track == 3) {
        wheelRobotForward();
        LEDrobotForward();
      } else {
          if(stance.clockwise) {
            wheelRobotTurnRight();  
            LEDrobotRight();
          } else {
            wheelRobotTurnLeft();  
            LEDrobotLeft();
          }   
      }
      break;*/