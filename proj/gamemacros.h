#ifndef _GAMEMACROS_H
#define _GAMEMACROS_H

#include <lcom/lcf.h>

//Macros

//#define BIT(n)				(0x01<<(n))
//#define PB2BASE(x) 			(((x) >> 4) & 0x0F000)	//Calculate segment base from a linear address
//#define PB2OFF(x) 			((x) & 0x0FFFF)			//Calculate offset from a linear address


#define H_RES		1024  //Mode 0x105
#define V_RES		768   //Mode 0x105
#define BPP		8   //Mode 0x105

#define NROWS 14
#define NCOLS 21

#define OFFSET 8
#define RESERVED_Y 80 // 768 - (672 + 2*8) y pixels reserved for the in-game menu

#define XOFFSET OFFSET // (1024 - 1008)/2
#define YOFFSET RESERVED_Y + XOFFSET

#define TILE_SIZE 48 // 1008/21 , 672/14

#define FRAME_RATE 60
#define FRAME_RATE_MAX 60

//#define WALL 20
//#define FLOOR 55
//#define PLAYER 200

#define VMODE 0x105

#define TANKSPEED 2
#define AIMSPEED 2
#define ANIMSPEED 6
#define AIM_ANIMSPEED 5
#define BALL_VEL 5

#define BALL_L 12
#define TANK_L 32
#define P_COOLDOWN_FRAMES 5
#define P_MAX_BALLS 3
#define P_BALL_REB 1
#define P_BALL_STR 2
#define P_IFRAMES 30
#define P_HITS 4

#define LIVES 3

#define MAX_TANKS 293
#define MAX_LEVELS 1000
#define MAX_MENUS 100

#define WAIT_TIME 10
#define CAP 100
#define TEXT_SIZE 32

#define BOX_FRAME 5
#define BOX_X_PIX 25
#define BOX_Y_PIX 8
#define BOX_RESERV_X BOX_X_PIX + BOX_FRAME
#define BOX_RESERV_Y BOX_Y_PIX + BOX_FRAME

#define MENU_BOX_X 417
#define MENU_BOX_Y 11
#define MENU_BOX_COLOR 55
#define MENU_BOX_FCOLOR 20
#define INGAME_MENU_COLOR 56
#define INGAME_MENU_FCOLOR 0

#define CLOCK_X 620
#define CLOCK_Y 25
#define RTC_X 216
#define RTC_Y 700
#define LN_X 475
#define LN_Y 400

#define BACKGROUND_NTILES 16
#define BG_XFACTOR 64
#define BG_YFACTOR 48

#define XTANK_SPEED 2
#define XTANK_COOLDOWN_FRAMES 20
#define XTANK_MAX_BALLS 4
#define XTANK_BALL_REB 0
#define XTANK_BALL_STR 2
#define XTANK_BALL_VEL 5
#define XTANK_HITS 1
#define XTANK_SCORE 100
#define XTANK_IFRAMES 10

#define YTANK_SPEED 2
#define YTANK_COOLDOWN_FRAMES 20
#define YTANK_MAX_BALLS 4
#define YTANK_BALL_REB 0
#define YTANK_BALL_STR 2
#define YTANK_BALL_VEL 5
#define YTANK_HITS 2
#define YTANK_SCORE 100
#define YTANK_IFRAMES 10

#define RSTANK_SPEED 0
#define RSTANK_COOLDOWN_FRAMES 40
#define RSTANK_MAX_BALLS 5
#define RSTANK_BALL_REB 0
#define RSTANK_BALL_STR 2
#define RSTANK_BALL_VEL 5
#define RSTANK_HITS 3
#define RSTANK_SCORE 100
#define RSTANK_IFRAMES 10

#define DSTANK_SPEED 0
#define DSTANK_COOLDOWN_FRAMES 40
#define DSTANK_MAX_BALLS 3
#define DSTANK_BALL_REB 0
#define DSTANK_BALL_STR 2
#define DSTANK_BALL_VEL 5
#define DSTANK_HITS 1
#define DSTANK_SCORE 300
#define DSTANK_IFRAMES 10

#define ISTANK_SPEED 0
#define ISTANK_COOLDOWN_FRAMES 10
#define ISTANK_MAX_BALLS 5
#define ISTANK_BALL_REB 4
#define ISTANK_BALL_STR 2
#define ISTANK_BALL_VEL 5
#define ISTANK_HITS 5
#define ISTANK_SCORE 300
#define ISTANK_IFRAMES 10



#define MENU_COLOR 56

#endif //_GAMEMACROS_H

