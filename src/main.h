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
  int avoidState;        
  int rightTurnTicks;    // Hur mycket roboten har svängt höger
  int leftTurnTicks;     // Hur mycket roboten har svängt tillbaka vänster
} DrivningStance;

/* Variables not in use */

/* Variables in use */
int distanceSensor;
int trackSensor;      // 0 = black line, 1 = black line left, 2 = black line right 3 = white area

void driveInTrack();
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
    driveInTrack();
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
}

void driveInTrack() 
{
  if (stance.leftTrackTurn) 
  {
    LEDrobotLeft();
    wheelRobotTurnLeft();
  } else if (stance.rightTrackTurn) 
  {
    LEDrobotRight();
    wheelRobotTurnRight();
  } else 
  {
    LEDrobotForward();
    wheelRobotForward();
  }
}

void avoidObstacle() 
{

    /*if(stance.avoidTicks == 0) {
    // Sväng höger för att börja undvika hindret
    //LEDrobotAvoid();
    LEDrobotRight();
    wheelRobotTurnRight();
  } else if(stance.avoidTicks < 3) {
    // Kör framåt lite
    //LEDrobotAvoid();
    LEDrobotForward();
    wheelRobotForward();
  } else if(stance.avoidTicks < 5)  {
    // Sväng vänster för att återgå till linjen
    //LEDrobotAvoid();
    LEDrobotLeft();
    wheelRobotTurnLeft();
  } else if (stance.avoidTicks >= 5) {
    // Hindret undveks, återgå till linjeföljning
    stance.avoidObject = false;
    stance.objectDetected = false; 
    stance.driveTrack = true;
    stance.avoidTicks = 0; // Nollställ undvikningssteget
    return;
  }
  stance.avoidTicks += 1;
}*/
    switch (stance.avoidState)
    {
      case 0: 
    //if (stance.rightTurnTicks == 0)
      if (distanceSensor < 40) 
      {
        //wheelRobotTurnRight();  
        LEDrobotRight();
        //LEDrobotAvoid();
        stance.rightTurnTicks++; 
      } else 
      {
        stance.avoidTicks = 0;  // Nollställ för kör framåt
        stance.avoidState = 1; 
      }
      break;
      
    case 1: // Kör rakt fram en liten stund
      if (stance.avoidTicks < 3) 
      {  
        //wheelRobotForward();
        LEDrobotForward();
        stance.avoidTicks++;
        Serial.print("case 1 -> if");
        Serial.println(stance.avoidTicks);
        Serial.flush();
      } else 
      {
        Serial.println("case 1 -> else");
        Serial.flush();
        stance.leftTurnTicks = 0; 
        stance.avoidState = 2; 
      }
      break;
      
    case 2: // Sväng vänster lika mycket som höger
      if (stance.leftTurnTicks < stance.rightTurnTicks) 
      {
        //wheelRobotTurnLeft(); 
        //LEDrobotAvoid();
        LEDrobotLeft();
        stance.leftTurnTicks++; 
        Serial.print("case 2 -> if");
        Serial.println(stance.avoidTicks);
      } else {
        stance.avoidState = 3;  
        Serial.print("case 2 -> if");
        Serial.println(stance.avoidTicks);
      }
      break;
      
    case 3: 
      Serial.print("case 3");
      stance.avoidObject = false;
      stance.driveTrack = true;  
      stance.avoidState = 0;     
      stance.rightTurnTicks = 0; 
      stance.leftTurnTicks = 0;
      //LEDrobotAvoid();
      //wheelRobotStop();
      //zBlockingDelay(50);
      break;
    }
}
