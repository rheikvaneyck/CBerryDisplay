/*##############################################################*/
/* 																*/
/* File		: RAIO8870.c										*/
/*																*/
/* Project	: TFT for Raspberry Pi Revision 2					*/
/* 																*/
/* Date		: 2013-11-22   	    last update: 2014-02-28			*/
/* 																*/
/* Author	: Hagen Ploog   									*/
/*		  	  Kai Gillmann										*/
/*		  	  Timo Pfander										*/
/* 																*/
/* IDE	 	: Geany 1.22										*/
/* Compiler : gcc (Debian 4.6.3-14+rpi1) 4.6.3					*/
/*																*/
/* Copyright (C) 2013 admatec GmbH								*/
/*																*/
/*																*/	
/* Description  :												*/
/* 																*/
/*  This file contain several functions to initialize and       */
/*  control	the graphic controller RAIO8870.					*/
/*																*/
/*																*/
/* License:														*/
/*																*/
/*	This program is free software; you can redistribute it 		*/ 
/*	and/or modify it under the terms of the GNU General			*/ 	
/*	Public License as published by the Free Software 			*/
/*	Foundation; either version 3 of the License, or 			*/
/*	(at your option) any later version. 						*/
/*    															*/
/*	This program is distributed in the hope that it will 		*/
/*	be useful, but WITHOUT ANY WARRANTY; without even the 		*/
/*	implied warranty of MERCHANTABILITY or 						*/
/*	FITNESS FOR A PARTICULAR PURPOSE. See the GNU General 		*/
/*	Public License for more details. 							*/
/*																*/
/*	You should have received a copy of the GNU General 			*/
/*	Public License along with this program; if not, 			*/
/*	see <http://www.gnu.org/licenses/>.							*/
/*																*/
/*																*/
/* Revision History:											*/
/*																*/
/*	Version 1.0 - Initial release								*/
/*	Version 1.1 - added function RAIO_clear_screen( ... )		*/
/*				  added function RAIO_SetFontSizeFactor( ... )	*/
/*				  added function RAIO_print_text( ... )			*/
/*																*/
/*																*/
/*																*/
/*##############################################################*/

#include <bcm2835.h>
#include "RAIO8870.h"
#include "tft.h"

uint16_t txc = 0x00;        // character x position on screen
uint16_t tyc = 0x00;        // character y position on screen
uint8_t  char_higth = 15;   // character hight depends on character set


#ifdef CM_4K
	static uint8_t BankNo_WR=0, BankNo_RD=1;
#endif


// write command to a register
// ----------------------------------------------------------
void RAIO_SetRegister( uint8_t reg, uint8_t value )
{
    TFT_RegWrite( (uint16_t)reg );
    TFT_DataWrite( (uint16_t)value );
}


// set PWM value for backlight 
// ----------------------------------------------------------
void RAIO_SetBacklightPWMValue( uint8_t BL_value )
{
	RAIO_SetRegister(  P1CR, 0x88 ); 	 // Enable PWM1 output devider 256  
	RAIO_SetRegister( P1DCR, BL_value ); // -> BL_vaue = 0 (0% PWM) - 255 (100% PWM)
}


// initialization of RAIO8870
// ----------------------------------------------------------
void RAIO_init( void )
{
 	static uint8_t PLL_Initial_Flag = 0;
 	
	// *************** PLL settings (System Clock)  
	
	if ( !PLL_Initial_Flag )				// wait until PLL is ready
	{ 
		PLL_Initial_Flag = 1;               // set Flag to avoid repeated PLL init
		
		RAIO_SetRegister( PLLC1, 0x07 );    // set sys_clk 
		bcm2835_delayMicroseconds( 200 );
		RAIO_SetRegister( PLLC2, 0x03 );    // set sys_clk 
		bcm2835_delayMicroseconds( 200 );
		
		RAIO_SetRegister( PWRR, 0x01 );     // Raio software reset ( bit 0 ) set
		RAIO_SetRegister( PWRR, 0x00 );     // Raio software reset ( bit 0 ) set to 0
		delay( 100 ); 


	// *************** color modes (color depths)  
	
		#ifdef CM_65K
			// System Configuration Register
			RAIO_SetRegister( SYSR, 0x0A );   // digital TFT
											  // parallel data out
											  // no external memory
											  // 8bit memory data bus
											  // 16bpp 65K color
											  // 16bit MCU-interface (data)
			RAIO_SetRegister( DPCR, 0x00 );   // one layer	
		#elif defined(CM_4K)
			// System Configuration Register
			RAIO_SetRegister( SYSR, 0x06 );  // digital TFT
											 // parallel data out
											 // no external memory
											 // 8bit memory data bus
											 // 12bpp 4K color
											 // 16bit MCU-interface (data)
			RAIO_SetRegister( DPCR, 0x80 );  // two layers	
			RAIO_SetRegister( MWCR1, BankNo_WR );
			RAIO_SetRegister( LTPR0, BankNo_RD );
		#else
			#error "color_mode not defined"
		#endif										
	}
 
 
	// *************** horizontal settings
	    
	// 0x27+1 * 8 = 320 pixel  
    RAIO_SetRegister( HDWR , (DISPLAY_WIDTH / 8) - 1 );   
    RAIO_SetRegister( HNDFTR, 0x02 ); // Horizontal Non-Display Period Fine Tuning

    // HNDR , Horizontal Non-Display Period Bit[4:0] 
    // Horizontal Non-Display Period (pixels) = (HNDR + 1)*8    
    RAIO_SetRegister( HNDR, 0x03 );                            //       0x06
    RAIO_SetRegister( HSTR, 0x04 );   //HSTR , HSYNC Start Position[4:0], HSYNC Start Position(PCLK) = (HSTR + 1)*8     0x02                                 

    // HPWR , HSYNC Polarity ,The period width of HSYNC. 
    // 1xxxxxxx activ high 0xxxxxxx activ low
    // HSYNC Width [4:0] HSYNC Pulse width
    // (PCLK) = (HPWR + 1)*8
    RAIO_SetRegister( HPWR, 0x03 );   // 0x00
    
    
    // ********************* vertical settings    
    
    // 0x0EF +1 = 240 pixel
    RAIO_SetRegister(  VDHR0 , ( (DISPLAY_HEIGHT-1) & 0xFF ) ); 
    RAIO_SetRegister(  VDHR1 , ( (DISPLAY_HEIGHT-1) >> 8)    );
    
    // VNDR0 , Vertical Non-Display Period Bit [7:0]
    // Vertical Non-Display area = (VNDR + 1)
    // VNDR1 , Vertical Non-Display Period Bit [8]
    // Vertical Non-Display area = (VNDR + 1)              
    RAIO_SetRegister( VNDR0, 0x10 );
    RAIO_SetRegister( VNDR1, 0x00 );
                      
    // VPWR , VSYNC Polarity ,VSYNC Pulse Width[6:0]
    // VSYNC , Pulse Width(PCLK) = (VPWR + 1) 
    RAIO_SetRegister( VPWR, 0x00 );
    
    
    // *************** miscellaneous settings 
    
    // active Window
    Active_Window( 0, DISPLAY_WIDTH-1, 0, DISPLAY_HEIGHT-1 );     
        
    // PCLK fetch data on rising edge 
    RAIO_SetRegister( PCLK, 0x00 );   

	// Backlight dimming       
	RAIO_SetBacklightPWMValue(50);

	Text_Background_Color( COLOR_BLACK );  
	// memory clear with background color                 
    RAIO_SetRegister( MCLR, 0x81 );     
    TFT_wait_for_raio(); 
  
    RAIO_SetRegister( IODR, 0x07 );    
    RAIO_SetRegister( PWRR, 0x80 );
}

	

// set coordinates for active window
// ----------------------------------------------------------
void Active_Window( uint16_t XL, uint16_t XR , uint16_t YT, uint16_t YB )
{ 
	union my_union number;
	
	//setting active window X
	number.value = XL;
	RAIO_SetRegister( HSAW0, number.split.low );    
	RAIO_SetRegister( HSAW1, number.split.high );

	number.value = XR;
	RAIO_SetRegister( HEAW0, number.split.low );
	RAIO_SetRegister( HEAW1, number.split.high );

	
	//setting active window Y
	number.value = YT;
	RAIO_SetRegister( VSAW0, number.split.low );
	RAIO_SetRegister( VSAW1, number.split.high );

	number.value = YB;
	RAIO_SetRegister( VEAW0, number.split.low );
    RAIO_SetRegister( VEAW1, number.split.high );
}


// show the BMP picture on the TFT screen
// ----------------------------------------------------------
void RAIO_Write_Picture( uint16_t *data, uint32_t count )
{
	TFT_RegWrite( MRWC );
	TFT_DataMultiWrite( data, count);
	
#ifdef CM_4K
	if ( BankNo_WR==0 )
	{
		BankNo_WR=1;
		BankNo_RD=0;
	}
	else
	{
		BankNo_WR=0;
		BankNo_RD=1;
	}
		
	RAIO_SetRegister( MWCR1, BankNo_WR );
	RAIO_SetRegister( LTPR0, BankNo_RD );	
#endif
}


// set mode for BET (Block Transfer Engine)
// ----------------------------------------------------------
void BTE_mode( uint8_t bte_operation, uint8_t rop_function )
{
	RAIO_SetRegister( BECR1, bte_operation | (rop_function<<4) );
}


// set color
// ----------------------------------------------------------
void Text_Background_Color( uint8_t color )
{ 
    RAIO_SetRegister( TBCR, color );
}
void Text_Foreground_Color( uint8_t color)
{ 
    RAIO_SetRegister( TFCR, color);
}


// clear screen
// ----------------------------------------------------------
void RAIO_clear_screen( void )
{
	// for more informations see RA8870 specification page 40
	//
	//    | Bit | Function
	//    |-----|-------------------------------------------------  
	//	  |  7  | 0 = stop clear   1 = start clear
	//    |  6  | 0 = fullwindow   1 = activewindow
	//    | 5-1 | NA
	//    |  0  | 0 = Memory clear with BTE background color   1 = Memory clear with font background color	
	//
	//      Reg 0x43 define font background color ( RRRGGGBB )
	//      Reg 0x60, 0x61, 0x62 define BTE background color ( BGCR0=red[4:0], BGCR1=green[5:0], BGCR2=blue[4:0] )
	
	RAIO_SetRegister( MCLR , 0x80 ); 
	TFT_wait_for_raio();
}


// print text
// ----------------------------------------------------------
void RAIO_print_text( uint16_t pos_x, uint16_t pos_y, unsigned char *str, uint8_t BG_color, uint8_t FG_color )
{
	union my_union number;
	
	// set cursor
	number.value = pos_x;
	RAIO_SetRegister( CURH0, number.split.low );
	RAIO_SetRegister( CURH1, number.split.high );
	
	number.value = pos_y;
	RAIO_SetRegister( CURV0, number.split.low );
	RAIO_SetRegister( CURV1, number.split.high );
	
	// set color 
	Text_Background_Color( BG_color );
	Text_Foreground_Color( FG_color );
	
	// set text mode
	RAIO_SetRegister( MWCR0, 0x80 );
	
	// write text to display
	TFT_RegWrite( MRWC );
	
	while ( *str != '\0' )
	{
		TFT_DataWrite( *str );
		++str;
		TFT_wait_for_raio();
	}
	
	TFT_wait_for_raio();
	
	// set graphic mode
	RAIO_SetRegister( MWCR0, 0x00 );
}


// set font size
// ----------------------------------------------------------
void RAIO_SetFontSizeFactor( uint8_t size )
{
	size = (size & 0x0f);
	RAIO_SetRegister ( FNCR1, size );
}



// set coordinates for drawing line and square
// ----------------------------------------------------------
void Set_Geometric_Coordinate(uint16_t X1, uint16_t Y1 ,uint16_t X2 ,uint16_t Y2 )
{ 
	union my_union number;

	number.value = X1;
	RAIO_SetRegister( DLHSR0, number.split.low );
	RAIO_SetRegister( DLHSR1, number.split.high );
	
	number.value = Y1;
	RAIO_SetRegister( DLVSR0, number.split.low  );
	RAIO_SetRegister( DLVSR1, number.split.high );

	number.value = X2;
	RAIO_SetRegister( DLHER0, number.split.low );
	RAIO_SetRegister( DLHER1, number.split.high );

	number.value = Y2;
	RAIO_SetRegister( DLVER0, number.split.low );
	RAIO_SetRegister( DLVER1, number.split.high );
}

// set coordinates for drawing circle
// ----------------------------------------------------------
void Set_Geometric_Coordinate_circle (uint16_t X1, uint16_t Y1 ,uint8_t rad )
{
	union my_union number;
		
	number.value = X1;
	RAIO_SetRegister( DCHR0, number.split.low );
	RAIO_SetRegister( DCHR1, number.split.high );
	
	number.value = Y1;
	RAIO_SetRegister( DCVR0, number.split.low  );
	RAIO_SetRegister( DCVR1, number.split.high );

	RAIO_SetRegister( DCRR, rad );
}


// set draw mode 
// ----------------------------------------------------------  
void RAIO_StartDrawing( int16_t whattodraw )
{
    switch( whattodraw ) // -> see DRAW_MODES
    {
        case CIRCLE_NONFILL:    {RAIO_SetRegister( DCR,  0x40 ); break;}
        case CIRCLE_FILL:       {RAIO_SetRegister( DCR,  0x60 ); break;}
        case SQUARE_NONFILL:    {RAIO_SetRegister( DCR,  0x90 ); break;}
        case SQUARE_FILL:       {RAIO_SetRegister( DCR,  0xB0 ); break;}
        case LINE:              {RAIO_SetRegister( DCR,  0x80 ); break;}
        default: break;
    }

    TFT_wait_for_raio();
}


// draw some basic geometrical forms
// ---------------------------------------------------------- 
void Draw_Line( uint16_t X1, uint16_t Y1 ,uint16_t X2 ,uint16_t Y2 )
{ 
	Set_Geometric_Coordinate( X1, Y1, X2, Y2 );
	RAIO_StartDrawing( LINE );
}

void Draw_Square( uint16_t X1, uint16_t Y1 ,uint16_t X2 ,uint16_t Y2 )
{ 
	Set_Geometric_Coordinate( X1, Y1, X2, Y2 );
	RAIO_StartDrawing( SQUARE_NONFILL );
}

void Draw_Circle( uint16_t X1, uint16_t Y1 ,uint8_t rad )
{ 
	Set_Geometric_Coordinate_circle ( X1, Y1, rad );
	RAIO_StartDrawing( CIRCLE_NONFILL );
}

