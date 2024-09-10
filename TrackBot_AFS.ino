#include "src/kaulab.h"
#include "src/main.h"



void setup() {
  // put your setup code here, to run once:
  zInitialize();
  zScheduleTask(getSensorStatus, 6, 2);
  zScheduleTask(drive, 6, 2);
  zScheduleTask(avoidManeuver, 20, 5);
  zStart();
}
/*

*/
void loop() {
  
}