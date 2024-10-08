int speed = 120;
int turnspeed = speed*0.3; 
// diameter on turn is 59cm
// Space between wheels is 15cm
// outer wheel is (59/2)+7.5cm from center = 37 cm
// distance for outer wheel to drive is 37*3,14 = 116,18 cm
// inner wheel is (59/2)-7.5cm from center = 22 cm
// distance for inne wheel to drive is 22*3,14 = 69,08 cm

// speed for inner wheel in turn is suppose to be 59% of outer wheel speed. 

// Motor 1 = right wheel, Motor 2 = left wheel

void wheelRobotForward() {
    if(stance.avoidObject) {
        zRobotSetMotorSpeed(1, -speed/2);
        zRobotSetMotorSpeed(2, speed/2);
    } else {
        zRobotSetMotorSpeed(1, -speed/2);
        zRobotSetMotorSpeed(2, speed/2);
    }
}

void wheelRobotStop() {
    zRobotSetMotorSpeed(1, 0);
    zRobotSetMotorSpeed(2, 0);
}

void wheelRobotTurnRight() {
    if(stance.avoidObject) {
        zRobotSetMotorSpeed(1, speed);
        zRobotSetMotorSpeed(2, speed);
    } else {
        zRobotSetMotorSpeed(1, -turnspeed);
        zRobotSetMotorSpeed(2, speed);
    }
}

void wheelRobotTurnLeft() {
    if(stance.avoidObject) {
        zRobotSetMotorSpeed(1, -speed);
        zRobotSetMotorSpeed(2, -speed);
    } else {
        zRobotSetMotorSpeed(1, -speed);
        zRobotSetMotorSpeed(2, turnspeed);
    }
}

void wheelRobotTurnOnSpot(bool right) {
    if(right) {
        zRobotSetMotorSpeed(1, speed);
        zRobotSetMotorSpeed(2, speed);
    } else {
        zRobotSetMotorSpeed(1, -speed);
        zRobotSetMotorSpeed(2, -speed);
    }
}