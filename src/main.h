typedef struct DrivingStance {
  bool clockwise;             // value to know if robot is driving clockwise or not. Good to know when avoiding object. 
  bool isClockwiseCalculated; // starts as false but is set to true after first loop. 
  bool rightTrackTurn; 
  bool leftTrackTurn; 
  bool avoidObject;
  int avoidTicks;
  bool objectDetected;
  bool driveTrack;
  bool outOfLineTicks;    // hur många ticks har roboten varit borta från linjen. 
  /*Test*/
  int avoidState;        // value used to set what state in the avoid function we are.
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
  stance.rightTrackTurn = false; 
  stance.leftTrackTurn = false; 
  stance.avoidObject = false;
  stance.avoidTicks = 0;
  stance.objectDetected = false;
  stance.driveTrack = true;
  stance.avoidState = 0;             
  stance.rightTurnTicks = 0;          
  stance.leftTurnTicks = 0;         
  stance.outOfLineTicks = 0;
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
  if (sensor.distance < 25) {
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
        if(stance.outOfLineTicks > 2) {
          if(stance.isClockwiseCalculated) {
            if(stance.outOfLineTicks < 7) {
              if(stance.clockwise) 
                stance.leftTrackTurn = true;
              else 
                stance.rightTrackTurn = true;
            } else {
              if(stance.clockwise)
                stance.rightTrackTurn = true; 
              else 
                stance.leftTrackTurn = true;
            }
          }
        }
        stance.outOfLineTicks++;
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
  } else {
    LEDrobotForward();
    wheelRobotForward();
  }
  if(!stance.isClockwiseCalculated) {
    if(stance.leftTurnTicks || stance.rightTurnTicks > 3) {
      calculateClockwise();
    }
  }
}

void avoidObstacle() 
{
    switch (stance.avoidState) {
      case 0: 
        if (stance.rightTurnTicks < 5 && stance.leftTurnTicks < 5) {
          if(stance.clockwise) {
            wheelRobotTurnRight();  
            LEDrobotRight();
            stance.rightTurnTicks++;
          } else {
            wheelRobotTurnLeft();  
            LEDrobotLeft();
            stance.leftTurnTicks++;
          }
      } else {
        stance.avoidTicks = 0;  // Nollställ för kör framåt
        stance.avoidState = 1; 
      }
      break;
      
    case 1: // Kör rakt fram en liten stund
      if (stance.avoidTicks < 5) {  
        wheelRobotForward();
        LEDrobotForward();
        stance.avoidTicks++;
      } else
        stance.avoidState = 2; 
      break;
      
    case 2: // Sväng vänster lika mycket som höger
      if(stance.clockwise) {
        wheelRobotTurnLeft();  
        LEDrobotLeft();
        stance.leftTurnTicks++;
      } else {
        wheelRobotTurnRight();  
        LEDrobotRight();
        stance.rightTurnTicks++;
      }
      if (stance.leftTurnTicks == stance.rightTurnTicks) {
        stance.avoidState = 3;
        stance.avoidTicks = 0;
      }
      break;

    case 3: 
      stance.avoidObject = false;
      stance.driveTrack = true;  
      stance.avoidState = 0;     
      stance.rightTurnTicks = 0; 
      stance.leftTurnTicks = 0;
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