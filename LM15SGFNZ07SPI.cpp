/**************************************************************************
* LM15SGFNZ07.c                      
* Siemens M55 LCD (LM15SGFNZ07) library
*  
*  Mindogas 2011 | markauskas.mindaugas at gmail.com
*  merge by znsoft in 2014 from Marcin Selerowski (bialasik) LM15SGFNZ07 SPI Arduino Fast Library                       
*  the 13 pin is always SCLK 
*   and 11 pin always SDATA for fast SPI 
**************************************************************************/
#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif
#include <SPI.h>
#include "LM15SGFNZ07SPI.h"
#include <digitalWriteFast.h>

//Signals
//**************************
#define LCD_RS_ON       digitalWriteFast(LCD_RS,HIGH); 
#define LCD_RS_OFF      digitalWriteFast(LCD_RS,LOW);
#define LCD_CS_OFF      digitalWriteFast(LCD_CS,HIGH);
#define LCD_CS_ON       digitalWriteFast(LCD_CS,LOW);
#define LCD_RESET_ON    digitalWriteFast(LCD_RESET,LOW);
#define LCD_RESET_OFF   digitalWriteFast(LCD_RESET,HIGH);
//*************************** 

const unsigned char initData[139]  =    // LCD Inicialization data
{
0xF4 , 0x90 , 0xB3 , 0xA0 , 0xD0,  0xF0 , 0xE2 , 0xD4 , 0x70 , 0x66 ,
0xB2 , 0xBA , 0xA1 , 0xA3 , 0xAB , 0x94 , 0x95 , 0x95 , 0x95 , 0xF5 ,
0x90,  0xF1 , 0x00 , 0x10 , 0x22 , 0x30 , 0x45 , 0x50 , 0x68 , 0x70 ,
0x8A , 0x90 , 0xAC , 0xB0 , 0xCE , 0xD0,  0xF2 , 0x0F , 0x10 , 0x20 ,
0x30 , 0x43 , 0x50 , 0x66 , 0x70 , 0x89 , 0x90 , 0xAB , 0xB0 , 0xCD ,
0xD0,  0xF3 , 0x0E , 0x10 , 0x2F , 0x30 , 0x40 , 0x50 , 0x64 , 0x70 ,
0x87 , 0x90 , 0xAA , 0xB0 , 0xCB , 0xD0,  0xF4 , 0x0D , 0x10 , 0x2E ,
0x30 , 0x4F , 0x50,  0xF5 , 0x91,  0xF1 , 0x01 , 0x11 , 0x22 , 0x31 ,
0x43 , 0x51 , 0x64 , 0x71 , 0x86 , 0x91 , 0xA8 , 0xB1 , 0xCB , 0xD1,
0xF2 , 0x0F , 0x11 , 0x21 , 0x31 , 0x42 , 0x51 , 0x63 , 0x71 , 0x85 ,
0x91 , 0xA6 , 0xB1 , 0xC8 , 0xD1,  0xF3 , 0x0B , 0x11 , 0x2F , 0x31 ,
0x41 , 0x51 , 0x62 , 0x71 , 0x83 , 0x91 , 0xA4 , 0xB1 , 0xC6 , 0xD1,
0xF4 , 0x08 , 0x11 , 0x2B , 0x31 , 0x4F , 0x51 , 0x80 , 0x94, 0xF5 ,
0xA2 , 0xF4 , 0x60 , 0xF0 , 0x40 , 0x50 , 0xC0 , 0xF4 , 0x70
};

unsigned char font5x7 [][5] = 			//Font data
{
    { 0x00, 0x00, 0x00, 0x00, 0x00 },   // sp
    { 0x00, 0x00, 0x2f, 0x00, 0x00 },   // !
    { 0x00, 0x07, 0x00, 0x07, 0x00 },   // "
    { 0x14, 0x7f, 0x14, 0x7f, 0x14 },   // #
    { 0x24, 0x2a, 0x7f, 0x2a, 0x12 },   // $
    { 0x32, 0x34, 0x08, 0x16, 0x26 },   // %
    { 0x36, 0x49, 0x55, 0x22, 0x50 },   // &
    { 0x00, 0x05, 0x03, 0x00, 0x00 },   // '
    { 0x00, 0x1c, 0x22, 0x41, 0x00 },   // (
    { 0x00, 0x41, 0x22, 0x1c, 0x00 },   // )
    { 0x14, 0x08, 0x3E, 0x08, 0x14 },   // *
    { 0x08, 0x08, 0x3E, 0x08, 0x08 },   // +
    { 0x00, 0x00, 0x50, 0x30, 0x00 },   // ,
    { 0x10, 0x10, 0x10, 0x10, 0x10 },   // -
    { 0x00, 0x60, 0x60, 0x00, 0x00 },   // .
    { 0x20, 0x10, 0x08, 0x04, 0x02 },   // /
    { 0x3E, 0x51, 0x49, 0x45, 0x3E },   // 0
    { 0x00, 0x42, 0x7F, 0x40, 0x00 },   // 1
    { 0x42, 0x61, 0x51, 0x49, 0x46 },   // 2
    { 0x21, 0x41, 0x45, 0x4B, 0x31 },   // 3
    { 0x18, 0x14, 0x12, 0x7F, 0x10 },   // 4
    { 0x27, 0x45, 0x45, 0x45, 0x39 },   // 5
    { 0x3C, 0x4A, 0x49, 0x49, 0x30 },   // 6
    { 0x01, 0x71, 0x09, 0x05, 0x03 },   // 7
    { 0x36, 0x49, 0x49, 0x49, 0x36 },   // 8
    { 0x06, 0x49, 0x49, 0x29, 0x1E },   // 9
    { 0x00, 0x36, 0x36, 0x00, 0x00 },   // :
    { 0x00, 0x56, 0x36, 0x00, 0x00 },   // ;
    { 0x08, 0x14, 0x22, 0x41, 0x00 },   // <
    { 0x14, 0x14, 0x14, 0x14, 0x14 },   // =
    { 0x00, 0x41, 0x22, 0x14, 0x08 },   // >
    { 0x02, 0x01, 0x51, 0x09, 0x06 },   // ?
    { 0x32, 0x49, 0x59, 0x51, 0x3E },   // @
    { 0x7E, 0x11, 0x11, 0x11, 0x7E },   // A
    { 0x7F, 0x49, 0x49, 0x49, 0x36 },   // B
    { 0x3E, 0x41, 0x41, 0x41, 0x22 },   // C
    { 0x7F, 0x41, 0x41, 0x22, 0x1C },   // D
    { 0x7F, 0x49, 0x49, 0x49, 0x41 },   // E
    { 0x7F, 0x09, 0x09, 0x09, 0x01 },   // F
    { 0x3E, 0x41, 0x49, 0x49, 0x7A },   // G
    { 0x7F, 0x08, 0x08, 0x08, 0x7F },   // H
    { 0x00, 0x41, 0x7F, 0x41, 0x00 },   // I
    { 0x20, 0x40, 0x41, 0x3F, 0x01 },   // J
    { 0x7F, 0x08, 0x14, 0x22, 0x41 },   // K
    { 0x7F, 0x40, 0x40, 0x40, 0x40 },   // L
    { 0x7F, 0x02, 0x0C, 0x02, 0x7F },   // M
    { 0x7F, 0x04, 0x08, 0x10, 0x7F },   // N
    { 0x3E, 0x41, 0x41, 0x41, 0x3E },   // O
    { 0x7F, 0x09, 0x09, 0x09, 0x06 },   // P
    { 0x3E, 0x41, 0x51, 0x21, 0x5E },   // Q
    { 0x7F, 0x09, 0x19, 0x29, 0x46 },   // R
    { 0x46, 0x49, 0x49, 0x49, 0x31 },   // S
    { 0x01, 0x01, 0x7F, 0x01, 0x01 },   // T
    { 0x3F, 0x40, 0x40, 0x40, 0x3F },   // U
    { 0x1F, 0x20, 0x40, 0x20, 0x1F },   // V
    { 0x3F, 0x40, 0x38, 0x40, 0x3F },   // W
    { 0x63, 0x14, 0x08, 0x14, 0x63 },   // X
    { 0x07, 0x08, 0x70, 0x08, 0x07 },   // Y
    { 0x61, 0x51, 0x49, 0x45, 0x43 },   // Z
    { 0x00, 0x7F, 0x41, 0x41, 0x00 },   // [
    { 0x55, 0x2A, 0x55, 0x2A, 0x55 },   // 55
    { 0x00, 0x41, 0x41, 0x7F, 0x00 },   // ]
    { 0x04, 0x02, 0x01, 0x02, 0x04 },   // ^
    { 0x40, 0x40, 0x40, 0x40, 0x40 },   // _
    { 0x00, 0x01, 0x02, 0x04, 0x00 },   // '
    { 0x20, 0x54, 0x54, 0x54, 0x78 },   // a
    { 0x7F, 0x48, 0x44, 0x44, 0x38 },   // b
    { 0x38, 0x44, 0x44, 0x44, 0x20 },   // c
    { 0x38, 0x44, 0x44, 0x48, 0x7F },   // d
    { 0x38, 0x54, 0x54, 0x54, 0x18 },   // e
    { 0x08, 0x7E, 0x09, 0x01, 0x02 },   // f
    { 0x0C, 0x52, 0x52, 0x52, 0x3E },   // g
    { 0x7F, 0x08, 0x04, 0x04, 0x78 },   // h
    { 0x00, 0x44, 0x7D, 0x40, 0x00 },   // i
    { 0x20, 0x40, 0x44, 0x3D, 0x00 },   // j
    { 0x7F, 0x10, 0x28, 0x44, 0x00 },   // k
    { 0x00, 0x41, 0x7F, 0x40, 0x00 },   // l
    { 0x7C, 0x04, 0x18, 0x04, 0x78 },   // m
    { 0x7C, 0x08, 0x04, 0x04, 0x78 },   // n
    { 0x38, 0x44, 0x44, 0x44, 0x38 },   // o
    { 0x7C, 0x14, 0x14, 0x14, 0x08 },   // p
    { 0x08, 0x14, 0x14, 0x18, 0x7C },   // q
    { 0x7C, 0x08, 0x04, 0x04, 0x08 },   // r
    { 0x48, 0x54, 0x54, 0x54, 0x20 },   // s
    { 0x04, 0x3F, 0x44, 0x40, 0x20 },   // t
    { 0x3C, 0x40, 0x40, 0x20, 0x7C },   // u
    { 0x1C, 0x20, 0x40, 0x20, 0x1C },   // v
    { 0x3C, 0x40, 0x30, 0x40, 0x3C },   // w
    { 0x44, 0x28, 0x10, 0x28, 0x44 },   // x
    { 0x0C, 0x50, 0x50, 0x50, 0x3C },   // y
    { 0x44, 0x64, 0x54, 0x4C, 0x44 },   // z
	};
	
	
	
//***************************************************************
//Initializing the MLCD Class
//***************************************************************	
LM15SGFNZ07SPI::LM15SGFNZ07SPI( byte RS, byte RESET, byte CS)
{
	this->LCD_RS = RS;
	this->LCD_RESET = RESET;
	this->LCD_CS = CS;
}

//***************************************************************
//Initializing the LCD
//***************************************************************
void LM15SGFNZ07SPI::init_lcd(void)
{

// Decrarating PORTS as OUTPUTS
    pinMode(LCD_RS, OUTPUT);
    pinMode(LCD_RESET, OUTPUT);
    pinMode(LCD_CS, OUTPUT);
    SPI.begin();
   unsigned short j;
   LCD_CS_OFF;
   LCD_RESET_OFF;
   LCD_RS_OFF;
   LCD_RESET_ON;   
   delay(10);
   LCD_RESET_OFF;
   delay(10);
    LCD_RS_ON;      
    LCD_CS_ON;      
   for(j = 0; j < 139; j++)
   {
	   send_lcd(initData[j]);   // Sends bytes to LCD
   }
   delay(1);
    LCD_CS_OFF;      

   delay(1);
    LCD_CS_ON;      

    send_lcd(0xF0);
    send_lcd(0x81);
    send_lcd(0xF4);
    send_lcd(0xB3);
    send_lcd(0xA0);

    send_lcd(0xF0);
    send_lcd(0x06);
    send_lcd(0x10);
    send_lcd(0x20);
    send_lcd(0x30);
    send_lcd(0xF5);
    send_lcd(0x0F);
    send_lcd(0x1C);
    send_lcd(0x2F);
    send_lcd(0x34);
    LCD_CS_OFF;     
    LCD_RS_OFF;     



}

//***************************************************************
//Contrast setting:
//***************************************************************
void LM15SGFNZ07SPI::contrast_lcd(unsigned char contrast)
{
   LCD_RS_ON;
   LCD_CS_ON;
   send_lcd(0xF4);
   send_lcd(0xB0 | (contrast >> 4));
   send_lcd(0xA0 | (contrast & 0x0F));
   LCD_CS_OFF;
}


//***************************************************************
//LCD window
//***************************************************************
void LM15SGFNZ07SPI::window_lcd(unsigned char xstart, unsigned char ystart, unsigned char xend, unsigned char yend)
{
    xstart <<= 1;
    xstart += 6;
    xend <<= 1;
    xend += 7;

    LCD_RS_ON;
    LCD_CS_ON;
    send_lcd(0xF0);
    send_lcd(0x00 | (xstart & 0x0F));
    send_lcd(0x10 | (xstart >> 4));
    send_lcd(0x20 | (ystart & 0x0F));
    send_lcd(0x30 | (ystart >> 4));
    send_lcd(0xF5);
    send_lcd(0x00 | (xend & 0x0F));
    send_lcd(0x10 | (xend >> 4));
    send_lcd(0x20 | (yend & 0x0F));
    send_lcd(0x30 | (yend >> 4));
    LCD_CS_OFF;
    LCD_RS_OFF;

}

//***************************************************************
//Setting the cursor
//***************************************************************
void LM15SGFNZ07SPI::cursor_lcd(unsigned char row, unsigned char col)
{
   window_lcd(row,col, 100, 79);
}
//***************************************************************
//Set the pixel color
//***************************************************************
void LM15SGFNZ07SPI::pixel_lcd(unsigned char x, unsigned char y,unsigned int color)
{
   cursor_lcd(x,79-y);
   LCD_RS_OFF;
   LCD_CS_ON;
   //SPI1_16bit;
   send_lcd((color & 0x0FFF) >> 8 );
   send_lcd(color & 0x00FF);
   //SPI1_8bit;
   LCD_CS_OFF;
}

//***************************************************************
//Cleaning the screen color
//***************************************************************
void LM15SGFNZ07SPI::clear_lcd(unsigned int col)
{
   unsigned int i;

   cursor_lcd(0,0);
   LCD_RS_OFF;
   LCD_CS_ON;
   for(i = 0; i < 8080; i++)
   {
      send_lcd(col >> 8);
      send_lcd(col & 0x00FF);
   }
   cursor_lcd(0,0);
   LCD_CS_OFF;
}



//***************************************************************
//Draws a line
//***************************************************************
void LM15SGFNZ07SPI::line_lcd(unsigned char x1,unsigned char x2, unsigned char y1, unsigned char y2,  unsigned int color)
   {
  
   int dx, dy, stepx, stepy, fraction;
    
    // 
    dy = y2 - y1;
    dx = x2 - x1;

    //when dy is negative
    if ( dy < 0 )
    {
        dy    = -dy;
        stepy = -1;
    }
    else
    {
        stepy = 1;
    }

    //where dx is negative
    if ( dx < 0 )
    {
        dx    = -dx;
        stepx = -1;
    }
    else
    {
        stepx = 1;
    }
	

    dx <<= 1;
    dy <<= 1;

    //Draw a pixel on the initial position of property as
    pixel_lcd( x1, y1, color);
   

    // The line of position to the end
    if ( dx > dy )
    {
        // In the frame
        fraction = dy - ( dx >> 1);
        while ( x1 != x2 )
        {
            if ( fraction >= 0 )
            {
                y1 += stepy;
                fraction -= dx;
            }
            x1 += stepx;
            fraction += dy;

            // Draw a calculated point
           pixel_lcd( x1, y1, color);
          
        }
    }
    else
    {
        //In the frame
        fraction = dx - ( dy >> 1);
        while ( y1 != y2 )
        {
            if ( fraction >= 0 )
            {
                x1 += stepx;
                fraction -= dy;
            }
            y1 += stepy;
            fraction += dx;

            // Draw the calculated point
            pixel_lcd( x1, y1, color);
           
        }
    }
 
   }
   
//***************************************************************
//Draws a box in a given color
//***************************************************************
   
void LM15SGFNZ07SPI::area_lcd(unsigned char x,unsigned char y, unsigned char height, unsigned char width, unsigned int color)
	{
	int yy=y+height;
	
	for (; y<yy; y++)
	{
		line_lcd(x, x+width, y, y, color);
	}

   
}

//***************************************************************
//Draws a frame
//***************************************************************
void LM15SGFNZ07SPI::frame_lcd(unsigned char x,unsigned char y, unsigned char h, unsigned char w, unsigned char s, unsigned int color)
{
  int x2=x+w;
  int y2=y+h;
  unsigned int i;
  for(i=1; i<=s; i++)
  {
    line_lcd(x, x, y, y2, color);
    line_lcd(x2, x2, y, y2, color);
    line_lcd(x, x2, y, y, color);
    line_lcd(x, x2, y2, y2, color);
    x++;
    y++;
    x2--;
    y2--;
  }
}

//***************************************************************
//He writes character
//Arguments: Character, position x, y, size 1 or 2, the color of the text, this text color (if "-1" it writes the same font without this text)
//***************************************************************
void LM15SGFNZ07SPI::chr_lcd(  char chr, unsigned char x, unsigned char y, unsigned int size, unsigned int color_font, unsigned int color)
{

    char buf;
	unsigned int j, l, k, xpoz, ypoz;
	xpoz=(x*6*size)-6*size;
	ypoz=79-(y*8*size) +7*size;	
   
    if(size==1) //if the size of 1
	{
		unsigned char i;
		for(i=0;i<5;i++)//for 5 elements character array
		{
			buf=font5x7[chr - 32][i];//buf assign one of 5 elements
			j=0;
			while(j<8)
			{
				if(buf & _BV(j))//If another element is 1
				{
					pixel_lcd(xpoz+i,ypoz-j, color_font);//draws a pixel
					j++;
				}
				else if(color==-1)
				{
					j++;
				}
				else
				{
					pixel_lcd(xpoz+i,ypoz-j, color);//draws a pixel
					j++;
				}
			}
		}
	}
	else if(size ==2)
    {
	l=0;
	int i;
	for (i = 0; i < 5; i++ )
    {
		k=0;
		j=0;
		l+=2;
		buf=font5x7[chr - 32][i];
	    while(j<8)
	     {
		
			 if(buf & _BV(j))
			 {
				 pixel_lcd(xpoz+l,ypoz-k, color_font);
				 pixel_lcd(xpoz+l,ypoz-k+1, color_font);
				 pixel_lcd(xpoz+l+1,ypoz-k, color_font);
				 pixel_lcd(xpoz+l+1,ypoz-k+1, color_font);
				 k+=2;
				 j++;
			 }
			 else if(color==-1)
			 {
				 k+=2;
				 j++;
			 }
			 else
			 {
				 pixel_lcd(xpoz+l,ypoz-k, color);
				 pixel_lcd(xpoz+l,ypoz-k+1, color);
				 pixel_lcd(xpoz+l+1,ypoz-k, color);
				 pixel_lcd(xpoz+l+1,ypoz-k+1, color);
				 k+=2;
				 j++;
			}
		 
	     
		}      
      }
	}	

}
//***************************************************************
//Writes text
//Arguments: text "", position x, y, size 1 or 2, the color of the text, this text color (if "-1" it writes the same font without this text)
//***************************************************************

void LM15SGFNZ07SPI::str_lcd(char *str, unsigned char x, unsigned char y, unsigned int size, unsigned int color_font, unsigned int color)
{
Serial.println(str);
    while((*str != 0) && (*str != 10)) 
	{	
        chr_lcd(*str++, x, y, size, color_font, color);
		x++;
		if(size==1 && x>17)////If mae letters and come off the screen
		{
			y++;
			x=1;
		}
		else if(size==2 && x>8)//If large letters and come off the screen
		{		
			y++;
			x=1;
		}
		
		
    }
	
}
//***************************************************************
//Sends RAW data to LCD
//***************************************************************
void LM15SGFNZ07SPI::send_lcd(unsigned char data)
{
	SPI.transfer(data); 
}

