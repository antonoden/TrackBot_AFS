#include "src/kaulab.h"
#include "src/main.h"

void setup() {
  // Usinng Kaulab.h då set up real time schuduling of tasks.
  zInitialize();
  zScheduleTask(getSensorStatus, 5, 2);
  zScheduleTask(drive, 10, 4);
  zStart();
}

// Schuduling and running of taskes is managed by kaulab.h which is started in setup().
void loop() {
}