#include "writetext.h"

void WriteText_BW( uint16_t pos_x, unsigned char *text ) {    
  RAIO_SetFontSizeFactor ( 0 );
  RAIO_print_text ( pos_x, 0, text, COLOR_BLACK , COLOR_WHITE );
}


int main( int argc, char **argv ) { 

  bool rst_screen = true;
  line_number = 1;
  font_size = 16;

  while((opt = getopt(argc, argv, "rl:")) != -1) {
    switch (opt) {
      case 'r':
        rst_screen = false;
        break;
      case 'l':
        line_number = atoi(optarg);
        break;
      default:
        fprintf(stderr, "Usage: %s [-r] [-l number]\n", argv[0]);
        exit(EXIT_FAILURE);
    }
  }

  if (!bcm2835_init())
    return 1;
  
  if (rst_screen)) {
    TFT_init_board();
    TFT_hard_reset();
    RAIO_init();  
  }

  
  WriteText_BW((line_number - 1)*font_size, (unsigned char*) argv[1]);

  printf("%i\n", line_number);
      
  bcm2835_close();
  
  return 0;
}