#include "src/kaulab.h"
#include "src/main.h"



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