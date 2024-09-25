#include "src/kaulab.h"
#include "src/main.h"

void setup() {
  // Usinng Kaulab.h då set up real time schuduling of tasks.
  robotSetup();
  zInitialize();
  zScheduleTask(setRobotStance, 7, 2);
  zScheduleTask(drive, 7, 3);
  zStart();
}

// Schuduling and running of taskes is managed by kaulab.h which is started in setup().
void loop() {
}