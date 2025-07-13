#include <stm32f031x6.h>
#include "display.h"
#include <stdint.h>
#include <stdlib.h>

#define PLATFORM_WIDTH  50
#define PLATFORM_HEIGHT 10
#define CHARACTER_WIDTH 12
#define CHARACTER_HEIGHT 16

// screen h 160. w 128

void initClock(void);
void initSysTick(void);
void SysTick_Handler(void);
void delay(volatile uint32_t dly);
void setupIO();
int isInside(uint16_t x1, uint16_t y1, uint16_t w, uint16_t h, uint16_t px, uint16_t py);
void enablePullUp(GPIO_TypeDef *Port, uint32_t BitNumber);
void pinMode(GPIO_TypeDef *Port, uint32_t BitNumber, uint32_t Mode);
int rightPressed(void);
int leftPressed(void);
int downPressed(void);
int upPressed(void);
void upLadder(void);
uint32_t prbs(void);

volatile uint32_t milliseconds;

const uint16_t deco1[]=
{
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,16142,0,0,0,0,0,0,0,0,0,0,16142,16142,16142,9293,0,0,0,0,0,0,0,16142,16142,9293,9293,9293,9293,0,0,0,0,0,0,0,16142,9293,9293,0,9293,9293,0,0,0,0,0,16142,16142,9293,9293,9293,9293,9293,0,0,0,0,0,0,16142,9293,9293,9293,65535,0,9293,0,0,0,0,0,16142,9293,9293,0,0,0,9293,16142,0,0,0,16142,9293,9293,54815,0,0,0,9293,9293,16142,0,16142,9293,9293,9293,54815,0,0,0,9293,9293,16142,16142,16142,9293,9293,9293,9293,54815,0,0,0,9293,9293,9293,9293,9293,9293,9293,54815,54815,0,0,0,9293,9293,9293,9293,9293,9293,9293,54815,0,0,0,0,0,9293,9293,54815,54815,9293,9293,54815,0,0,0,0,0,9293,9293,0,0,9293,9293,0,0,0,0,0,0,9293,9293,9293,0,0,9293,9293,0,0,0,
};
const uint16_t deco2[]= 
	{
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,16142,0,0,0,0,0,0,0,0,0,0,16142,16142,16142,9293,0,0,0,0,0,0,0,16142,16142,9293,9293,9293,9293,0,0,0,0,0,0,0,16142,9293,9293,0,9293,9293,0,0,0,0,0,16142,16142,9293,9293,9293,9293,9293,0,0,0,0,0,0,16142,9293,9293,9293,65535,0,9293,0,0,0,0,0,16142,9293,9293,0,0,0,9293,16142,0,0,0,16142,9293,9293,54815,0,0,0,9293,9293,16142,0,16142,9293,9293,9293,54815,0,0,0,9293,9293,16142,16142,16142,9293,9293,9293,9293,54815,0,0,0,9293,9293,9293,9293,9293,9293,9293,54815,54815,0,0,0,9293,9293,9293,9293,9293,9293,9293,54815,0,0,0,0,0,9293,9293,54815,54815,9293,9293,54815,0,0,0,0,9293,9293,0,0,0,0,9293,9293,0,0,0,0,9293,9293,9293,0,0,0,9293,9293,9293,0,0,
	};
const uint16_t deco3[]= 
{
0,0,0,0,0,0,0,9293,0,0,0,0,0,0,0,0,0,0,9293,9293,0,0,0,0,0,0,0,0,0,9293,9293,0,0,0,0,0,0,0,0,0,0,9293,9293,0,0,0,0,0,0,0,0,9293,9293,9293,16142,9293,0,9293,0,0,0,0,0,9293,9293,16142,16142,9293,9293,0,9293,0,0,0,0,9293,9293,16142,9293,9293,9293,0,0,0,0,0,9293,0,9293,9293,16142,9293,0,0,0,0,0,9293,0,9293,0,16142,16142,0,0,0,0,0,0,0,0,0,0,9293,16142,0,0,0,0,0,0,0,0,0,9293,16142,9293,9293,0,0,0,0,0,0,0,0,9293,9293,16142,9293,0,0,0,0,0,0,0,0,65535,9293,9293,65535,0,0,0,0,0,0,0,0,0,9293,9293,0,0,0,0,0,0,0,0,0,0,9293,9293,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
};
const uint16_t dg1[]=
{
	0,0,16142,16142,16142,16142,16142,16142,16142,16142,0,0,0,0,0,16142,16142,16142,16142,16142,16142,0,0,0,0,0,16142,16142,16142,16142,16142,16142,16142,16142,0,0,0,0,16142,16142,16142,1994,1994,16142,16142,16142,0,0,0,0,16142,16142,16142,1994,16142,1994,16142,16142,0,0,0,0,16142,16142,16142,1994,16142,1994,16142,16142,0,0,0,0,16142,16142,16142,1994,16142,1994,16142,16142,0,0,0,0,16142,16142,16142,1994,1994,16142,16142,16142,0,0,0,0,16142,16142,16142,16142,16142,16142,16142,16142,0,0,0,0,16142,16142,16142,1994,1994,1994,16142,16142,0,0,0,0,16142,16142,16142,1994,16142,16142,16142,16142,0,0,0,0,16142,16142,16142,1994,16142,16142,16142,16142,0,0,0,0,16142,16142,16142,1994,16142,1994,16142,16142,0,0,0,0,16142,16142,16142,1994,1994,1994,16142,16142,0,0,0,0,0,16142,16142,16142,16142,16142,16142,0,0,0,0,0,0,16142,16142,16142,16142,16142,16142,0,0,0,
};
const uint16_t D[]=
{
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,61307,61307,65535,65535,65535,65535,65535,0,0,0,0,0,65535,65535,0,0,0,0,65535,61307,0,0,0,0,65535,65535,0,0,0,0,65535,61307,0,0,0,0,65535,65535,0,0,0,0,65535,65535,0,0,0,0,65535,65535,0,0,0,0,65535,65535,0,0,0,0,65535,65535,0,0,0,0,65535,65535,0,0,0,0,65535,65535,0,0,0,0,65535,65535,0,0,0,0,65535,65535,0,0,0,0,65535,65535,0,0,0,0,65535,65535,0,0,0,0,65535,65535,0,0,0,0,61307,65535,0,0,0,0,65535,65535,0,0,0,0,61307,65535,65535,65535,65535,65535,65535,61307,0,0,0,0,61307,65535,65535,65535,65535,65535,61307,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
};
const uint16_t I[]=
{
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,61307,61307,65535,65535,65535,65535,61307,61307,0,0,0,0,65535,65535,65535,65535,65535,65535,65535,61307,0,0,0,0,0,0,0,65535,65535,0,0,0,0,0,0,0,0,0,0,65535,65535,0,0,0,0,0,0,0,0,0,0,65535,65535,0,0,0,0,0,0,0,0,0,0,65535,61307,0,0,0,0,0,0,0,0,0,0,65535,61307,0,0,0,0,0,0,0,0,0,0,65535,65535,0,0,0,0,0,0,0,0,0,0,65535,65535,0,0,0,0,0,0,0,0,0,0,65535,65535,0,0,0,0,0,0,0,61307,65535,65535,65535,65535,65535,65535,65535,0,0,0,0,61307,61307,65535,65535,65535,65535,65535,61307,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
};
const uint16_t N[]=
{
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,65535,65535,61307,0,0,0,61307,61307,0,0,0,0,65535,65535,65535,61307,0,0,65535,61307,0,0,0,0,65535,65535,65535,61307,0,0,65535,65535,0,0,0,0,65535,65535,65535,65535,61307,0,65535,65535,0,0,0,0,61307,65535,65535,65535,65535,0,65535,65535,0,0,0,0,65535,65535,65535,65535,65535,65535,65535,65535,0,0,0,0,65535,65535,65535,65535,65535,65535,65535,65535,0,0,0,0,65535,65535,0,61307,65535,65535,65535,65535,0,0,0,0,65535,65535,0,0,61307,65535,65535,65535,0,0,0,0,65535,65535,0,0,61307,65535,65535,65535,0,0,0,0,61307,65535,0,0,0,65535,65535,61307,0,0,0,0,61307,65535,0,0,0,65535,65535,61307,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
};
const uint16_t O[]=
{
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,61307,65535,65535,65535,61307,61307,0,0,0,0,0,61307,61307,0,0,0,0,65535,65535,0,0,0,0,65535,65535,0,0,0,0,65535,65535,0,0,0,0,65535,65535,0,0,0,0,65535,65535,0,0,0,0,65535,65535,0,0,0,0,65535,65535,0,0,0,0,65535,65535,0,0,0,0,61307,65535,0,0,0,0,65535,61307,0,0,0,0,65535,65535,0,0,0,0,65535,65535,0,0,0,0,65535,61307,0,0,0,0,61307,65535,0,0,0,0,65535,65535,0,0,0,0,65535,65535,0,0,0,0,65535,65535,0,0,0,0,65535,65535,0,0,0,0,65535,65535,0,0,0,0,65535,65535,0,0,0,0,61307,61307,0,0,0,0,0,65535,65535,65535,65535,65535,61307,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
};
const uint16_t pt[]=
{
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,16142,0,0,0,40224,0,0,0,0,0,0,0,16142,24327,0,0,40224,40224,0,0,16142,0,0,0,16142,24327,24327,40224,65315,0,0,16142,0,16142,0,0,0,16142,16142,40224,65315,16142,16142,16142,16142,0,0,0,0,0,40224,65315,65535,65315,16142,16142,0,0,0,0,0,40224,65315,65315,65315,16142,24327,0,0,0,0,0,0,65315,65315,0,16142,24327,40743,0,0,0,0,0,0,0,0,0,16142,24327,40743,0,0,0,0,0,0,0,0,0,0,16142,24327,24327,0,0,0,0,0,0,0,0,0,0,16142,16142,16142,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
};
const uint16_t egg[]=
{
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,63421,63421,0,0,0,0,0,0,0,0,0,7980,24364,63950,32316,0,0,0,0,0,0,0,48948,7980,40756,65007,65315,16180,0,0,0,0,0,0,40756,57140,65007,65535,16172,65323,0,0,0,0,0,63950,65007,63950,65535,65007,65535,40959,63950,0,0,0,0,65535,24364,32556,65535,64743,65535,65535,65535,0,0,0,0,65535,65084,65315,65332,65535,65535,32556,65332,0,0,0,0,65007,63950,16172,65332,65007,48948,65315,32604,0,0,0,0,0,65535,65007,65007,63950,65315,65332,0,0,0,0,0,0,32768,65535,65535,65535,65348,24576,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
};
const uint16_t soap[]=
{
	0,0,13294,0,0,0,0,0,0,0,0,0,20156,0,0,0,0,13294,0,13294,0,0,0,63973,0,0,24475,20156,0,20156,0,0,0,0,13294,0,0,0,63973,57293,63973,65401,65401,24475,20156,0,0,0,0,0,0,0,57293,57293,57293,63973,65401,65401,24475,57293,57293,40876,0,0,0,65535,40876,57293,57293,57293,24475,65401,65401,24475,57293,57293,65535,65535,40876,0,0,40876,65535,57293,57293,24475,24475,24475,40876,65535,65535,65535,40876,24475,24475,0,0,40876,24475,65535,57293,57293,65535,65535,65535,57293,40876,40876,24475,65401,24475,0,0,40876,65401,24475,65535,65535,57293,65401,65401,40876,40876,24475,65401,65401,24475,0,0,40876,65401,65401,65535,57293,65401,65401,40876,40876,24475,65401,24475,0,0,0,0,0,24475,65401,65535,65401,65401,40876,40876,24475,0,0,0,0,0,0,0,0,0,65401,57293,65401,40876,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
};


int main()
{
	int hinverted = 0;
	int vinverted = 0;
	int toggle = 0;
	int hmoved = 0;
	int vmoved = 0;
	int titlescreen = 1;
	int soaptouch = 0;
	int isdead = 0;

	uint16_t x = 12;
	uint16_t y = 144;
	uint16_t oldx = x;
	uint16_t oldy = y;
	uint16_t eggy = 0;
	uint32_t eggx = 0;
	uint32_t oldeggy = 0;

	int lowb = 12;
	int highb = 110;

	initClock();
	initSysTick();
	setupIO();
	while(1)
	{
		while(titlescreen == 1)
		{
			if (upPressed() == 0)
			{
				titlescreen = 0;
			}
			if (titlescreen == 1)
			{
				putImage(37,32,12,16,D,0,0);
				putImage(49,32,12,16,I,0,0);
				putImage(61,32,12,16,N,0,0);
				putImage(73,32,12,16,O,0,0);
				printText("Press up to start", 5,50,RGBToWord(255,255,255),RGBToWord(0,0,0));
			}
			else
			{
				fillRectangle(37,32,12,16,0);
				fillRectangle(49,32,12,16,0);
				fillRectangle(61,32,12,16,0);
				fillRectangle(73,32,12,16,0);
				fillRectangle(0,50,128,16,0);
				fillRectangle(100,124,14,39,RGBToWord(222,184,135));
				fillRectangle(0,100,14,25,RGBToWord(222,184,135));
				fillRectangle(100,75,14,25,RGBToWord(222,184,135));
				fillRectangle(0,50,14,25,RGBToWord(222,184,135));
				fillRectangle(100,25,14,25,RGBToWord(222,184,135));
				drawLine(0,25,128,25,RGBToWord(255,255,255));
				drawLine(0,50,128,50,RGBToWord(255,255,255));
				drawLine(0,75,128,75,RGBToWord(255,255,255));
				drawLine(0,100,128,100,RGBToWord(255,255,255));
				drawLine(0,125,128,125,RGBToWord(255,255,255));
				putImage(100, 12, 16, 12, soap, 0, 0);
			}
		}
	


		hmoved = vmoved = 0;
		hinverted = vinverted = 0;
		while (soaptouch == 0 && isdead == 0)
		{
			// Egg position
			
			eggy = eggy+2;
			putImage(eggx,eggy,12,16,egg,0,0);
			fillRectangle(eggx,oldeggy,12,16,0);
			oldeggy = y;	
			fillRectangle(100,124,14,39,RGBToWord(222,184,135));
			fillRectangle(0,100,14,25,RGBToWord(222,184,135));
			fillRectangle(100,75,14,25,RGBToWord(222,184,135));
			fillRectangle(0,50,14,25,RGBToWord(222,184,135));
			fillRectangle(100,25,14,25,RGBToWord(222,184,135));
			putImage(100, 12, 16, 12, soap, 0, 0);

			// to die to the egg
			if(isInside(eggx,eggy,12,16,x,y) || isInside(eggx,eggy,12,16,x+12,y) || isInside(eggx,eggy,12,16,x,y+ 16) || isInside(eggx,eggy,12,16,x+12,y+16))
			{
				isdead = 1;
				
			}

			if (eggy == 150)
			{
				eggy = 0;
				eggx = lowb + rand()%(highb-lowb+1);
			}

			if (rightPressed()==0) // right pressed
			{			
				if (x < 110)
				{
					x = x + 1;
					hmoved = 1;
					hinverted=0;
				}						
			}
			if (leftPressed()==0) // left pressed
			{			
				
				if (x > 10)
				{
					x = x - 1;
					hmoved = 1;
					hinverted=1;
				}
			}
			if (downPressed() == 0) // down pressed
			{
				if ( (isInside(100,125,14,40,x,y) || isInside(100,125,14,39,x+12,y) || isInside(100,125,14,39,x,y+16) || isInside(100,125,14,39,x+12,y+16) || isInside(0,100,14,40,x,y) || isInside(0,100,14,39,x+12,y) || isInside(0,100,14,39,x,y+16) || isInside(0,100,14,39,x+12,y+16) || isInside(100,75,14,40,x,y) || isInside(100,75,14,39,x+12,y) || isInside(100,75,14,39,x,y+16) || isInside(100,75,14,39,x+12,y+16) || isInside(0,50,14,40,x,y) || isInside(0,50,14,39,x+12,y) || isInside(0,50,14,39,x,y+16) || isInside(0,50,14,39,x+12,y+16) || isInside(100,25,14,40,x,y) || isInside(100,25,14,39,x+12,y) || isInside(100,25,14,39,x,y+16) || isInside(100,25,14,39,x+12,y+16) ) )
				{
					{
						y = y + 1;			
						vmoved = 1;
						vinverted = 0;
					}
				}
			}
				

					/*
					Coords for the ladders
					5(125,166
					4(100,141
					3(75,116
					2(50,91
					1(25,66
					*/
			if (upPressed() == 0) // up pressed
			{			
				if ((isInside(100,125,14,40,x,y) || isInside(100,125,14,39,x+12,y) || isInside(100,125,14,39,x,y+16) || isInside(100,125,14,39,x+12,y+16) || isInside(0,100,14,40,x,y) || isInside(0,100,14,39,x+12,y) || isInside(0,100,14,39,x,y+16) || isInside(0,100,14,39,x+12,y+16) || isInside(100,75,14,40,x,y) || isInside(100,75,14,39,x+12,y) || isInside(100,75,14,39,x,y+16) || isInside(100,75,14,39,x+12,y+16) || isInside(0,50,14,40,x,y) || isInside(0,50,14,39,x+12,y) || isInside(0,50,14,39,x,y+16) || isInside(0,50,14,39,x+12,y+16) || isInside(100,25,14,40,x,y) || isInside(100,25,14,39,x+12,y) || isInside(100,25,14,39,x,y+16) || isInside(100,25,14,39,x+12,y+16) ))
					{
						y = y - 1;
						vmoved = 1;
						vinverted = 1;
					}
					
				fillRectangle(100,124,14,39,RGBToWord(222,184,135));
				fillRectangle(0,100,14,25,RGBToWord(222,184,135));
				fillRectangle(100,75,14,25,RGBToWord(222,184,135));
				fillRectangle(0,50,14,25,RGBToWord(222,184,135));
				fillRectangle(100,25,14,25,RGBToWord(222,184,135));

					
			}
			if ((vmoved) || (hmoved))
			{
				// only redraw if there has been some movement (reduces flicker)
				fillRectangle(oldx,oldy,12,16,0);
				oldx = x;
				oldy = y;					
				if (hmoved)
				{
					if (toggle)
						putImage(x,y,12,16,deco1,hinverted,0);
					else
						putImage(x,y,12,16,deco2,hinverted,0);
					
					toggle = toggle ^ 1;
				}
				else
				{
					putImage(x,y,12,16,deco3,0,vinverted);
				}
					drawLine(0,25,128,25,RGBToWord(255,255,255));
					drawLine(0,50,128,50,RGBToWord(255,255,255));
					drawLine(0,75,128,75,RGBToWord(255,255,255));
					drawLine(0,100,128,100,RGBToWord(255,255,255));
					drawLine(0,125,128,125,RGBToWord(255,255,255));
			}		
			delay(50);
			
			if(y<=15)
			{
				soaptouch = 1;
			}
		
		}
		if (soaptouch == 1)
		{
			fillRectangle(100,124,14,39,0);
			fillRectangle(0,100,14,25,0);
			fillRectangle(100,75,14,25,0);
			fillRectangle(0,50,14,25,0);
			fillRectangle(100,25,14,25,0);
			drawLine(0,25,128,25,0);
			drawLine(0,50,128,50,0);
			drawLine(0,75,128,75,0);
			drawLine(0,100,128,100,0);
			drawLine(0,125,128,125,0);
			fillRectangle(0,100,100,100,0);
			fillRectangle(x,y,12,16,0);
			fillRectangle(eggx,eggy,12,16,0);
			printText(("Dino clean!"),35,45,RGBToWord(255,255,255),0);
		}
		else if (isdead == 1)
		{
				fillRectangle(100,124,14,39,0);
				fillRectangle(0,100,14,25,0);
				fillRectangle(100,75,14,25,0);
				fillRectangle(0,50,14,25,0);
				fillRectangle(100,25,14,25,0);
				drawLine(0,25,128,25,0);
				drawLine(0,50,128,50,0);
				drawLine(0,75,128,75,0);
				drawLine(0,100,128,100,0);
				drawLine(0,125,128,125,0);
				fillRectangle(100,12,16,12,0);
				fillRectangle(x,y,12,16,0);
				fillRectangle(eggx,eggy,12,16,0);
				printText(("You died"),35,45,RGBToWord(255,255,255),0);
		}
	}
}

void initSysTick(void)
{
	SysTick->LOAD = 48000;
	SysTick->CTRL = 7;
	SysTick->VAL = 10;
	__asm(" cpsie i "); // enable interrupts
}
void SysTick_Handler(void)
{
	milliseconds++;
}
void initClock(void)
{
// This is potentially a dangerous function as it could
// result in a system with an invalid clock signal - result: a stuck system
        // Set the PLL up
        // First ensure PLL is disabled
        RCC->CR &= ~(1u<<24);
        while( (RCC->CR & (1 <<25))); // wait for PLL ready to be cleared
        
// Warning here: if system clock is greater than 24MHz then wait-state(s) need to be
// inserted into Flash memory interface
				
        FLASH->ACR |= (1 << 0);
        FLASH->ACR &=~((1u << 2) | (1u<<1));
        // Turn on FLASH prefetch buffer
        FLASH->ACR |= (1 << 4);
        // set PLL multiplier to 12 (yielding 48MHz)
        RCC->CFGR &= ~((1u<<21) | (1u<<20) | (1u<<19) | (1u<<18));
        RCC->CFGR |= ((1<<21) | (1<<19) ); 

        // Need to limit ADC clock to below 14MHz so will change ADC prescaler to 4
        RCC->CFGR |= (1<<14);

        // and turn the PLL back on again
        RCC->CR |= (1<<24);        
        // set PLL as system clock source 
        RCC->CFGR |= (1<<1);
}
void delay(volatile uint32_t dly)
{
	uint32_t end_time = dly + milliseconds;
	while(milliseconds != end_time)
		__asm(" wfi "); // sleep
}

void enablePullUp(GPIO_TypeDef *Port, uint32_t BitNumber)
{
	Port->PUPDR = Port->PUPDR &~(3u << BitNumber*2); // clear pull-up resistor bits
	Port->PUPDR = Port->PUPDR | (1u << BitNumber*2); // set pull-up bit
}
void pinMode(GPIO_TypeDef *Port, uint32_t BitNumber, uint32_t Mode)
{
	/*
	*/
	uint32_t mode_value = Port->MODER;
	Mode = Mode << (2 * BitNumber);
	mode_value = mode_value & ~(3u << (BitNumber * 2));
	mode_value = mode_value | Mode;
	Port->MODER = mode_value;
}
int isInside(uint16_t x1, uint16_t y1, uint16_t w, uint16_t h, uint16_t px, uint16_t py)
{
	// checks to see if point px,py is within the rectange defined by x,y,w,h
	uint16_t x2,y2;
	x2 = x1+w;
	y2 = y1+h;
	int rvalue = 0;
	if ( (px >= x1) && (px <= x2))
	{
		// ok, x constraint met
		if ( (py >= y1) && (py <= y2))
			rvalue = 1;
	}
	return rvalue;
}

void setupIO()
{
	RCC->AHBENR |= (1 << 18) + (1 << 17); // enable Ports A and B
	display_begin();
	pinMode(GPIOB,4,0);
	pinMode(GPIOB,5,0);
	pinMode(GPIOA,8,0);
	pinMode(GPIOA,11,0);
	enablePullUp(GPIOB,4);
	enablePullUp(GPIOB,5);
	enablePullUp(GPIOA,11);
	enablePullUp(GPIOA,8);
}

int rightPressed(void)
{
	return GPIOB->IDR & (1 << 4);
}

int leftPressed(void)
{
	return GPIOB->IDR & (1 << 5);
}

int downPressed(void)
{
	return GPIOA->IDR & (1 << 11);
}

int upPressed(void)
{
	return GPIOA->IDR & (1 << 8);
}

uint32_t shift_register=1234;
uint32_t prbs()
{
	// This is an unverified 31 bit PRBS generator
	// It should be maximum length but this has not been verified 
	unsigned long new_bit=0;
	static int busy=0; // need to prevent re-entrancy here
	if (!busy)
	{
		busy=1;
		new_bit= ((shift_register & (1<<27))>>27) ^ ((shift_register & (1<<30))>>30);
		new_bit= ~new_bit;
		new_bit = new_bit & 1;
		shift_register=shift_register << 1;
		shift_register=shift_register | (new_bit);
		busy=0;
	}
	return shift_register & 0x7fffffff; // return 31 LSB's 
}

