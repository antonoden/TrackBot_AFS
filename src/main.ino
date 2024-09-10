#include "KauLab/src/kaulab.h"
#include "main.h"



void setup() {
  // put your setup code here, to run once:
  zInitialize();
}
/*

*/
void loop() {
  
  mainloop();

  zBlockingDelay(5);
}