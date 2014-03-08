#include "clearscreen.h"

int main( int argc, char **argv ) { 
  
  TFT_init_board();
  TFT_hard_reset();
  RAIO_clear_screen();
}