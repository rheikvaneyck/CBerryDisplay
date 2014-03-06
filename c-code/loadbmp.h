/*##############################################################*/
/* 																*/
/* From file		: examples.h	by Hagen Ploog and timo Pfander   */
/*																*/
/* Project	: TFT for Raspberry Pi Revision 2					*/
/* 																*/
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

#ifndef LOADBMP_H
#define LOADBMP_H

#include <stdint.h>
#include <math.h>


// load and depict a BMP file
// ---------------------------------------------
void CB_DepictBMP( char const *file_name );

#endif

