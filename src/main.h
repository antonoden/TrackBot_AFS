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
  /*Test*/
  int avoidState;        // Nya variabler för tillståndshantering
  int rightTurnTicks;    // Hur mycket roboten har svängt höger
  int leftTurnTicks;     // Hur mycket roboten har svängt tillbaka vänster
} DrivningStance;

/* Variables not in use */

/* Variables in use */
int distanceSensor;
int trackSensor;      // 0 = black line, 1 = black line left, 2 = black line right 3 = white area

void driveTrack();
void avoidObstacle();

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
  if(stance.driveTrack) 
  {       
    if(stance.leftTrackTurn) 
    {
      LEDrobotLeft();
      wheelRobotTurnLeft();
    } else if (stance.rightTrackTurn) 
    {
      LEDrobotRight();
      wheelRobotTurnRight();
    } else {
      LEDrobotForward();
      wheelRobotForward();
    }
  } else if(stance.avoidObject) 
  {
    avoidObstacle();  
  } else if(stance.objectDetected) 
  {
    LEDrobotObstacle();
    wheelRobotStop();
  } else {
    zSetAllLed(40, 40, 40);  // Standard LED om inget annat
  }
  /*
  /* TRACK driving 
  if(stance.driveTrack) 
  {  
    driveTrack();
  /* AVOID driving 
  } else if(stance.avoidObject) 
  {
    avoidObstacle();

  } else if(stance.objectDetected) 
  {
    LEDrobotObstacle();
    wheelRobotStop();
  } else 
  {
    zSetAllLed(40,40,40);
  }*/
}

void driveTrack() 
{
  if(stance.leftTrackTurn) 
  {
    LEDrobotLeft();
    wheelRobotTurnLeft();
  } else if(stance.rightTrackTurn) 
  {
    LEDrobotRight();
    wheelRobotTurnRight();
  } else {
    LEDrobotForward();
    wheelRobotForward();
  }
}

void avoidObstacle() 
{
  switch (stance.avoidState) {
    case 0: 
      if (distanceSensor < 40) 
      {
        wheelRobotTurnRight();  
        LEDrobotRight();
        //LEDrobotAvoid();
        stance.rightTurnTicks++; 
      } else 
      {
        stance.avoidState = 1; 
        stance.avoidTicks = 0;  // Nollställ för kör framåt
      }
      break;
      
    case 1: // Kör rakt fram en liten stund
      if (stance.avoidTicks < 6) 
      {  
        wheelRobotForward();
        stance.avoidTicks++;
      } else 
      {
        stance.avoidState = 2; 
        stance.leftTurnTicks = 0; 
      }
      break;
      
    case 2: // Sväng vänster lika mycket som höger
      if (stance.leftTurnTicks < stance.rightTurnTicks) 
      {
        wheelRobotTurnLeft(); 
        //LEDrobotAvoid();
        LEDrobotLeft();
        stance.leftTurnTicks++; 
      } else {
        stance.avoidState = 3;  
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
  /*
  if(stance.avoidTicks < 2) 
  {
    LEDrobotAvoid();
    wheelRobotTurnRight();
  } else {
    LEDrobotAvoid();
    wheelRobotTurnLeft();
  }
  stance.avoidTicks += 1;*/
}

