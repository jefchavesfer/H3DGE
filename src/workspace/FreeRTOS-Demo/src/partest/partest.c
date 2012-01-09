/*
    FreeRTOS V7.0.2 - Copyright (C) 2011 Real Time Engineers Ltd.
	

    ***************************************************************************
     *                                                                       *
     *    FreeRTOS tutorial books are available in pdf and paperback.        *
     *    Complete, revised, and edited pdf reference manuals are also       *
     *    available.                                                         *
     *                                                                       *
     *    Purchasing FreeRTOS documentation will not only help you, by       *
     *    ensuring you get running as quickly as possible and with an        *
     *    in-depth knowledge of how to use FreeRTOS, it will also help       *
     *    the FreeRTOS project to continue with its mission of providing     *
     *    professional grade, cross platform, de facto standard solutions    *
     *    for microcontrollers - completely free of charge!                  *
     *                                                                       *
     *    >>> See http://www.FreeRTOS.org/Documentation for details. <<<     *
     *                                                                       *
     *    Thank you for using FreeRTOS, and thank you for your support!      *
     *                                                                       *
    ***************************************************************************


    This file is part of the FreeRTOS distribution.

    FreeRTOS is free software; you can redistribute it and/or modify it under
    the terms of the GNU General Public License (version 2) as published by the
    Free Software Foundation AND MODIFIED BY the FreeRTOS exception.
    >>>NOTE<<< The modification to the GPL is included to allow you to
    distribute a combined work that includes FreeRTOS without being obliged to
    provide the source code for proprietary components outside of the FreeRTOS
    kernel.  FreeRTOS is distributed in the hope that it will be useful, but
    WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
    or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
    more details. You should have received a copy of the GNU General Public
    License and the FreeRTOS license exception along with FreeRTOS; if not it
    can be viewed here: http://www.freertos.org/a00114.html and also obtained
    by writing to Richard Barry, contact details for whom are available on the
    FreeRTOS WEB site.

    1 tab == 4 spaces!

    http://www.FreeRTOS.org - Documentation, latest information, license and
    contact details.

    http://www.SafeRTOS.com - A version that is certified for use in safety
    critical systems.

    http://www.OpenRTOS.com - Commercial support, development, porting,
    licensing and training services.
*/


/* Scheduler includes. */
#include "FreeRTOS.h"

/* Demo application includes. */
#include "partest.h"

/* Library includes. */
#include "xparameters.h"
#include "xgpio_l.h"
#include "xgpio.h"

/* Misc hardware specific definitions. */
#define partstALL_AS_OUTPUT	0x00
#define partstCHANNEL_1		0x01
#define partstMAX_8BIT_LED	0x07

/* GPIO driver struct */
static XGpio x8BitGpio, xPositionsGpio;

/* The outputs are split into two IO sections, these variables maintain the 
current value of either section. */
static unsigned portBASE_TYPE uxCurrentOutput8Bit, uxCurrentOutput5Bit;

/*-----------------------------------------------------------*/
/*
 * Setup the IO for the LED outputs.
 */
void vParTestInitialise( void )
{
	XGpio_Config x8BitConfig = {
			.DeviceId = 0,
			.BaseAddress = XPAR_LEDS_8BIT_BASEADDR,
			.InterruptPresent = 0,
			.IsDual = 0
	};

	XGpio_Config xPositionsConfig = {
			.DeviceId = 1,
			.BaseAddress = XPAR_LEDS_POSITIONS_BASEADDR,
			.InterruptPresent = 0,
			.IsDual = 0
	};

	XGpio_CfgInitialize( &x8BitGpio, &x8BitConfig, XPAR_LEDS_8BIT_BASEADDR );
	XGpio_CfgInitialize( &xPositionsGpio, &xPositionsConfig, XPAR_LEDS_POSITIONS_BASEADDR );

	/* Set both sets of LED's on the demo board to outputs. */
	XGpio_SetDataDirection( &x8BitGpio, partstCHANNEL_1, partstALL_AS_OUTPUT );
	XGpio_SetDataDirection( &xPositionsGpio, partstCHANNEL_1, partstALL_AS_OUTPUT );

	/* Start with all outputs off. */
	uxCurrentOutput8Bit = 0;
	XGpio_DiscreteWrite( &x8BitGpio, partstCHANNEL_1, 0x00 );
	uxCurrentOutput5Bit = 0;
	XGpio_DiscreteWrite( &xPositionsGpio, partstCHANNEL_1, 0x00 );
}
/*-----------------------------------------------------------*/

void vParTestSetLED( unsigned portBASE_TYPE uxLED, signed portBASE_TYPE xValue )
{
unsigned portBASE_TYPE *puxCurrentValue;
XGpio *pxGpio;

	portENTER_CRITICAL();
	{
		/* Which IO section does the LED being set/cleared belong to?  The
		8 bit or 5 bit outputs? */
		if( uxLED <= partstMAX_8BIT_LED )
		{
			pxGpio = &x8BitGpio;
			puxCurrentValue = &uxCurrentOutput5Bit;
		}	
		else
		{
			pxGpio = &xPositionsGpio;
			puxCurrentValue = &uxCurrentOutput8Bit;
			uxLED -= partstMAX_8BIT_LED;
		}

		/* Setup the bit mask accordingly. */
		uxLED = 0x01 << uxLED;

		/* Maintain the current output value. */
		if( xValue )
		{
			*puxCurrentValue |= uxLED;
		}
		else
		{
			*puxCurrentValue &= ~uxLED;
		}

		/* Write the value to the port. */
		XGpio_DiscreteWrite( pxGpio, partstCHANNEL_1, *puxCurrentValue );
	}
	portEXIT_CRITICAL();
}
/*-----------------------------------------------------------*/

void vParTestToggleLED( unsigned portBASE_TYPE uxLED )
{
unsigned portBASE_TYPE *puxCurrentValue;
XGpio *pxGpio;

	portENTER_CRITICAL();
	{
		/* Which IO section does the LED being toggled belong to?  The
		8 bit or 5 bit outputs? */
		if( uxLED <= partstMAX_8BIT_LED )
		{

			pxGpio = &x8BitGpio;
			puxCurrentValue = &uxCurrentOutput5Bit;
		}	
		else
		{
			pxGpio = &xPositionsGpio;
			puxCurrentValue = &uxCurrentOutput8Bit;
			uxLED -= partstMAX_8BIT_LED;
		}

		/* Setup the bit mask accordingly. */
		uxLED = 0x01 << uxLED;

		/* Maintain the current output value. */
		if( *puxCurrentValue & uxLED )
		{
			*puxCurrentValue &= ~uxLED;
		}
		else
		{
			*puxCurrentValue |= uxLED;
		}

		/* Write the value to the port. */
		XGpio_DiscreteWrite( pxGpio, partstCHANNEL_1, *puxCurrentValue );
	}
	portEXIT_CRITICAL();
}


