#include "writetext.h"

void WriteText_BW( unsigned char *text ) {    
  RAIO_SetFontSizeFactor ( 0 );
  RAIO_print_text ( 0, 0, text, COLOR_BLACK , COLOR_WHITE );
}


int main( int argc, char **argv ) { 
  if(!argv[1])
   return 1;

  if (!bcm2835_init())
    return 1;
  
  
  TFT_init_board();
  TFT_hard_reset();
  RAIO_init();
  
  // depict a BMP file
  // ---------------------------------------------  
  WriteText_BW( (unsigned char*) argv[1]);
      
  bcm2835_close();
  
  return 0;
}