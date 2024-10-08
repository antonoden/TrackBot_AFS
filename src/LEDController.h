int nextLEDtoGlow = 0;
int LEDinstensity = 5;


void turnOfAllLEDs() {
  for (int i = 0; i < 12; i++)
    zLedRing.setColorAt(i, 0, 0, 0);
}

void LEDrobotForward() {
    turnOfAllLEDs();
    if(stance.avoidObject) {
        zLedRing.setColorAt(1, LEDinstensity, 0, 0);
        zLedRing.setColorAt(2, LEDinstensity, 0, 0);
        zLedRing.setColorAt(3, LEDinstensity, 0, 0);
    } else {
        zLedRing.setColorAt(1, 0, LEDinstensity, 0);
        zLedRing.setColorAt(2, 0, LEDinstensity, 0);
        zLedRing.setColorAt(3, 0, LEDinstensity, 0);
    }
    zLedRing.show();
}

void LEDrobotReverse() {
    turnOfAllLEDs();
    if(stance.avoidObject) {
        zLedRing.setColorAt(7, LEDinstensity, 0, 0);
        zLedRing.setColorAt(8, LEDinstensity, 0, 0);
        zLedRing.setColorAt(9, LEDinstensity, 0, 0);
    } else {
        zLedRing.setColorAt(7, 0, LEDinstensity, 0);
        zLedRing.setColorAt(8, 0, LEDinstensity, 0);
        zLedRing.setColorAt(9, 0, LEDinstensity, 0);
    }
    zLedRing.show();
}

void LEDrobotRight() {
    turnOfAllLEDs();
    if(stance.avoidObject) {
        zLedRing.setColorAt(4, LEDinstensity, 0, 0);
        zLedRing.setColorAt(5, LEDinstensity, 0, 0);
        zLedRing.setColorAt(6, LEDinstensity, 0, 0);
    } else {
        zLedRing.setColorAt(4, 0, LEDinstensity, 0);
        zLedRing.setColorAt(5, 0, LEDinstensity, 0);
        zLedRing.setColorAt(6, 0, LEDinstensity, 0);
    }
    zLedRing.show();
}

void LEDrobotLeft() {
    turnOfAllLEDs();
    if(stance.avoidObject) {
        zLedRing.setColorAt(10, LEDinstensity, 0, 0);
        zLedRing.setColorAt(11, LEDinstensity, 0, 0);
        zLedRing.setColorAt(0, LEDinstensity, 0, 0);
    } else {
        zLedRing.setColorAt(10, 0, LEDinstensity, 0);
        zLedRing.setColorAt(11, 0, LEDinstensity, 0);
        zLedRing.setColorAt(0, 0, LEDinstensity, 0);
    }
    
    zLedRing.show();
}

void LEDrobotStop() {
    turnOfAllLEDs();
    zSetOneLed(nextLEDtoGlow, LEDinstensity, 0, 0);
    zLedRing.show();
    if (nextLEDtoGlow < 11) { nextLEDtoGlow += + 1; }
    else { nextLEDtoGlow = 0; }
}

void LEDrobotObstacle() {
    turnOfAllLEDs();
    zSetOneLed(nextLEDtoGlow, LEDinstensity, 0, 0);
    zLedRing.show();
    if (nextLEDtoGlow < 11) { nextLEDtoGlow += + 1; }
    else { nextLEDtoGlow = 0; }
}

void LEDrobotAvoid() {
    turnOfAllLEDs();
    zSetOneLed(nextLEDtoGlow, 0, LEDinstensity, LEDinstensity);
    zLedRing.show();
    if (nextLEDtoGlow < 11) { nextLEDtoGlow += + 1; }
    else { nextLEDtoGlow = 0; }
}
