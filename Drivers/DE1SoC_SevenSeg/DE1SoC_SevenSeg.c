/*
 * DE1SoC_SevenSeg.c
 *
 *  Created on: 12 Feb 2021
 *      Author: Harry Clegg
 *  Modified on: 20 Feb 2022
 *      by    : Maxim Delacoe
 *
 *  Contains the drivers for the seven
 *  segment displays (SSDs), displaying
 *  numbers in hex or decimal over single
 *  or pairs of SSDs. Developed for the
 *  ARM Cortex A9 on the DE1-SoC 5CSEMA5F31C6
 */

#include "DE1SoC_SevenSeg.h"

// Base addresses of the seven segment display peripherals.
volatile unsigned char *sevenseg_base_lo_ptr = (unsigned char *)0xFF200020; // Hexes 0 to 3
volatile unsigned char *sevenseg_base_hi_ptr = (unsigned char *)0xFF200030; // Hexes 4 to 5

// There are four HEX displays attached to the low (first) address.
#define SEVENSEG_N_DISPLAYS_LO 4

// There are two HEX displays attached to the high (second) address.
#define SEVENSEG_N_DISPLAYS_HI 2

void DE1SoC_SevenSeg_Write(unsigned int display, unsigned char value) {
    // Select between the two addresses so that the higher level functions
    // have a seamless interface.
    if (display < SEVENSEG_N_DISPLAYS_LO) {
        // If we are targeting a low address, use byte addressing to access
        // directly.
        sevenseg_base_lo_ptr[display] = value;
    } else {
        // If we are targeting a high address, shift down so byte addressing
        // works.
        display = display - SEVENSEG_N_DISPLAYS_LO;
        sevenseg_base_hi_ptr[display] = value;
    }
}

void DE1SoC_SevenSeg_SetSingle(unsigned int display, unsigned int value) {
	// If 0 <= value <= 15, use the look up table to find the bit map value.
	if (value < 0x10) {
		// The look up table (array) maps values to their seven segment
		// display bit maps. 'static' prevents the array from being allocated
		// and initialised on every call; 'const' prevents the values from
		// being changed.
		static const int look_up[16] = {
			0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07,
			0x7F, 0x67, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71
		};
		value = look_up[value] ; // Update value to represent the bit map.
		DE1SoC_SevenSeg_Write(display, value) ; // Display the number.

	// Else, (i.e., value > 15) display a dash because the value is outside the acceptable range.
	} else {
		value = 0x40 ; // Update value to represent a dash.
		DE1SoC_SevenSeg_Write(display, value) ; // Display the number.
	}
}

void DE1SoC_SevenSeg_SetDoubleHex(unsigned int display, unsigned int value) {
	// If 0 <= value <= 255, display the hex value over two seven segment displays (SSDs).
	if (value < 0x100) {
		// Using bitwise AND, obtain  value's 4 LSBs.
		unsigned int least_significant_digit =  value & 0x0F ;
		// Using bitwise AND and shift, obtain  value's 4 MSBs.
		unsigned int  most_significant_digit = (value & 0xF0) >> 4 ;
		// Display the Hex character of the LSBs on 'display' (right)
		// and the MSBs on 'display +1' (left).
		DE1SoC_SevenSeg_SetSingle(display,  least_significant_digit) ;
		DE1SoC_SevenSeg_SetSingle(display+1, most_significant_digit) ;

	// Else (i.e., value > 255), display dashes. Input > 15 will cause
	// DE1SoC_SevenSeg_SetSingle to display dashes.
	} else {
		DE1SoC_SevenSeg_SetSingle(display,   0x10) ;
		DE1SoC_SevenSeg_SetSingle(display+1, 0x10) ;
	}
}

void DE1SoC_SevenSeg_SetDoubleDec(unsigned int display, unsigned int value) {
	// If 0 <= value <= 99, display the decimal value over two seven segment displays (SSDs).
	if (value < 100) {
		// Convert the least significant digit (LSD) from hex to decimal.
		unsigned int least_significant_digit = value % 10 ;
		// Convert the most significant digit (MSD) from hex to decimal.
		unsigned int  most_significant_digit = (value - least_significant_digit) / 10 ;
		most_significant_digit = most_significant_digit % 10 ;
		// Display the decimal character of the LSD on 'display' (right)
		// and the MSD on 'display +1' (left).
		DE1SoC_SevenSeg_SetSingle(display,  least_significant_digit) ;
		DE1SoC_SevenSeg_SetSingle(display+1, most_significant_digit) ;

	// Else (i.e., value > 99), display dashes. Input > 15 will cause
	// DE1SoC_SevenSeg_SetSingle to display dashes.
	} else {
		DE1SoC_SevenSeg_SetSingle(display,   0x10) ;
		DE1SoC_SevenSeg_SetSingle(display+1, 0x10) ;
	}
}
