// Motor 1 = right wheel, Motor 2 = left wheel

void wheelRobotForward() {
    zRobotSetMotorSpeed(1, -50);
    zRobotSetMotorSpeed(2, 50);
}

void wheelRobotStop() {
    zRobotSetMotorSpeed(1, 0);
    zRobotSetMotorSpeed(2, 0);
}

void wheelRobotTurnRight() {
    zRobotSetMotorSpeed(1, -50);
    zRobotSetMotorSpeed(2, 100);
}

void wheelRobotTurnLeft() {
    zRobotSetMotorSpeed(1, -100);
    zRobotSetMotorSpeed(2, 50);
}

void wheelRobotTurnOnSpot(bool right) {
    if(right) {
        zRobotSetMotorSpeed(1, 50);
        zRobotSetMotorSpeed(2, 50);
    } else {
        zRobotSetMotorSpeed(1, -50);
        zRobotSetMotorSpeed(2, -50);
    }
}