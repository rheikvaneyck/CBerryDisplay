/*##############################################################*/
/* 																															*/
/* From file: tft_test.c	bv																		*/
/* Author	: 	Hagen Ploog   																		*/
/*		  	  	Kai Gillmann																			*/
/*		  	  	Timo Pfander																			*/
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
/*##############################################################*/

#include <bcm2835.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "tft.h"
#include "RAIO8870.h"
#include "bmp.h"
#include "loadbmp.h"


// load and depict a BMP file
// ---------------------------------------------
void CB_DepictBMP( char const *file_name  )
{
	uint16_t picture[1][ PICTURE_PIXELS ];
	
	Read_bmp2memory ( file_name, &picture[0][ PICTURE_PIXELS-1 ] );
	RAIO_Write_Picture ( &picture[0][0], PICTURE_PIXELS );	
}

int main( int argc, char **argv )
{	
	if(!argv[1])
	 return 1;
		
	char *my_filename = malloc(strlen(argv[1]));
	strcpy(my_filename,argv[1]);
	printf("using: %s\n", my_filename);
		
	if (!bcm2835_init())
	return 1;
	
	
	TFT_init_board();
	TFT_hard_reset();
	RAIO_init();
	
	// depict a BMP file
	// ---------------------------------------------	
	CB_DepictBMP( &my_filename[0] );
    	
	bcm2835_close();
  
   	return 0;
}



	
	
	
	
	
	
	
	
	
	
	
	
	

