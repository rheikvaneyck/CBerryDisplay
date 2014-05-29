#include "clearscreen.h"

void RAIO_screen_off( void ) {
  RAIO_SetRegister( PWRR , 0x40 ); 
  TFT_wait_for_raio();
}

int main( int argc, char **argv ) { 
  if (!bcm2835_init())
    return 1;
  
/*  TFT_init_board();
  TFT_hard_reset();
  RAIO_init(); */
  
  RAIO_screen_off();

  bcm2835_close();
  
  return 0;
}
