int nextLEDtoGlow = 0;
int LEDinstensity = 5;


void turnOfAllLEDs() {
  for (int i = 0; i < 12; i++)
    zLedRing.setColorAt(i, 0, 0, 0);
}

void LEDrobotForward() {
    turnOfAllLEDs();
    zLedRing.setColorAt(1, 0, LEDinstensity, 0);
    zLedRing.setColorAt(2, 0, LEDinstensity, 0);
    zLedRing.setColorAt(3, 0, LEDinstensity, 0);
    zLedRing.show();
}

void LEDrobotRight() {
    turnOfAllLEDs();
    zLedRing.setColorAt(4, 0, LEDinstensity, 0);
    zLedRing.setColorAt(5, 0, LEDinstensity, 0);
    zLedRing.setColorAt(6, 0, LEDinstensity, 0);
    zLedRing.show();
}

void LEDrobotLeft() {
    turnOfAllLEDs();
    zLedRing.setColorAt(10, 0, LEDinstensity, 0);
    zLedRing.setColorAt(11, 0, LEDinstensity, 0);
    zLedRing.setColorAt(0, 0, LEDinstensity, 0);
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