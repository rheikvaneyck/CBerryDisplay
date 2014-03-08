#include "writetext.h"

void example_WriteText( unsigned char *text ) {
  RAIO_clear_screen();
    
  RAIO_SetFontSizeFactor ( 0 );
  RAIO_print_text ( 10, 10, text, COLOR_BLACK , COLOR_WHITE );
  
  RAIO_SetFontSizeFactor ( 5 );
  RAIO_print_text( 30, 30, text, COLOR_BLACK, COLOR_RED );
     
  RAIO_SetFontSizeFactor ( 10 );
  RAIO_print_text ( 60, 70, text, COLOR_BLACK, COLOR_BLUE );
  
  RAIO_SetFontSizeFactor ( 15 );
  RAIO_print_text ( 80, 120, text, COLOR_BLACK, COLOR_GREEN );
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
  example_WriteText( "C-Berry" );
      
  bcm2835_close();
  
  return 0;
}