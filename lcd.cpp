#include <string.h>
#include "lcd.h"

void HD44780::OutNibble(unsigned char nibbleToWrite) {
	if (nibbleToWrite & 0x01)
		LCD_DB4_PORT |= LCD_DB4;
	else
		LCD_DB4_PORT &= ~LCD_DB4;

	if (nibbleToWrite & 0x02)
		LCD_DB5_PORT |= LCD_DB5;
	else
		LCD_DB5_PORT &= ~LCD_DB5;

	if (nibbleToWrite & 0x04)
		LCD_DB6_PORT |= LCD_DB6;
	else
		LCD_DB6_PORT &= ~LCD_DB6;

	if (nibbleToWrite & 0x08)
		LCD_DB7_PORT |= LCD_DB7;
	else
		LCD_DB7_PORT &= ~LCD_DB7;
}
//-------------------------------------------------------------------------------------------------
//
// Function to write a byte to the display (without distinguishing between instruction/data).
//
//-------------------------------------------------------------------------------------------------
void HD44780::Write(unsigned char dataToWrite) {
	LCD_E_PORT |= LCD_E;
	OutNibble(dataToWrite >> 4);
	LCD_E_PORT &= ~LCD_E;
	LCD_E_PORT |= LCD_E;
	OutNibble(dataToWrite);
	LCD_E_PORT &= ~LCD_E;
	_delay_us(50);
}
//-------------------------------------------------------------------------------------------------
//
// Function to write a command to the display.
//
//-------------------------------------------------------------------------------------------------
void HD44780::WriteCommand(unsigned char commandToWrite) {
	LCD_RS_PORT &= ~LCD_RS;
	Write(commandToWrite);
}
//-------------------------------------------------------------------------------------------------
//
// Function to write data to the display memory.
//
//-------------------------------------------------------------------------------------------------
void HD44780::WriteData(unsigned char dataToWrite) {
	LCD_RS_PORT |= LCD_RS;
	Write(dataToWrite);
	if (position_x == 15 && position_y == 0) {
		position_x = 0;
		position_y = 1;
	} else if (position_x == 15) {
		position_x = 0;
		position_y = 0;
	}
	++position_x;
}
//-------------------------------------------------------------------------------------------------
//
// Function to display text on the screen.
//
//-------------------------------------------------------------------------------------------------
void HD44780::WriteText(const char *text) {
    while (*text) {
        // Write the current character to the display
        WriteData(*text++);
        // Increment the x position for the next character
        ++position_x;
    }
}
//-------------------------------------------------------------------------------------------------
//
// Function to scroll through text on the display. 
//
//-------------------------------------------------------------------------------------------------
void HD44780::ScrollText(const char *text) {
    uint8_t textLength = strlen(text);
        // Scroll the text to the left
        for (uint8_t i = 0; i < textLength; ++i) {
            Clear();
            GoTo(0, 0);
            WriteText(text + i);
            _delay_ms(400);
        }
}
//-------------------------------------------------------------------------------------------------
//
// Function to write blinking text on the display. 
//
//-------------------------------------------------------------------------------------------------
void HD44780::BlinkText(const char *text, uint8_t blinkCount) {
    for (uint8_t count = 0; count < blinkCount; ++count) {
        Clear();
        _delay_ms(600);
        WriteText(text);
        _delay_ms(600);
		Clear();
    }
}
//-------------------------------------------------------------------------------------------------
//
// Function to set screen coordinates.
//
//-------------------------------------------------------------------------------------------------
void HD44780::GoTo(unsigned char dx, unsigned char dy) {
	position_x = dx;
	position_y = dy;
	WriteCommand(HD44780_DDRAM_SET | (dx + (0x40 * dy)));
}
//-------------------------------------------------------------------------------------------------
//
// Function to clear the display screen.
//
//-------------------------------------------------------------------------------------------------
void HD44780::Clear(void) {
	WriteCommand(HD44780_CLEAR);
	_delay_ms(2);
}
//-------------------------------------------------------------------------------------------------
//
// Function to restore the initial coordinates of the display.
//
//-------------------------------------------------------------------------------------------------
void HD44780::Home(void) {
	WriteCommand(HD44780_HOME);
	_delay_ms(2);
}
//-------------------------------------------------------------------------------------------------
//
// Procedure for the initialization of the HD44780 controller.
//
//-------------------------------------------------------------------------------------------------
void HD44780::Initalize(void) {
	unsigned char i;

	LCD_DB4_DIR |= LCD_DB4; // Configure the direction of the port pins
	LCD_DB5_DIR |= LCD_DB5; //
	LCD_DB6_DIR |= LCD_DB6; //
	LCD_DB7_DIR |= LCD_DB7; //
	LCD_E_DIR |= LCD_E;   //
	LCD_RS_DIR |= LCD_RS;  //
	_delay_ms(15);  // Wait for the power supply voltage to stabilize
	// Set RS (Register Select) and E (Enable) lines to low
	LCD_RS_PORT &= ~LCD_RS; 
	LCD_E_PORT &= ~LCD_E;  

	// Execute a sequence of commands to initialize the LCD
	for (i = 0; i < 3; i++)
			{
		LCD_E_PORT |= LCD_E; // E = 1
		OutNibble(0x03); // 8-bit mode
		LCD_E_PORT &= ~LCD_E; // E = 0
		_delay_ms(5); // Wait for 5ms
	}

	LCD_E_PORT |= LCD_E; // E = 1
	OutNibble(0x02); // 4-bit mode
	LCD_E_PORT &= ~LCD_E; // E = 0

	_delay_ms(1); // Wait for 1ms
	WriteCommand(
	HD44780_FUNCTION_SET | HD44780_FONT5x7 | HD44780_TWO_LINE | HD44780_4_BIT); // Configure LCD function: 4-bit interface, 2 lines, 5x7 character font
	WriteCommand(HD44780_DISPLAY_ONOFF | HD44780_DISPLAY_OFF); // Turn off the display
	WriteCommand(HD44780_CLEAR);  // Clear the content of DDRAM
	_delay_ms(2);
	WriteCommand(
	HD44780_ENTRY_MODE | HD44780_EM_SHIFT_CURSOR | HD44780_EM_INCREMENT);  // Set the entry mode: increment the address, shift the cursor
	WriteCommand(
			HD44780_DISPLAY_ONOFF | HD44780_DISPLAY_ON | HD44780_CURSOR_OFF
					| HD44780_CURSOR_NOBLINK); // Turn on the display without cursor and blinking
}

// Input:
//     location: location where you want to store
//               0,1,2,....7
//     ptr: Pointer to pattern data
//
// Usage:
//     pattern[8]={0x04,0x0E,0x0E,0x0E,0x1F,0x00,0x04,0x00};
//     LCD_build(1,pattern);
//
// LCD Ports are same as discussed in previous sections

void HD44780::BuildFont(unsigned char location, unsigned char *ptr) {
	unsigned char i;
	if (location < 8) {
		WriteCommand(0x40 + (location * 8));
		for (i = 0; i < 8; i++)
			WriteData(ptr[i]);
	}
}
void HD44780::BuildFonts(unsigned char** dictionary) {
	int k;
	for (k = 0; k < 8; k++)
		BuildFont(k, *(dictionary + k));

}
/**
 *@param customFont - 0,1...7
 */
void HD44780::DrawCustomFont(unsigned char customFont) {
	WriteData(customFont);
}
void HD44780::DrawCustomFont(unsigned char customFont, int pass){
	WriteData(customFont);
	if(pass+position_x <16)
		GoTo(position_x+pass,position_y);
	else
		GoTo(pass+position_x - 16, (position_y+1)%2 );

}