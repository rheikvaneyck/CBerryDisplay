#include "clearscreen.h"

void CB_clear_screen( void )
{
  // for more informations see RA8870 specification page 40
  //
  //    | Bit | Function
  //    |-----|-------------------------------------------------  
  //    |  7  | 0 = stop clear   1 = start clear
  //    |  6  | 0 = fullwindow   1 = activewindow
  //    | 5-1 | NA
  //    |  0  | 0 = Memory clear with BTE background color   1 = Memory clear with font background color  
  //
  //      Reg 0x43 define font background color ( RRRGGGBB )
  //      Reg 0x60, 0x61, 0x62 define BTE background color ( BGCR0=red[4:0], BGCR1=green[5:0], BGCR2=blue[4:0] )
  
  RAIO_SetRegister( MCLR , 0x81 ); 
  TFT_wait_for_raio();
}

int main( int argc, char **argv ) { 
  
  TFT_init_board();
  TFT_hard_reset();
  RAIO_init();
  CB_clear_screen();
}