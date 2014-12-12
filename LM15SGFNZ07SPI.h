/**************************************************************************
* LM15SGFNZ07.h                      
* Siemens M55 LCD (LM15SGFNZ07) library
*  E
*  Mindogas 2011 | markauskas.mindaugas at gmail.com
*                       
**************************************************************************/
#ifndef LM15SGFNZ07SPI _h
#define LM15SGFNZ07SPI _h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

//Colors definitions
#define WHITE 0xFFF
#define BLACK 0x000
#define RED 0xF00
#define GREEN 0x0F0
#define BLUE 0x00F
#define CYAN 0x0FF
#define MAGENTA 0xF0F
#define YELLOW 0xFF0
#define BROWN 0xB22
#define ORANGE 0xFA0
#define PINK 0xF9E
//***************************

class LM15SGFNZ07SPI
{
  public:
    LM15SGFNZ07SPI( byte RS, byte RESET, byte CS);
   void init_lcd(void);
   void cursor_lcd(unsigned char row, unsigned char col);
   void clear_lcd(unsigned int col);
   void contrast_lcd(unsigned char contrast);
   void window_lcd(unsigned char xstart, unsigned char ystart, unsigned char xend, unsigned char yend);
   void pixel_lcd(unsigned char x, unsigned char y,unsigned int color);
   void send_lcd(unsigned char dat);
   void line_lcd(unsigned char x1,unsigned char x2, unsigned char y1, unsigned char y2, unsigned int color);
   void area_lcd(unsigned char x1,unsigned char x2, unsigned char y1, unsigned char y2, unsigned int color);
   void frame_lcd(unsigned char x,unsigned char y, unsigned char height, unsigned char width, unsigned char size, unsigned int color);
   void chr_lcd( char chr, unsigned char x, unsigned char y, unsigned int size, unsigned int color_font, unsigned int color);
   void str_lcd(char *str, unsigned char x, unsigned char y, unsigned int size, unsigned int color_font,unsigned int color);
  private:
//	byte LCD_SDATA; = 11
//	byte LCD_SCLK;  = 13
	byte LCD_RS;
	byte LCD_RESET;
	byte LCD_CS;
};

#endif
