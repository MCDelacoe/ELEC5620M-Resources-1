/* 
 * LT24 Display Controller Driver
 * ------------------------------
 * Description: 
 * Driver for the LT24 Display Controller
 *
 * Company: University of Leeds
 * Author: T Carpenter
 *
 * Change Log:
 *
 * Date       | Changes
 * -----------+----------------------------------
 * 05/02/2017 | Creation of driver
 * 20/10/2017 | Update driver to match new styles
 * 01/04/2022 | Added functions from line 85
 */

#ifndef DE1SoC_LT24_H_
#define DE1SoC_LT24_H_

//Include required header files
#include <stdbool.h> //Boolean variable type "bool" and "true"/"false" constants.

//Error Codes
#define LT24_SUCCESS       0
#define LT24_ERRORNOINIT  -1
#define LT24_INVALIDSIZE  -4
#define LT24_INVALIDSHAPE -6

//Size of the LCD
#define LT24_WIDTH  240
#define LT24_HEIGHT 320

//Some basic colours
#define LT24_BLACK   (0x0000)
#define LT24_WHITE   (0xFFFF)
#define LT24_RED     (0x1F << 11)
#define LT24_GREEN   (0x1F << 6)
#define LT24_BLUE    (0x1F << 0)
#define LT24_YELLOW  (LT24_RED | LT24_GREEN)
#define LT24_CYAN    (LT24_GREEN | LT24_BLUE)
#define LT24_MAGENTA (LT24_BLUE | LT24_RED)


//Function to initialise the LCD
// - Returns 0 if successful
signed int LT24_initialise( unsigned int pio_base_address, unsigned int pio_hw_base_address );

//Check if driver initialised
// - returns true if initialised
bool LT24_isInitialised( void );

//Function for writing to LT24 Registers (using dedicated HW)
//You must check LT24_isInitialised() before calling this function
void LT24_write( bool isData, unsigned short value );

//Function for configuring LCD reset/power (using PIO)
//You must check LT24_isInitialised() before calling this function
void LT24_powerConfig( bool isOn );

//Function to clear display to a set colour
// - Returns 0 if successful
signed int LT24_clearDisplay(unsigned short colour);

//Function to convert Red/Green/Blue to RGB565 encoded colour value 
unsigned short LT24_makeColour( unsigned int R, unsigned int G, unsigned int B );

//Function to set the drawing window on the display
//  Returns 0 if successful
signed int LT24_setWindow( unsigned int xleft, unsigned int ytop, unsigned int width, unsigned int height);

//Generates test pattern on display
// - returns 0 if successful
signed int LT24_testPattern( void );

//Copy frame buffer to display
// - returns 0 if successful
signed int LT24_copyFrameBuffer(const unsigned short* framebuffer, unsigned int xleft, unsigned int ytop, unsigned int width, unsigned int height);

//Plot a single pixel on the LT24 display
// - returns 0 if successful
signed int LT24_drawPixel(unsigned short colour,unsigned int x,unsigned int y);

/* Added Functions ///////////////////////////////////////////////////////////////////
 * by Maxim Delacoe
 * on 1/4/2022
 */

/**
 * LT24_initGeometries
 *
 * Initialises the parameters for drawing columns on the LCD. The header
 * pins are regarded as the top of the screen.
 *
 * Inputs:
 * 	  graphWidth	width of the graph.
 * 	  graphHeight	height of the graph.
 * 	  columns		number of columns on the graph.
 * 	  windowHeight  height of the windows (rows) on the graph.
 *
 */
void LT24_initGeometries(unsigned short graphWidth, unsigned short graphHeight,
					     unsigned short columns, 	unsigned short windowHeight);

/**
 * LT24_drawWindow
 *
 * The function draws a single column of coloured rows on the LCD
 * display.
 *
 * Inputs:
 * 	  colour	specify the colour the window will take.
 * 	  x			the x coordinate of the corner of the window.
 * 	  y			the y coordinate of the corner of the window.
 *
 */
signed int LT24_drawWindow(unsigned short colour,unsigned int x,unsigned int y) ;

/**
 * LT24_drawColumn
 *
 * The function draws a single column of coloured rows on the LCD
 * display.
 *
 * Inputs:
 * 	  colours_ptr		a pointer to an array of values mapped to colour.
 * 	  columnNumbers		the total number of columns across the LCD.
 *
 */
void LT24_drawColumn(unsigned short *colours_ptr, unsigned int columnNumber);

/**
 * LT24_mapMagnitudeToColour
 *
 * The function maps a magnitude from 0 to MAX (e.g.,
 * 0 to 0xFFFF) to a colour. The colours range from blue, through
 * cyan-green-yellow, to red, indicating magnitude with colour.
 *
 * Inputs:
 * 	  mag	Specify the magnitude of the input.
 */
unsigned short LT24_mapMagnitudeToColour(unsigned int mag) ;

#endif /*DE1SoC_LT24_H_*/

