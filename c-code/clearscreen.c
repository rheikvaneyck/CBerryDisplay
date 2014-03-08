#include "clearscreen.h"

int main( int argc, char **argv ) { 
  if (!bcm2835_init())
    return 1;
  
  TFT_init_board();
  TFT_hard_reset();
  RAIO_init();
  
  RAIO_clear_screen();

  bcm2835_close();
  
  return 0;
}