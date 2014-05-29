#include "writetext.h"

void WriteText( uint16_t pos_y, unsigned char *text, uint8_t fg_color) {
  RAIO_SetFontSizeFactor ( 0 );
  RAIO_print_text ( 0, pos_y, text, COLOR_BLACK , fg_color );
}


int main( int argc, char **argv ) {

  bool rst_screen = true;
  bool blank_line = false;
  uint8_t line_number = 1;
  uint8_t font_size = 16;
  char color_name[10];
  uint8_t color = COLOR_WHITE;
  uint8_t bg_color = COLOR_BLACK;
  int opt;
  union my_union cursor_x;
  union my_union cursor_y;

  uint16_t characters_per_line = 320 / font_size * 2;
  uint16_t max_line_number = 240 / font_size;
  char *spaces;


  while((opt = getopt(argc, argv, "rbl:c:")) != -1) {
    switch (opt) {
      case 'r':
        rst_screen = false;
        break;
      case 'l':
        line_number = atoi(optarg);
        break;
      case 'c':
        strncpy(color_name, optarg, 10);
        break;
      case 'b':
        blank_line = true;
        break;
      default:
        fprintf(stderr, "Usage: %s [-r] [-b] [-l number] [-c text_color]\n", argv[0]);
        exit(EXIT_FAILURE);
    }
  }

 if (strcmp(color_name, "red") == 0) {
    color = COLOR_RED;
  } else if (strcmp(color_name, "green") == 0) {
    color = COLOR_GREEN;
  } else if (strcmp(color_name, "blue") == 0) {
    color = COLOR_BLUE;
  } else if (strcmp(color_name, "yellow") == 0) {
    color = COLOR_YELLOW;
  } else if (strcmp(color_name, "cyan") == 0) {
    color = COLOR_CYAN;
  } else if (strcmp(color_name, "magenta") == 0) {
    color = COLOR_MAGENTA;
  } else if (strcmp(color_name, "darkgreen") == 0) {
    color = COLOR_DARK_GREEN;
  } else {
    color = COLOR_WHITE;
  }


  if (!bcm2835_init())
    return 1;
  
  if (rst_screen) {
    TFT_init_board();
    TFT_hard_reset();
    RAIO_init();  
  }

  if (blank_line) {
    spaces = malloc(characters_per_line*sizeof(char));
    memset(spaces, ' ', characters_per_line);
    WriteText((line_number - 1)*font_size, spaces, bg_color);
  }

  WriteText((line_number - 1)*font_size, (unsigned char*) argv[optind], color);

  RAIO_SetRegister( MWCR0, 0x80 );
  RAIO_SetRegister(CURH0, 0x10);
  RAIO_SetRegister(CURH1, 0x01);
  RAIO_SetRegister(CURV0, 0x20);
  RAIO_SetRegister(CURV1, 0x01);
  TFT_RegWrite( MRWC );
  TFT_DataWrite( 'A' );

  cursor_x.split.low = (uint8_t) RAIO_GetRegister(RCURH0);
  cursor_x.split.high = (uint8_t) RAIO_GetRegister(RCURH01);
  cursor_y.split.low = (uint8_t) RAIO_GetRegister(RCURV0);
  cursor_y.split.high = (uint8_t) RAIO_GetRegister(RCURV1);
  RAIO_SetRegister( MWCR0, 0x00 );
  printf("Cursor pos: (%i, %i)\n", cursor_x.value, cursor_y.value);  
  printf("%i\n", line_number + 1 > max_line_number ? 1 : line_number + 1);

      
  bcm2835_close();
  
  exit(EXIT_SUCCESS);
}
