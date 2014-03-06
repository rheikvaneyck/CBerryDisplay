/*##############################################################*/
/* 																*/
/* File		: tft.c												*/
/*																*/
/* Project	: TFT for Raspberry Pi Revision 2					*/
/* 																*/
/* Date		: 2013-11-22   	    last update: 2013-12-06			*/
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
/*	This file controlls the communications between the 			*/
/*	Raspberry Pi and the TFT. The file initialized also the		*/
/*	GPIO Pins of the Raspberry Pi.								*/
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
/*																*/
/*																*/
/*																*/
/*##############################################################*/


#include <stdint.h>
#include <bcm2835.h>
#include "RAIO8870.h"
#include "tft.h"


// initialization of GPIO and SPI
// ----------------------------------------------------------
void TFT_init_board ( void )
{
	// *************** set the pins to be an output and turn them on
	
	bcm2835_gpio_fsel( OE, BCM2835_GPIO_FSEL_OUTP );
	bcm2835_gpio_write( OE, HIGH );
	
	bcm2835_gpio_fsel( RAIO_RST, BCM2835_GPIO_FSEL_OUTP );
	bcm2835_gpio_write( RAIO_RST, HIGH );

    bcm2835_gpio_fsel( RAIO_CS, BCM2835_GPIO_FSEL_OUTP );
	bcm2835_gpio_write( RAIO_CS, HIGH );
		
	bcm2835_gpio_fsel( RAIO_RS, BCM2835_GPIO_FSEL_OUTP );
	bcm2835_gpio_write( RAIO_RS, HIGH );

    bcm2835_gpio_fsel( RAIO_WR, BCM2835_GPIO_FSEL_OUTP );
    bcm2835_gpio_write( RAIO_WR, HIGH );
	
	bcm2835_gpio_fsel( RAIO_RD, BCM2835_GPIO_FSEL_OUTP );
	bcm2835_gpio_write( RAIO_RD, HIGH );
	
	
	// *************** now the inputs
	
	bcm2835_gpio_fsel( RAIO_WAIT, BCM2835_GPIO_FSEL_INPT );
	bcm2835_gpio_set_pud( RAIO_WAIT, BCM2835_GPIO_PUD_UP);
	
	bcm2835_gpio_fsel( RAIO_INT, BCM2835_GPIO_FSEL_INPT );
	bcm2835_gpio_set_pud( RAIO_INT, BCM2835_GPIO_PUD_UP);
	
		
	// *************** set pins for SPI
	
    bcm2835_gpio_fsel(MISO, BCM2835_GPIO_FSEL_ALT0); 
    bcm2835_gpio_fsel(MOSI, BCM2835_GPIO_FSEL_ALT0); 
    bcm2835_gpio_fsel(SCLK, BCM2835_GPIO_FSEL_ALT0);
    bcm2835_gpio_fsel(SPI_CE1, BCM2835_GPIO_FSEL_ALT0);
        
    // set the SPI CS register to the some sensible defaults
    volatile uint32_t* paddr = bcm2835_spi0 + BCM2835_SPI0_CS/8;
    bcm2835_peri_write( paddr, 0 ); // All 0s
    
    // clear TX and RX fifos
    bcm2835_peri_write_nb( paddr, BCM2835_SPI0_CS_CLEAR );
    
	bcm2835_spi_setBitOrder( BCM2835_SPI_BIT_ORDER_MSBFIRST );      
    bcm2835_spi_setDataMode( BCM2835_SPI_MODE0 );                 
    bcm2835_spi_setClockDivider( BCM2835_SPI_CLOCK_DIVIDER_2 ); 
    bcm2835_spi_chipSelect( BCM2835_SPI_CS1 );                      
    bcm2835_spi_setChipSelectPolarity( BCM2835_SPI_CS1, LOW );    
}


// hard reset of the graphic controller and the tft
// ----------------------------------------------------------
void TFT_hard_reset( void )
{
	bcm2835_gpio_write( RAIO_RST, LOW );
    bcm2835_delay( 10 );
 	bcm2835_gpio_write( RAIO_RST, HIGH );
 	bcm2835_delay( 1 );
}


// wait during raio is busy
// ----------------------------------------------------------
void TFT_wait_for_raio ( void )
{
	while ( !bcm2835_gpio_lev( RAIO_WAIT ) );
}


// write data via SPI to tft
// ----------------------------------------------------------
void TFT_SPI_data_out ( uint16_t data )
{
	union my_union number;
	char buffer[2];

	number.value = data;
	buffer[0] = (char) number.split.high;
	buffer[1] = (char) number.split.low;
	
	bcm2835_spi_writenb( &buffer[0], 2 );
}


// write byte to register
// ----------------------------------------------------------
void TFT_RegWrite( uint16_t reg )
{
	bcm2835_gpio_write( RAIO_RS, HIGH );               
	bcm2835_gpio_write( RAIO_CS, LOW ); 
    bcm2835_gpio_write( RAIO_WR, LOW ); 
    bcm2835_gpio_write( OE, LOW );
       
    TFT_SPI_data_out ( reg );
    
    bcm2835_gpio_write( RAIO_WR, HIGH ); 
 	bcm2835_gpio_write( RAIO_CS, HIGH ); 
	bcm2835_gpio_write( OE, HIGH );
}


// write byte to tft
// ----------------------------------------------------------
void TFT_DataWrite( uint16_t data )
{ 
	bcm2835_gpio_write( RAIO_RS, LOW ); 
	bcm2835_gpio_write( RAIO_CS, LOW ); 
    bcm2835_gpio_write( RAIO_WR, LOW ); 
    bcm2835_gpio_write( OE, LOW );
    
    TFT_SPI_data_out ( data );
        
    bcm2835_gpio_write( RAIO_WR, HIGH );
	bcm2835_gpio_write( RAIO_CS, HIGH ); 
	bcm2835_gpio_write( OE, HIGH );
};


// write 'count'-bytes to tft
// ----------------------------------------------------------
void TFT_DataMultiWrite( uint16_t *data, uint32_t count )
{
    volatile uint32_t* paddr = bcm2835_spi0 + BCM2835_SPI0_CS/4;
    volatile uint32_t* fifo  = bcm2835_spi0 + BCM2835_SPI0_FIFO/4;
	
	volatile uint32_t* gpio_set   = bcm2835_gpio + BCM2835_GPSET0/4;
	volatile uint32_t* gpio_clear = bcm2835_gpio + BCM2835_GPCLR0/4;
	
	uint32_t i;

	bcm2835_gpio_write( RAIO_RS, LOW ); 
	bcm2835_gpio_write( RAIO_CS, LOW ); 
	bcm2835_gpio_write( OE, LOW );
                  
	for( i=0; i<count; i++ )
	{
		// WR = 0
		*gpio_clear = ( 1 << RAIO_WRpin );			
		
		// activate SPI transfer
		*paddr |= BCM2835_SPI0_CS_TA;

		// fill the FIFO
		*fifo = (uint8_t)(data[i] >> 8);
		*fifo = (uint8_t)(data[i] & 0xFF);	
		
		// write fifo data to SPI TX buffer
		while (!(*paddr & BCM2835_SPI0_CS_DONE))
		{
			// clear SPI RX buffer
			*paddr |=BCM2835_SPI0_CS_CLEAR_RX;	
		};
		
		// deactivate SPI transfer
		*paddr &= ~BCM2835_SPI0_CS_TA;

		// WR = 1
		*gpio_set = ( 1 << RAIO_WRpin );			
	}
	
	bcm2835_gpio_write( RAIO_CS, HIGH ); 
	bcm2835_gpio_write( OE, HIGH );

}
