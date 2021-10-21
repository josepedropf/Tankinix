#include <lcom/lcf.h>
#include <lcom/liblm.h>
#include <lcom/proj.h>

#include "i8254.h"
#include "i8042.h"
#include "videomacros.h"

#include "timer.h"
#include "keyboard.h"
#include "mouse.h"
#include "video_gr.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdarg.h>
#include <math.h>

#include "tankpix.h"

#include "proj_aux.h"

//Levels
Tile_t level_free[14][21]= {
  { F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F},
  { F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F},
  { F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F},
  { F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F},
  { F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F},
  { F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F},
  { F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F},
  { F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F},
  { F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F},
  { F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F},
  { F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F},
  { F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F},
  { F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F},
  { P,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F},
};

Tile_t level_f[14][21]= {
  { F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F},
  { F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F},
  { F,  DS,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F},
  { F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F},
  { F,  F,  F,  W,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  W,  F,  F,  F},
  { F,  F,  F,  W,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  W,  F,  F,  F},
  { F,  F,  F,  W,  F,  F,  F,  F,  F,  F, RS,  F,  F,  F,  F,  F,  F,  W,  F,  F,  F},
  { F,  F,  F,  W,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  W,  F,  F,  F},
  { F,  F,  F,  W,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  W,  F,  F,  F},
  { F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F},
  { F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F},
  { F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F},
  { W,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F},
  { P,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F},
};

Tile_t level_1[14][21]= {
  { F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F},
  { W,  W,  W,  W,  W,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F},
  { W,  W,  W,  W,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F},
  { W,  W,  W,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  W,  F,  W,  W,  W},
  { W,  W,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F},
  { W,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F},
  { W,  W,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F},
  { W,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F},
  { W,  W,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F},
  { W,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F},
  { W,  W,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F},
  { W,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F},
  { F,  F,  F,  W,  W,  W,  W,  F,  F,  F,  F,  F,  F,  W,  F,  F,  F,  F,  F,  F,  F},
  { P,  F,  F,  W,  W,  W,  W,  W,  F,  F,  F,  F,  W,  W,  W,  F,  F,  F,  F,  F,  F}
};

Tile_t level_2[14][21]= {
  { F,  F,  F,  F,  W,  F,  F,  F,  F,  F,  W,  F,  F,  F,  F,  F,  W,  F,  F,  F,  F},
  { F,  F,  F,  F,  W,  F,  F,  F,  F,  F,  W,  F,  F,  F,  F,  F,  W,  F,  F,  F,  F},
  { F,  F,  F,  F,  W,  F,  F,  F,  F,  F,  W,  F,  F,  F,  F,  F,  W,  F, RS,  F,  F},
  { F,  F,  F,  F,  W,  F,  F, RS,  F,  F,  W,  F,  F,  F,  F, XU,  W,  F,  F,  F,  F},
  { F,  F,  F,  F,  W,  F,  F,  F,  F,  F,  W,  F,  F,  F,  F,  F,  W,  F,  F,  F,  F},
  { F,  F,  F,  F,  W,  F,  F,  F,  F,  F,  W,  F,  F,  F,  F,  F,  W,  F,  F,  F,  F},
  { F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F},
  { F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F},
  { F,  F,  W,  F,  F,  F,  F,  W,  F,  F,  F,  F,  F,  W,  F,  F,  F,  F,  F,  W,  F},
  { F,  F,  W,  F,  F,  F,  F,  W,  F,  F,  F,  F,  F,  W,  F,  F,  F,  F,  F,  W,  F},
  { F,  F,  W,  F,  F,  F,  F,  W,  F,  F, XD,  F,  F,  W,  F,  F,  F,  F,  F,  W,  F},
  { F,  F,  W,  F,  F,  F,  F,  W,  F,  F,  F,  F,  F,  W,  F,  F,  F,  F,  F,  W,  F},
  { F,  F,  W,  F,  F,  F,  F,  W,  F,  F,  F,  F,  F,  W,  F,  F,  F,  F,  F,  W,  F},
  { P,  F,  W,  F,  F,  F,  F,  W,  F,  F,  F,  F,  F,  W,  F,  F,  F,  F,  F,  W,  F}
};

Tile_t level_3[14][21]= {
  { F,  F,  F,  F,  W,  F,  F,  F,  F,  F,  W,  F,  F,  F,  F,  F,  W,  F,  F,  F,  F},
  { F,  F,  F,  F,  W,  F,  F,  F,  F,  F,  W,  F,  F,  F,  F,  F,  W,  F,  F,  F,  F},
  { F,  F,  F,  W,  W,  F,  F, RS,  F,  F,  W,  F,  F, XD,  F,  F,  W,  F,  F,  F,  F},
  { F,  F,  F,  F,  W,  F,  F,  F,  F,  F,  W,  F,  F,  F,  F,  F,  W,  F,  F, DS,  F},
  { F,  F,  F,  W,  W,  F,  F,  F,  F,  F,  W,  F,  F,  F,  F,  F,  W,  F,  F,  F,  F},
  { F,  F,  F,  F,  W,  F,  F,  F,  F,  F,  W,  F,  F,  F,  F,  F,  W,  F,  F,  W,  F},
  { F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F},
  { F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F},
  { F,  F,  W,  F,  F,  F,  F,  W,  F,  F,  F,  F,  F,  W,  W,  F,  F,  F,  F,  W,  F},
  { F,  F,  W,  F,  F,  F,  F,  W,  F,  F,  F,  F,  F,  W,  F,  F,  F,  F,  F,  W,  F},
  { F,  F,  W,  F,  F,  F,  F,  W,  F,  F, RS,  F,  F,  W,  F,  F,  F,  F,  F,  W,  F},
  { F,  F,  W,  F,  F,  F,  F,  W,  F,  F,  F,  F,  F,  W,  F,  F,  F,  F,  F,  W,  F},
  { F,  F,  W,  F,  F,  F,  F,  W,  F,  F,  W,  F,  F,  W, YR,  F,  F,  F,  F,  W,  F},
  { P,  F,  W,  F,  F,  F,  F,  W,  F,  F, YR,  F,  F,  W,  F,  F,  F,  F,  F,  W, W}
};

Tile_t level_4[14][21]= {
  { F,  F,  F,  F,  W,  F,  F,  F,  F,  F,  W,  F,  F,  F,  F,  F,  W,  F,  F,  F,  F},
  { F,  F,  F,  F,  W,  F,  F,  F,  F,  F,  W,  F,  F,  F,  F,  F,  W,  F,  F,  F,  F},
  { F,  F,  F,  F,  W,  F,  F,  F,  F,  F,  W,  F,  F,  F,  F,  F,  W,  F, RS,  F,  F},
  { F, RS,  F,  F,  W,  F,  F, DS,  F,  F,  W,  F,  F,  F,  F, XU,  W,  F,  F,  F,  F},
  { F,  F,  F,  F,  W,  F,  F,  F,  F,  F,  W,  F,  F,  F,  F,  F,  W,  F,  F,  F,  F},
  { F,  F,  F, XD,  W,  F,  F,  F,  F,  F,  W,  F,  F,  F,  F,  F,  W,  F,  F,  F,  F},
  { F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F, DS,  F},
  { F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F,  F},
  { F,  F,  W,  F,  F,  F,  F,  W,  F,  F,  F,  F,  F,  W,  F,  F,  F,  F,  F,  W,  F},
  { F,  F,  W,  F,  F,  F,  F,  W,  F,  F,  F,  F,  F,  W,  F,  F,  F,  F,  F,  W,  F},
  { F,  F,  W,  F,  F,  F,  F,  W,  F,  F, XD,  F,  F,  W,  F,  F,  F,  F,  F,  W,  F},
  { F,  F,  W,  F,  F,  F,  F,  W,  F,  F,  F,  F,  F,  W,  F, DS,  F,  F,  F,  W,  F},
  { F,  F,  W,  F,  F,  F,  F,  W,  F,  F,  F,  F,  F,  W,  F,  F,  F,  F,  F,  W,  F},
  { P,  F,  W,  F,  F,  F,  F,  W,  F,  F,  F,  F,  F,  W,  F,  F,  F,  F,  F,  W,  F}
};

//Backgrounds
uint16_t background_clear[BACKGROUND_NTILES][BACKGROUND_NTILES] ={
{65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65},
{65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65},
{65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65},
{65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65},
{65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65},
{65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65},
{65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65},
{65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65},
{65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65},
{65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65},
{65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65},
{65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65},
{65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65},
{65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65},
{65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65},
{65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65}
};

uint16_t background_grey[BACKGROUND_NTILES][BACKGROUND_NTILES] = {
{56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56},
{56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56},
{56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56},
{56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56},
{56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56},
{56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56},
{56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56},
{56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56},
{56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56},
{56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56},
{56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56},
{56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56},
{56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56},
{56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56},
{56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56},
{56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56}
};



static char * screen_saver;

/**
 * @brief creates the levels and puts them in an array
 * 
 */
void (create_levels) (Level_t * Game_Levels[MAX_LEVELS]){
  Game_Levels[0] = create_newlevel(DEATHMATCH, level_f, 0);
  Game_Levels[1] = create_newlevel(SURVIVAL, level_free, 60);
  Game_Levels[2] = create_newlevel(DEATHMATCH, level_1, 0);
  Game_Levels[3] = create_newlevel(DEATHMATCH, level_2, 0);
  Game_Levels[4] = create_newlevel(DEATHMATCH, level_3, 0);
  Game_Levels[5] = create_newlevel(SURVIVAL, level_4, 120);
}

/**
 * @brief creates menus and puts them in an array
 * 
 */
void (create_menus) (Menu_t * Game_Menus[MAX_MENUS]){
  Game_Menus[0] = create_newmenu(background_clear, 3, *create_newbox(align_mid(strlen("Pause")), 11, INGAME_MENU_COLOR, INGAME_MENU_FCOLOR, false, "Pause"), *create_newbox(align_mid(strlen("Resume")), 300, INGAME_MENU_COLOR, INGAME_MENU_FCOLOR, true, "Resume"), *create_newbox(align_mid(strlen("Quit")), 500, INGAME_MENU_COLOR, INGAME_MENU_FCOLOR, true, "Quit"));

  Game_Menus[1] = create_newmenu(background_grey, 6, *create_newbox(align_mid(strlen("Tankinix")), 11, INGAME_MENU_COLOR, INGAME_MENU_FCOLOR, false, "Tankinix"), *create_newbox(align_mid(strlen("Play")), 200, INGAME_MENU_COLOR, INGAME_MENU_FCOLOR, true, "Play"), *create_newbox(align_mid(strlen("Create")), 300, INGAME_MENU_COLOR, INGAME_MENU_FCOLOR, true, "Create"), *create_newbox(align_mid(strlen("Help")), 400, INGAME_MENU_COLOR, INGAME_MENU_FCOLOR, true, "Help"), *create_newbox(align_mid(strlen("Quit")), 500, INGAME_MENU_COLOR, INGAME_MENU_FCOLOR, true, "Quit"), *create_newbox(align_mid(strlen("Quit")), 500, INGAME_MENU_COLOR, INGAME_MENU_FCOLOR, true, "Quit"));

  Game_Menus[2] = create_newmenu(background_grey, 4, *create_newbox(align_mid(strlen("Levels")), 11, INGAME_MENU_COLOR, INGAME_MENU_FCOLOR, false, "Levels"), *create_newbox(610, 500, INGAME_MENU_COLOR, INGAME_MENU_FCOLOR, true, "+"), *create_newbox(325, 500, INGAME_MENU_COLOR, INGAME_MENU_FCOLOR, true, "-"), *create_newbox(align_mid(strlen("Play")), 500, INGAME_MENU_COLOR, INGAME_MENU_FCOLOR, true, "Play"));

}

uint8_t scancode = 0;
int counter = 0;

int mousecounter = 0;
uint8_t packet[3];

static bool movement[4] = {0, 0, 0, 0}; // {W, A, S, D}
static bool mouseinfo[6] = {0, 0, 0, 0, 0, 0}; // {lb, mb, rb, lb release, mb release, rb release}
static struct packet pck;
static rtc_time rtc;

//Struct Init
/**
 * @brief creates an enemy tank with the specified type, hits, sprite...
 * 
 */
Tank_t *(create_new_EnemyTank)(AnimSprite asp, AnimSprite * basp, uint8_t cooldown_frames, uint8_t max_balls, int ball_rebounds, int ball_strength, double ball_vel, TankType_t type, int hits, int score_value, int i_frames){
  Tank_t *tank = (Tank_t *) malloc (sizeof(Tank_t));
  tank->asp = asp;
  tank->basp = basp;
  tank->slope = 0;
  tank->angle = 0;
  tank->ball_count = 0;
  tank->cooldown_count = cooldown_frames;
  tank->aim = NULL;
  tank->cooldown_frames = cooldown_frames;
  tank->max_balls = max_balls;
  tank->aim_right = false;
  tank->aim_up = false;
  tank->ball_vel = ball_vel;
  tank->ball_rebounds = ball_rebounds;
  tank->ball_strength = ball_strength;
  tank->current_ball_id = 0;
  tank->aim_quadrant = XP;
  tank->balls = malloc(max_balls * sizeof(Ball_t));
  for (int i = 0; max_balls > i; i++){
    tank->balls[i].asp = *tank->basp;
    tank->balls[i].angle = tank->angle;
    tank->balls[i].vel = BALL_VEL;
    tank->balls[i].vx = 0;
    tank->balls[i].vy = 0;
    tank->balls[i].rebounds = ball_rebounds;
    tank->balls[i].strength = ball_strength;
    tank->balls[i].state = DESTROYED;
  }
  tank->state = ALIVE;

  tank->hits = hits;
  tank->score_value = score_value;
  tank->last_fig = 0;
  tank->i_frames = i_frames;

  tank->type = type;

  return tank;
}

/**
 * @brief creates the player tank with the specified type, hits, sprite (...) and a pointer to the Aim
 * 
 */
Tank_t *(create_new_PlayerTank)(AnimSprite asp, Aim_t * aim, uint8_t cooldown_frames, uint8_t max_balls, int ball_rebounds, int ball_strength, double ball_vel, int i_frames, int hits){
  Tank_t *tank = (Tank_t *) malloc (sizeof(Tank_t));
  tank->asp = asp;
  tank->basp = create_animSprite_spec(0, 0, 0, 0, ANIMSPEED, NSPRITES_BALL, ball_maps[0], ball_maps[1], ball_maps[2]);
  tank->slope = 0;
  tank->angle = 0;
  tank->ball_count = 0;
  tank->cooldown_count = cooldown_frames;
  tank->aim = aim;
  tank->cooldown_frames = cooldown_frames;
  tank->max_balls = max_balls;
  tank->aim_right = false;
  tank->aim_up = false;
  tank->ball_vel = ball_vel;
  tank->ball_rebounds = ball_rebounds;
  tank->ball_strength = ball_strength;
  tank->current_ball_id = 0;
  tank->aim_quadrant = XP;
  tank->balls = malloc(max_balls * sizeof(Ball_t));
  for (int i = 0; max_balls > i; i++){
    tank->balls[i].asp = *tank->basp;
    tank->balls[i].angle = tank->angle;
    tank->balls[i].vel = BALL_VEL;
    tank->balls[i].vx = 0;
    tank->balls[i].vy = 0;
    tank->balls[i].rebounds = ball_rebounds;
    tank->balls[i].strength = ball_strength;
    tank->balls[i].state = DESTROYED;
  }
  tank->state = ALIVE;

  tank->hits = hits;
  tank->score_value = 0;
  tank->last_fig = 0;
  tank->i_frames = i_frames;

  tank->type = TPLAYER;

  return tank;
}

/**
 * @brief creates a new level with the specified type, layout and initial timer
 * 
 */
Level_t *(create_newlevel)(LevelType_t type, Tile_t layout[NROWS][NCOLS], int init_time){
  Level_t *level = (Level_t *) malloc (sizeof(Level_t));
  level->type = type;
  layout_init(level->layout, layout);
  bool increasing = true;
  if(level->type == SURVIVAL) increasing = false;
  level->clock = *create_newclock(increasing, init_time);
  level->passed = false;
  return level;
}
/**
 * @brief creates new aim with the specified sprite
 * 
 */
Aim_t *(create_newaim)(AnimSprite asp){
  Aim_t *aim = (Aim_t *) malloc (sizeof(Aim_t));
  aim->asp = asp;
  aim->ingame = true;
  aim->status = NORMAL_AIM;
  return aim;
}
/**
 * @brief creates a new clock with an initial time and a bool true when the timer is increasing and false if it is decreasing
 * 
 */
Clock_t *(create_newclock)(bool increasing, int init_time){
  Clock_t *clock = (Clock_t *) malloc (sizeof(Clock_t));
  clock->timer_tot = init_time;
  clock->increasing = increasing;
  clock->seconds = init_time % 60;
  clock->minutes = init_time / 60;
  
  return clock;
}
/**
 * @brief creates new ball and adds it to a tank's array of balls if the bool real is true
 * 
 */
Ball_t *(create_newball)(Tank_t * tank, bool real){
  Ball_t *ball = (Ball_t *) malloc (sizeof(Ball_t));
  ball->asp = *create_animSprite_spec(0, 0, 0, 0, ANIMSPEED, NSPRITES_BALL, ball_maps[0], ball_maps[1], ball_maps[2]);
  ball->asp.num_fig = tank->basp->num_fig;
  ball->asp.maps = tank->basp->maps;
  ball->state = ALIVE;
  ball->asp.sp->x = 0;
  ball->asp.sp->y = 0;
  ball->rebounds = tank->ball_rebounds;
  ball->strength = tank->ball_strength;
  ball->vel = tank->ball_vel;
  if(tank->aim_quadrant != VP && tank->aim_quadrant != VN){
    ball->angle = tank->angle;
    ball->vx = ball->vel * cos(ball->angle);
    ball->vy = -ball->vel * sin(ball->angle);
  }
  else{
    ball->vx = 0;
    if(tank->aim_quadrant == VP){
      ball->angle = (PI)/2.0;
      ball->vy = -ball->vel;
    }
    else{
      ball->angle = 3*(PI)/2.0;
      ball->vy = ball->vel;
    }
  }

  ball->id = tank->current_ball_id;

  if(real){
    for(int i = 0; tank->max_balls > i; i++){
      if(tank->balls[i].state == DESTROYED){
        tank->balls[i] = *ball;
        break;
      }
    }

  tank->current_ball_id++;
  if(tank->current_ball_id >= 1000) tank->current_ball_id = 0;
  tank->ball_count++;
  }

  return ball;
}
/**
 * @brief creates new menu with the specified background and boxes
 * 
 */
Menu_t *(create_newmenu) (uint16_t background[BACKGROUND_NTILES][BACKGROUND_NTILES], int num_boxes, Box_t box, ...){
  Menu_t *menu = (Menu_t *) malloc (sizeof(Menu_t));  
  background_init(menu->background, background);
  menu->num_boxes = num_boxes;
  menu->boxes = malloc(num_boxes * sizeof(Box_t));

  menu->boxes[0] = box;

  va_list ap;
  va_start(ap, box);
  for(int i = 1; i < num_boxes; i++ ){
    Box_t tmp = va_arg(ap, Box_t);
    menu->boxes[i] = tmp;
  }

  va_end(ap);
  return menu;
}
/**
 * @brief creates a box with the position and color indicated, also containing the text passed as argument
 * 
 */
Box_t *(create_newbox)(uint16_t x, uint16_t y, uint16_t color, uint16_t frame_color, bool clickable, char * text){
  Box_t *box = (Box_t *) malloc (sizeof(Box_t));

  box->text = realloc(NULL, sizeof(char) * strlen(text));
  strcpy(box->text, text);

  box->x = x;
  box->y = y;
  box->color = color;
  box->frame_color = frame_color;
  box->clickable = clickable;

  if(strlen(box->text) == 0){
    box->text_h = 0;
    box->text_w = 0;
  }
  else{
    box->text_h = TEXT_SIZE;
    if(strlen(box->text) == 1) box->text_w = TEXT_SIZE;
    else box->text_w = (strlen(box->text) * TEXT_SIZE) + strlen(box->text) - 1;
  }

  box->width = box->text_w + 2 * (BOX_X_PIX + BOX_FRAME);
  box->height = box->text_h + 2 * (BOX_Y_PIX + BOX_FRAME);

  return box;
}
/**
 * @brief creates a new player with the specified tank, number of lives and name
 * 
 */
Player_t *(create_newplayer)(Tank_t * PlayerTank, int lives, char * name){
  Player_t * player = (Player_t *) malloc (sizeof(Player_t));
  player->PlayerTank = PlayerTank;
  player->lives = lives;
  player->score = 0;
  player->name = realloc(NULL, sizeof(char) * strlen(name));
  strcpy(player->name, name);

  return player;
}

/**
 * @brief creates a tank and then a player containing a pointer to that tank
 * 
 */
Player_t *(create_newplayer_spec)(AnimSprite asp, Aim_t * aim, uint8_t cooldown_frames, uint8_t max_balls, int ball_rebounds, int ball_strength, double ball_vel, int i_frames, int hits, int lives, char * name){
  return create_newplayer(create_new_PlayerTank(asp, aim, cooldown_frames, max_balls, ball_rebounds, ball_strength, ball_vel, i_frames, hits), lives, name);
}

//Enemy Tanks
/**
 * @brief creates an enemy tank that moves in the X axis and it's respective XPM's
 * 
 */
void (create_new_XTank)(Tank_t * tanks[MAX_TANKS], bool up, uint16_t x_init, uint16_t y_init){
  xpm_row_t xtank_E[] = {tank_maps[0][0], XTANK_LINE1, XTANK_LINE2, XTANK_LINE3, XTANK_LINE4, tank_maps[0][5], tank_maps[0][6], tank_maps[0][7], tank_maps[0][8], tank_maps[0][9], tank_maps[0][10], tank_maps[0][11], tank_maps[0][12], tank_maps[0][13], tank_maps[0][14], tank_maps[0][15], tank_maps[0][16], tank_maps[0][17], tank_maps[0][18], tank_maps[0][19], tank_maps[0][20], tank_maps[0][21], tank_maps[0][22], tank_maps[0][23], tank_maps[0][24], tank_maps[0][25], tank_maps[0][26], tank_maps[0][27], tank_maps[0][28], tank_maps[0][29], tank_maps[0][30], tank_maps[0][31], tank_maps[0][32], tank_maps[0][33], tank_maps[0][34], tank_maps[0][35], tank_maps[0][36], tank_maps[0][37]};

  xpm_row_t xtank_ENE[] = {tank_maps[1][0], XTANK_LINE1, XTANK_LINE2, XTANK_LINE3, XTANK_LINE4, tank_maps[1][5], tank_maps[1][6], tank_maps[1][7], tank_maps[1][8], tank_maps[1][9], tank_maps[1][10], tank_maps[1][11], tank_maps[1][12], tank_maps[1][13], tank_maps[1][14], tank_maps[1][15], tank_maps[1][16], tank_maps[1][17], tank_maps[1][18], tank_maps[1][19], tank_maps[1][20], tank_maps[1][21], tank_maps[1][22], tank_maps[1][23], tank_maps[1][24], tank_maps[1][25], tank_maps[1][26], tank_maps[1][27], tank_maps[1][28], tank_maps[1][29], tank_maps[1][30], tank_maps[1][31], tank_maps[1][32], tank_maps[1][33], tank_maps[1][34], tank_maps[1][35], tank_maps[1][36], tank_maps[1][37]};

  xpm_row_t xtank_NE[] = {tank_maps[2][0], XTANK_LINE1, XTANK_LINE2, XTANK_LINE3, XTANK_LINE4, tank_maps[2][5], tank_maps[2][6], tank_maps[2][7], tank_maps[2][8], tank_maps[2][9], tank_maps[2][10], tank_maps[2][11], tank_maps[2][12], tank_maps[2][13], tank_maps[2][14], tank_maps[2][15], tank_maps[2][16], tank_maps[2][17], tank_maps[2][18], tank_maps[2][19], tank_maps[2][20], tank_maps[2][21], tank_maps[2][22], tank_maps[2][23], tank_maps[2][24], tank_maps[2][25], tank_maps[2][26], tank_maps[2][27], tank_maps[2][28], tank_maps[2][29], tank_maps[2][30], tank_maps[2][31], tank_maps[2][32], tank_maps[2][33], tank_maps[2][34], tank_maps[2][35], tank_maps[2][36], tank_maps[2][37]};

  xpm_row_t xtank_NNE[] = {tank_maps[3][0], XTANK_LINE1, XTANK_LINE2, XTANK_LINE3, XTANK_LINE4, tank_maps[3][5], tank_maps[3][6], tank_maps[3][7], tank_maps[3][8], tank_maps[3][9], tank_maps[3][10], tank_maps[3][11], tank_maps[3][12], tank_maps[3][13], tank_maps[3][14], tank_maps[3][15], tank_maps[3][16], tank_maps[3][17], tank_maps[3][18], tank_maps[3][19], tank_maps[3][20], tank_maps[3][21], tank_maps[3][22], tank_maps[3][23], tank_maps[3][24], tank_maps[3][25], tank_maps[3][26], tank_maps[3][27], tank_maps[3][28], tank_maps[3][29], tank_maps[3][30], tank_maps[3][31], tank_maps[3][32], tank_maps[3][33], tank_maps[3][34], tank_maps[3][35], tank_maps[3][36], tank_maps[3][37]};

  xpm_row_t xtank_N[] = {tank_maps[4][0], XTANK_LINE1, XTANK_LINE2, XTANK_LINE3, XTANK_LINE4, tank_maps[4][5], tank_maps[4][6], tank_maps[4][7], tank_maps[4][8], tank_maps[4][9], tank_maps[4][10], tank_maps[4][11], tank_maps[4][12], tank_maps[4][13], tank_maps[4][14], tank_maps[4][15], tank_maps[4][16], tank_maps[4][17], tank_maps[4][18], tank_maps[4][19], tank_maps[4][20], tank_maps[4][21], tank_maps[4][22], tank_maps[4][23], tank_maps[4][24], tank_maps[4][25], tank_maps[4][26], tank_maps[4][27], tank_maps[4][28], tank_maps[4][29], tank_maps[4][30], tank_maps[4][31], tank_maps[4][32], tank_maps[4][33], tank_maps[4][34], tank_maps[4][35], tank_maps[4][36], tank_maps[4][37]};

  xpm_row_t xtank_NNO[] = {tank_maps[5][0], XTANK_LINE1, XTANK_LINE2, XTANK_LINE3, XTANK_LINE4, tank_maps[5][5], tank_maps[5][6], tank_maps[5][7], tank_maps[5][8], tank_maps[5][9], tank_maps[5][10], tank_maps[5][11], tank_maps[5][12], tank_maps[5][13], tank_maps[5][14], tank_maps[5][15], tank_maps[5][16], tank_maps[5][17], tank_maps[5][18], tank_maps[5][19], tank_maps[5][20], tank_maps[5][21], tank_maps[5][22], tank_maps[5][23], tank_maps[5][24], tank_maps[5][25], tank_maps[5][26], tank_maps[5][27], tank_maps[5][28], tank_maps[5][29], tank_maps[5][30], tank_maps[5][31], tank_maps[5][32], tank_maps[5][33], tank_maps[5][34], tank_maps[5][35], tank_maps[5][36], tank_maps[5][37]};

  xpm_row_t xtank_NO[] = {tank_maps[6][0], XTANK_LINE1, XTANK_LINE2, XTANK_LINE3, XTANK_LINE4, tank_maps[6][5], tank_maps[6][6], tank_maps[6][7], tank_maps[6][8], tank_maps[6][9], tank_maps[6][10], tank_maps[6][11], tank_maps[6][12], tank_maps[6][13], tank_maps[6][14], tank_maps[6][15], tank_maps[6][16], tank_maps[6][17], tank_maps[6][18], tank_maps[6][19], tank_maps[6][20], tank_maps[6][21], tank_maps[6][22], tank_maps[6][23], tank_maps[6][24], tank_maps[6][25], tank_maps[6][26], tank_maps[6][27], tank_maps[6][28], tank_maps[6][29], tank_maps[6][30], tank_maps[6][31], tank_maps[6][32], tank_maps[6][33], tank_maps[6][34], tank_maps[6][35], tank_maps[6][36], tank_maps[6][37]};

  xpm_row_t xtank_ONO[] = {tank_maps[7][0], XTANK_LINE1, XTANK_LINE2, XTANK_LINE3, XTANK_LINE4, tank_maps[7][5], tank_maps[7][6], tank_maps[7][7], tank_maps[7][8], tank_maps[7][9], tank_maps[7][10], tank_maps[7][11], tank_maps[7][12], tank_maps[7][13], tank_maps[7][14], tank_maps[7][15], tank_maps[7][16], tank_maps[7][17], tank_maps[7][18], tank_maps[7][19], tank_maps[7][20], tank_maps[7][21], tank_maps[7][22], tank_maps[7][23], tank_maps[7][24], tank_maps[7][25], tank_maps[7][26], tank_maps[7][27], tank_maps[7][28], tank_maps[7][29], tank_maps[7][30], tank_maps[7][31], tank_maps[7][32], tank_maps[7][33], tank_maps[7][34], tank_maps[7][35], tank_maps[7][36], tank_maps[7][37]};

  xpm_row_t xtank_O[] = {tank_maps[8][0], XTANK_LINE1, XTANK_LINE2, XTANK_LINE3, XTANK_LINE4, tank_maps[8][5], tank_maps[8][6], tank_maps[8][7], tank_maps[8][8], tank_maps[8][9], tank_maps[8][10], tank_maps[8][11], tank_maps[8][12], tank_maps[8][13], tank_maps[8][14], tank_maps[8][15], tank_maps[8][16], tank_maps[8][17], tank_maps[8][18], tank_maps[8][19], tank_maps[8][20], tank_maps[8][21], tank_maps[8][22], tank_maps[8][23], tank_maps[8][24], tank_maps[8][25], tank_maps[8][26], tank_maps[8][27], tank_maps[8][28], tank_maps[8][29], tank_maps[8][30], tank_maps[8][31], tank_maps[8][32], tank_maps[8][33], tank_maps[8][34], tank_maps[8][35], tank_maps[8][36], tank_maps[8][37]};

  xpm_row_t xtank_OSO[] = {tank_maps[9][0], XTANK_LINE1, XTANK_LINE2, XTANK_LINE3, XTANK_LINE4, tank_maps[9][5], tank_maps[9][6], tank_maps[9][7], tank_maps[9][8], tank_maps[9][9], tank_maps[9][10], tank_maps[9][11], tank_maps[9][12], tank_maps[9][13], tank_maps[9][14], tank_maps[9][15], tank_maps[9][16], tank_maps[9][17], tank_maps[9][18], tank_maps[9][19], tank_maps[9][20], tank_maps[9][21], tank_maps[9][22], tank_maps[9][23], tank_maps[9][24], tank_maps[9][25], tank_maps[9][26], tank_maps[9][27], tank_maps[9][28], tank_maps[9][29], tank_maps[9][30], tank_maps[9][31], tank_maps[9][32], tank_maps[9][33], tank_maps[9][34], tank_maps[9][35], tank_maps[9][36], tank_maps[9][37]};

  xpm_row_t xtank_SO[] = {tank_maps[10][0], XTANK_LINE1, XTANK_LINE2, XTANK_LINE3, XTANK_LINE4, tank_maps[10][5], tank_maps[10][6], tank_maps[10][7], tank_maps[10][8], tank_maps[10][9], tank_maps[10][10], tank_maps[10][11], tank_maps[10][12], tank_maps[10][13], tank_maps[10][14], tank_maps[10][15], tank_maps[10][16], tank_maps[10][17], tank_maps[10][18], tank_maps[10][19], tank_maps[10][20], tank_maps[10][21], tank_maps[10][22], tank_maps[10][23], tank_maps[10][24], tank_maps[10][25], tank_maps[10][26], tank_maps[10][27], tank_maps[10][28], tank_maps[10][29], tank_maps[10][30], tank_maps[10][31], tank_maps[10][32], tank_maps[10][33], tank_maps[10][34], tank_maps[10][35], tank_maps[10][36], tank_maps[10][37]};

  xpm_row_t xtank_SSO[] = {tank_maps[11][0], XTANK_LINE1, XTANK_LINE2, XTANK_LINE3, XTANK_LINE4, tank_maps[11][5], tank_maps[11][6], tank_maps[11][7], tank_maps[11][8], tank_maps[11][9], tank_maps[11][10], tank_maps[11][11], tank_maps[11][12], tank_maps[11][13], tank_maps[11][14], tank_maps[11][15], tank_maps[11][16], tank_maps[11][17], tank_maps[11][18], tank_maps[11][19], tank_maps[11][20], tank_maps[11][21], tank_maps[11][22], tank_maps[11][23], tank_maps[11][24], tank_maps[11][25], tank_maps[11][26], tank_maps[11][27], tank_maps[11][28], tank_maps[11][29], tank_maps[11][30], tank_maps[11][31], tank_maps[11][32], tank_maps[11][33], tank_maps[11][34], tank_maps[11][35], tank_maps[11][36], tank_maps[11][37]};

  xpm_row_t xtank_S[] = {tank_maps[12][0], XTANK_LINE1, XTANK_LINE2, XTANK_LINE3, XTANK_LINE4, tank_maps[12][5], tank_maps[12][6], tank_maps[12][7], tank_maps[12][8], tank_maps[12][9], tank_maps[12][10], tank_maps[12][11], tank_maps[12][12], tank_maps[12][13], tank_maps[12][14], tank_maps[12][15], tank_maps[12][16], tank_maps[12][17], tank_maps[12][18], tank_maps[12][19], tank_maps[12][20], tank_maps[12][21], tank_maps[12][22], tank_maps[12][23], tank_maps[12][24], tank_maps[12][25], tank_maps[12][26], tank_maps[12][27], tank_maps[12][28], tank_maps[12][29], tank_maps[12][30], tank_maps[12][31], tank_maps[12][32], tank_maps[12][33], tank_maps[12][34], tank_maps[12][35], tank_maps[12][36], tank_maps[12][37]};

  xpm_row_t xtank_SSE[] = {tank_maps[13][0], XTANK_LINE1, XTANK_LINE2, XTANK_LINE3, XTANK_LINE4, tank_maps[13][5], tank_maps[13][6], tank_maps[13][7], tank_maps[13][8], tank_maps[13][9], tank_maps[13][10], tank_maps[13][11], tank_maps[13][12], tank_maps[13][13], tank_maps[13][14], tank_maps[13][15], tank_maps[13][16], tank_maps[13][17], tank_maps[13][18], tank_maps[13][19], tank_maps[13][20], tank_maps[13][21], tank_maps[13][22], tank_maps[13][23], tank_maps[13][24], tank_maps[13][25], tank_maps[13][26], tank_maps[13][27], tank_maps[13][28], tank_maps[13][29], tank_maps[13][30], tank_maps[13][31], tank_maps[13][32], tank_maps[13][33], tank_maps[13][34], tank_maps[13][35], tank_maps[13][36], tank_maps[13][37]};

  xpm_row_t xtank_SE[] = {tank_maps[14][0], XTANK_LINE1, XTANK_LINE2, XTANK_LINE3, XTANK_LINE4, tank_maps[14][5], tank_maps[14][6], tank_maps[14][7], tank_maps[14][8], tank_maps[14][9], tank_maps[14][10], tank_maps[14][11], tank_maps[14][12], tank_maps[14][13], tank_maps[14][14], tank_maps[14][15], tank_maps[14][16], tank_maps[14][17], tank_maps[14][18], tank_maps[14][19], tank_maps[14][20], tank_maps[14][21], tank_maps[14][22], tank_maps[14][23], tank_maps[14][24], tank_maps[14][25], tank_maps[14][26], tank_maps[14][27], tank_maps[14][28], tank_maps[14][29], tank_maps[14][30], tank_maps[14][31], tank_maps[14][32], tank_maps[14][33], tank_maps[14][34], tank_maps[14][35], tank_maps[14][36], tank_maps[14][37]};

  xpm_row_t xtank_ESE[] = {tank_maps[15][0], XTANK_LINE1, XTANK_LINE2, XTANK_LINE3, XTANK_LINE4, tank_maps[15][5], tank_maps[15][6], tank_maps[15][7], tank_maps[15][8], tank_maps[15][9], tank_maps[15][10], tank_maps[15][11], tank_maps[15][12], tank_maps[15][13], tank_maps[15][14], tank_maps[15][15], tank_maps[15][16], tank_maps[15][17], tank_maps[15][18], tank_maps[15][19], tank_maps[15][20], tank_maps[15][21], tank_maps[15][22], tank_maps[15][23], tank_maps[15][24], tank_maps[15][25], tank_maps[15][26], tank_maps[15][27], tank_maps[15][28], tank_maps[15][29], tank_maps[15][30], tank_maps[15][31], tank_maps[15][32], tank_maps[15][33], tank_maps[15][34], tank_maps[15][35], tank_maps[15][36], tank_maps[15][37]};

  xpm_row_t *xtank_maps[NSPRITES_TANK] = {xtank_E, xtank_ENE, xtank_NE, xtank_NNE, xtank_N, xtank_NNO, xtank_NO, xtank_ONO, xtank_O, xtank_OSO, xtank_SO, xtank_SSO, xtank_S, xtank_SSE, xtank_SE, xtank_ESE, tank_exp1, tank_exp2, tank_base};

  Tank_t * xtank = create_new_EnemyTank(*create_animSprite_spec(NCOLS * TILE_SIZE / 2, NROWS * TILE_SIZE / 2, XTANK_SPEED, 0, ANIMSPEED, NSPRITES_TANK, xtank_maps[0], xtank_maps[1], xtank_maps[2], xtank_maps[3], xtank_maps[4], xtank_maps[5], xtank_maps[6], xtank_maps[7], xtank_maps[8], xtank_maps[9], xtank_maps[10], xtank_maps[11], xtank_maps[12], xtank_maps[13], xtank_maps[14], xtank_maps[15], xtank_maps[16], xtank_maps[17], xtank_maps[18]), create_animSprite_spec(0, 0, 0, 0, ANIMSPEED, NSPRITES_BLACK_BALL, black_ball_maps[0], black_ball_maps[1], black_ball_maps[2]), XTANK_COOLDOWN_FRAMES, XTANK_MAX_BALLS, XTANK_BALL_REB, XTANK_BALL_STR, XTANK_BALL_VEL, XTANK, XTANK_HITS, XTANK_SCORE, XTANK_IFRAMES);

  if(up) xtank->asp.cur_fig = 4; 
  else xtank->asp.cur_fig = 12;

  xtank->asp.sp->x = x_init;
  xtank->asp.sp->y = y_init;

  add_tank(tanks, xtank);

  draw_animsprite(&xtank->asp);
}
/**
 * @brief creates an enemy tank that moves in the Y axis and it's respective XPM's
 * 
 */
void (create_new_YTank)(Tank_t * tanks[MAX_TANKS], bool right, uint16_t x_init, uint16_t y_init){
  xpm_row_t ytank_E[] = {tank_maps[0][0], YTANK_LINE1, YTANK_LINE2, YTANK_LINE3, YTANK_LINE4, tank_maps[0][5], tank_maps[0][6], tank_maps[0][7], tank_maps[0][8], tank_maps[0][9], tank_maps[0][10], tank_maps[0][11], tank_maps[0][12], tank_maps[0][13], tank_maps[0][14], tank_maps[0][15], tank_maps[0][16], tank_maps[0][17], tank_maps[0][18], tank_maps[0][19], tank_maps[0][20], tank_maps[0][21], tank_maps[0][22], tank_maps[0][23], tank_maps[0][24], tank_maps[0][25], tank_maps[0][26], tank_maps[0][27], tank_maps[0][28], tank_maps[0][29], tank_maps[0][30], tank_maps[0][31], tank_maps[0][32], tank_maps[0][33], tank_maps[0][34], tank_maps[0][35], tank_maps[0][36], tank_maps[0][37]};

  xpm_row_t ytank_ENE[] = {tank_maps[1][0], YTANK_LINE1, YTANK_LINE2, YTANK_LINE3, YTANK_LINE4, tank_maps[1][5], tank_maps[1][6], tank_maps[1][7], tank_maps[1][8], tank_maps[1][9], tank_maps[1][10], tank_maps[1][11], tank_maps[1][12], tank_maps[1][13], tank_maps[1][14], tank_maps[1][15], tank_maps[1][16], tank_maps[1][17], tank_maps[1][18], tank_maps[1][19], tank_maps[1][20], tank_maps[1][21], tank_maps[1][22], tank_maps[1][23], tank_maps[1][24], tank_maps[1][25], tank_maps[1][26], tank_maps[1][27], tank_maps[1][28], tank_maps[1][29], tank_maps[1][30], tank_maps[1][31], tank_maps[1][32], tank_maps[1][33], tank_maps[1][34], tank_maps[1][35], tank_maps[1][36], tank_maps[1][37]};

  xpm_row_t ytank_NE[] = {tank_maps[2][0], YTANK_LINE1, YTANK_LINE2, YTANK_LINE3, YTANK_LINE4, tank_maps[2][5], tank_maps[2][6], tank_maps[2][7], tank_maps[2][8], tank_maps[2][9], tank_maps[2][10], tank_maps[2][11], tank_maps[2][12], tank_maps[2][13], tank_maps[2][14], tank_maps[2][15], tank_maps[2][16], tank_maps[2][17], tank_maps[2][18], tank_maps[2][19], tank_maps[2][20], tank_maps[2][21], tank_maps[2][22], tank_maps[2][23], tank_maps[2][24], tank_maps[2][25], tank_maps[2][26], tank_maps[2][27], tank_maps[2][28], tank_maps[2][29], tank_maps[2][30], tank_maps[2][31], tank_maps[2][32], tank_maps[2][33], tank_maps[2][34], tank_maps[2][35], tank_maps[2][36], tank_maps[2][37]};

  xpm_row_t ytank_NNE[] = {tank_maps[3][0], YTANK_LINE1, YTANK_LINE2, YTANK_LINE3, YTANK_LINE4, tank_maps[3][5], tank_maps[3][6], tank_maps[3][7], tank_maps[3][8], tank_maps[3][9], tank_maps[3][10], tank_maps[3][11], tank_maps[3][12], tank_maps[3][13], tank_maps[3][14], tank_maps[3][15], tank_maps[3][16], tank_maps[3][17], tank_maps[3][18], tank_maps[3][19], tank_maps[3][20], tank_maps[3][21], tank_maps[3][22], tank_maps[3][23], tank_maps[3][24], tank_maps[3][25], tank_maps[3][26], tank_maps[3][27], tank_maps[3][28], tank_maps[3][29], tank_maps[3][30], tank_maps[3][31], tank_maps[3][32], tank_maps[3][33], tank_maps[3][34], tank_maps[3][35], tank_maps[3][36], tank_maps[3][37]};

  xpm_row_t ytank_N[] = {tank_maps[4][0], YTANK_LINE1, YTANK_LINE2, YTANK_LINE3, YTANK_LINE4, tank_maps[4][5], tank_maps[4][6], tank_maps[4][7], tank_maps[4][8], tank_maps[4][9], tank_maps[4][10], tank_maps[4][11], tank_maps[4][12], tank_maps[4][13], tank_maps[4][14], tank_maps[4][15], tank_maps[4][16], tank_maps[4][17], tank_maps[4][18], tank_maps[4][19], tank_maps[4][20], tank_maps[4][21], tank_maps[4][22], tank_maps[4][23], tank_maps[4][24], tank_maps[4][25], tank_maps[4][26], tank_maps[4][27], tank_maps[4][28], tank_maps[4][29], tank_maps[4][30], tank_maps[4][31], tank_maps[4][32], tank_maps[4][33], tank_maps[4][34], tank_maps[4][35], tank_maps[4][36], tank_maps[4][37]};

  xpm_row_t ytank_NNO[] = {tank_maps[5][0], YTANK_LINE1, YTANK_LINE2, YTANK_LINE3, YTANK_LINE4, tank_maps[5][5], tank_maps[5][6], tank_maps[5][7], tank_maps[5][8], tank_maps[5][9], tank_maps[5][10], tank_maps[5][11], tank_maps[5][12], tank_maps[5][13], tank_maps[5][14], tank_maps[5][15], tank_maps[5][16], tank_maps[5][17], tank_maps[5][18], tank_maps[5][19], tank_maps[5][20], tank_maps[5][21], tank_maps[5][22], tank_maps[5][23], tank_maps[5][24], tank_maps[5][25], tank_maps[5][26], tank_maps[5][27], tank_maps[5][28], tank_maps[5][29], tank_maps[5][30], tank_maps[5][31], tank_maps[5][32], tank_maps[5][33], tank_maps[5][34], tank_maps[5][35], tank_maps[5][36], tank_maps[5][37]};

  xpm_row_t ytank_NO[] = {tank_maps[6][0], YTANK_LINE1, YTANK_LINE2, YTANK_LINE3, YTANK_LINE4, tank_maps[6][5], tank_maps[6][6], tank_maps[6][7], tank_maps[6][8], tank_maps[6][9], tank_maps[6][10], tank_maps[6][11], tank_maps[6][12], tank_maps[6][13], tank_maps[6][14], tank_maps[6][15], tank_maps[6][16], tank_maps[6][17], tank_maps[6][18], tank_maps[6][19], tank_maps[6][20], tank_maps[6][21], tank_maps[6][22], tank_maps[6][23], tank_maps[6][24], tank_maps[6][25], tank_maps[6][26], tank_maps[6][27], tank_maps[6][28], tank_maps[6][29], tank_maps[6][30], tank_maps[6][31], tank_maps[6][32], tank_maps[6][33], tank_maps[6][34], tank_maps[6][35], tank_maps[6][36], tank_maps[6][37]};

  xpm_row_t ytank_ONO[] = {tank_maps[7][0], YTANK_LINE1, YTANK_LINE2, YTANK_LINE3, YTANK_LINE4, tank_maps[7][5], tank_maps[7][6], tank_maps[7][7], tank_maps[7][8], tank_maps[7][9], tank_maps[7][10], tank_maps[7][11], tank_maps[7][12], tank_maps[7][13], tank_maps[7][14], tank_maps[7][15], tank_maps[7][16], tank_maps[7][17], tank_maps[7][18], tank_maps[7][19], tank_maps[7][20], tank_maps[7][21], tank_maps[7][22], tank_maps[7][23], tank_maps[7][24], tank_maps[7][25], tank_maps[7][26], tank_maps[7][27], tank_maps[7][28], tank_maps[7][29], tank_maps[7][30], tank_maps[7][31], tank_maps[7][32], tank_maps[7][33], tank_maps[7][34], tank_maps[7][35], tank_maps[7][36], tank_maps[7][37]};

  xpm_row_t ytank_O[] = {tank_maps[8][0], YTANK_LINE1, YTANK_LINE2, YTANK_LINE3, YTANK_LINE4, tank_maps[8][5], tank_maps[8][6], tank_maps[8][7], tank_maps[8][8], tank_maps[8][9], tank_maps[8][10], tank_maps[8][11], tank_maps[8][12], tank_maps[8][13], tank_maps[8][14], tank_maps[8][15], tank_maps[8][16], tank_maps[8][17], tank_maps[8][18], tank_maps[8][19], tank_maps[8][20], tank_maps[8][21], tank_maps[8][22], tank_maps[8][23], tank_maps[8][24], tank_maps[8][25], tank_maps[8][26], tank_maps[8][27], tank_maps[8][28], tank_maps[8][29], tank_maps[8][30], tank_maps[8][31], tank_maps[8][32], tank_maps[8][33], tank_maps[8][34], tank_maps[8][35], tank_maps[8][36], tank_maps[8][37]};

  xpm_row_t ytank_OSO[] = {tank_maps[9][0], YTANK_LINE1, YTANK_LINE2, YTANK_LINE3, YTANK_LINE4, tank_maps[9][5], tank_maps[9][6], tank_maps[9][7], tank_maps[9][8], tank_maps[9][9], tank_maps[9][10], tank_maps[9][11], tank_maps[9][12], tank_maps[9][13], tank_maps[9][14], tank_maps[9][15], tank_maps[9][16], tank_maps[9][17], tank_maps[9][18], tank_maps[9][19], tank_maps[9][20], tank_maps[9][21], tank_maps[9][22], tank_maps[9][23], tank_maps[9][24], tank_maps[9][25], tank_maps[9][26], tank_maps[9][27], tank_maps[9][28], tank_maps[9][29], tank_maps[9][30], tank_maps[9][31], tank_maps[9][32], tank_maps[9][33], tank_maps[9][34], tank_maps[9][35], tank_maps[9][36], tank_maps[9][37]};

  xpm_row_t ytank_SO[] = {tank_maps[10][0], YTANK_LINE1, YTANK_LINE2, YTANK_LINE3, YTANK_LINE4, tank_maps[10][5], tank_maps[10][6], tank_maps[10][7], tank_maps[10][8], tank_maps[10][9], tank_maps[10][10], tank_maps[10][11], tank_maps[10][12], tank_maps[10][13], tank_maps[10][14], tank_maps[10][15], tank_maps[10][16], tank_maps[10][17], tank_maps[10][18], tank_maps[10][19], tank_maps[10][20], tank_maps[10][21], tank_maps[10][22], tank_maps[10][23], tank_maps[10][24], tank_maps[10][25], tank_maps[10][26], tank_maps[10][27], tank_maps[10][28], tank_maps[10][29], tank_maps[10][30], tank_maps[10][31], tank_maps[10][32], tank_maps[10][33], tank_maps[10][34], tank_maps[10][35], tank_maps[10][36], tank_maps[10][37]};

  xpm_row_t ytank_SSO[] = {tank_maps[11][0], YTANK_LINE1, YTANK_LINE2, YTANK_LINE3, YTANK_LINE4, tank_maps[11][5], tank_maps[11][6], tank_maps[11][7], tank_maps[11][8], tank_maps[11][9], tank_maps[11][10], tank_maps[11][11], tank_maps[11][12], tank_maps[11][13], tank_maps[11][14], tank_maps[11][15], tank_maps[11][16], tank_maps[11][17], tank_maps[11][18], tank_maps[11][19], tank_maps[11][20], tank_maps[11][21], tank_maps[11][22], tank_maps[11][23], tank_maps[11][24], tank_maps[11][25], tank_maps[11][26], tank_maps[11][27], tank_maps[11][28], tank_maps[11][29], tank_maps[11][30], tank_maps[11][31], tank_maps[11][32], tank_maps[11][33], tank_maps[11][34], tank_maps[11][35], tank_maps[11][36], tank_maps[11][37]};

  xpm_row_t ytank_S[] = {tank_maps[12][0], YTANK_LINE1, YTANK_LINE2, YTANK_LINE3, YTANK_LINE4, tank_maps[12][5], tank_maps[12][6], tank_maps[12][7], tank_maps[12][8], tank_maps[12][9], tank_maps[12][10], tank_maps[12][11], tank_maps[12][12], tank_maps[12][13], tank_maps[12][14], tank_maps[12][15], tank_maps[12][16], tank_maps[12][17], tank_maps[12][18], tank_maps[12][19], tank_maps[12][20], tank_maps[12][21], tank_maps[12][22], tank_maps[12][23], tank_maps[12][24], tank_maps[12][25], tank_maps[12][26], tank_maps[12][27], tank_maps[12][28], tank_maps[12][29], tank_maps[12][30], tank_maps[12][31], tank_maps[12][32], tank_maps[12][33], tank_maps[12][34], tank_maps[12][35], tank_maps[12][36], tank_maps[12][37]};

  xpm_row_t ytank_SSE[] = {tank_maps[13][0], YTANK_LINE1, YTANK_LINE2, YTANK_LINE3, YTANK_LINE4, tank_maps[13][5], tank_maps[13][6], tank_maps[13][7], tank_maps[13][8], tank_maps[13][9], tank_maps[13][10], tank_maps[13][11], tank_maps[13][12], tank_maps[13][13], tank_maps[13][14], tank_maps[13][15], tank_maps[13][16], tank_maps[13][17], tank_maps[13][18], tank_maps[13][19], tank_maps[13][20], tank_maps[13][21], tank_maps[13][22], tank_maps[13][23], tank_maps[13][24], tank_maps[13][25], tank_maps[13][26], tank_maps[13][27], tank_maps[13][28], tank_maps[13][29], tank_maps[13][30], tank_maps[13][31], tank_maps[13][32], tank_maps[13][33], tank_maps[13][34], tank_maps[13][35], tank_maps[13][36], tank_maps[13][37]};

  xpm_row_t ytank_SE[] = {tank_maps[14][0], YTANK_LINE1, YTANK_LINE2, YTANK_LINE3, YTANK_LINE4, tank_maps[14][5], tank_maps[14][6], tank_maps[14][7], tank_maps[14][8], tank_maps[14][9], tank_maps[14][10], tank_maps[14][11], tank_maps[14][12], tank_maps[14][13], tank_maps[14][14], tank_maps[14][15], tank_maps[14][16], tank_maps[14][17], tank_maps[14][18], tank_maps[14][19], tank_maps[14][20], tank_maps[14][21], tank_maps[14][22], tank_maps[14][23], tank_maps[14][24], tank_maps[14][25], tank_maps[14][26], tank_maps[14][27], tank_maps[14][28], tank_maps[14][29], tank_maps[14][30], tank_maps[14][31], tank_maps[14][32], tank_maps[14][33], tank_maps[14][34], tank_maps[14][35], tank_maps[14][36], tank_maps[14][37]};

  xpm_row_t ytank_ESE[] = {tank_maps[15][0], YTANK_LINE1, YTANK_LINE2, YTANK_LINE3, YTANK_LINE4, tank_maps[15][5], tank_maps[15][6], tank_maps[15][7], tank_maps[15][8], tank_maps[15][9], tank_maps[15][10], tank_maps[15][11], tank_maps[15][12], tank_maps[15][13], tank_maps[15][14], tank_maps[15][15], tank_maps[15][16], tank_maps[15][17], tank_maps[15][18], tank_maps[15][19], tank_maps[15][20], tank_maps[15][21], tank_maps[15][22], tank_maps[15][23], tank_maps[15][24], tank_maps[15][25], tank_maps[15][26], tank_maps[15][27], tank_maps[15][28], tank_maps[15][29], tank_maps[15][30], tank_maps[15][31], tank_maps[15][32], tank_maps[15][33], tank_maps[15][34], tank_maps[15][35], tank_maps[15][36], tank_maps[15][37]};

  xpm_row_t *ytank_maps[NSPRITES_TANK] = {ytank_E, ytank_ENE, ytank_NE, ytank_NNE, ytank_N, ytank_NNO, ytank_NO, ytank_ONO, ytank_O, ytank_OSO, ytank_SO, ytank_SSO, ytank_S, ytank_SSE, ytank_SE, ytank_ESE, tank_exp1, tank_exp2, tank_base};

  Tank_t * ytank = create_new_EnemyTank(*create_animSprite_spec(NCOLS * TILE_SIZE / 2, NROWS * TILE_SIZE / 2, 0, YTANK_SPEED, ANIMSPEED, NSPRITES_TANK, ytank_maps[0], ytank_maps[1], ytank_maps[2], ytank_maps[3], ytank_maps[4], ytank_maps[5], ytank_maps[6], ytank_maps[7], ytank_maps[8], ytank_maps[9], ytank_maps[10], ytank_maps[11], ytank_maps[12], ytank_maps[13], ytank_maps[14], ytank_maps[15], ytank_maps[16], ytank_maps[17], ytank_maps[18]), create_animSprite_spec(0, 0, 0, 0, ANIMSPEED, NSPRITES_BLACK_BALL, black_ball_maps[0], black_ball_maps[1], black_ball_maps[2]), YTANK_COOLDOWN_FRAMES, YTANK_MAX_BALLS, YTANK_BALL_REB, YTANK_BALL_STR, YTANK_BALL_VEL, YTANK, YTANK_HITS, YTANK_SCORE, YTANK_IFRAMES);

  if(right) ytank->asp.cur_fig = 0; 
  else ytank->asp.cur_fig = 8;

  ytank->asp.sp->x = x_init;
  ytank->asp.sp->y = y_init;

  add_tank(tanks, ytank);

  draw_animsprite(&ytank->asp);
}
/**
 * @brief creates an enemy tank that is static and shoots randomly and it's respective XPM's
 * 
 */
void (create_new_RSTank)(Tank_t * tanks[MAX_TANKS], double angle_init, uint16_t x_init, uint16_t y_init){
  xpm_row_t rstank_E[] = {tank_maps[0][0], RSTANK_LINE1, RSTANK_LINE2, RSTANK_LINE3, RSTANK_LINE4, tank_maps[0][5], tank_maps[0][6], tank_maps[0][7], tank_maps[0][8], tank_maps[0][9], tank_maps[0][10], tank_maps[0][11], tank_maps[0][12], tank_maps[0][13], tank_maps[0][14], tank_maps[0][15], tank_maps[0][16], tank_maps[0][17], tank_maps[0][18], tank_maps[0][19], tank_maps[0][20], tank_maps[0][21], tank_maps[0][22], tank_maps[0][23], tank_maps[0][24], tank_maps[0][25], tank_maps[0][26], tank_maps[0][27], tank_maps[0][28], tank_maps[0][29], tank_maps[0][30], tank_maps[0][31], tank_maps[0][32], tank_maps[0][33], tank_maps[0][34], tank_maps[0][35], tank_maps[0][36], tank_maps[0][37]};

  xpm_row_t rstank_ENE[] = {tank_maps[1][0], RSTANK_LINE1, RSTANK_LINE2, RSTANK_LINE3, RSTANK_LINE4, tank_maps[1][5], tank_maps[1][6], tank_maps[1][7], tank_maps[1][8], tank_maps[1][9], tank_maps[1][10], tank_maps[1][11], tank_maps[1][12], tank_maps[1][13], tank_maps[1][14], tank_maps[1][15], tank_maps[1][16], tank_maps[1][17], tank_maps[1][18], tank_maps[1][19], tank_maps[1][20], tank_maps[1][21], tank_maps[1][22], tank_maps[1][23], tank_maps[1][24], tank_maps[1][25], tank_maps[1][26], tank_maps[1][27], tank_maps[1][28], tank_maps[1][29], tank_maps[1][30], tank_maps[1][31], tank_maps[1][32], tank_maps[1][33], tank_maps[1][34], tank_maps[1][35], tank_maps[1][36], tank_maps[1][37]};

  xpm_row_t rstank_NE[] = {tank_maps[2][0], RSTANK_LINE1, RSTANK_LINE2, RSTANK_LINE3, RSTANK_LINE4, tank_maps[2][5], tank_maps[2][6], tank_maps[2][7], tank_maps[2][8], tank_maps[2][9], tank_maps[2][10], tank_maps[2][11], tank_maps[2][12], tank_maps[2][13], tank_maps[2][14], tank_maps[2][15], tank_maps[2][16], tank_maps[2][17], tank_maps[2][18], tank_maps[2][19], tank_maps[2][20], tank_maps[2][21], tank_maps[2][22], tank_maps[2][23], tank_maps[2][24], tank_maps[2][25], tank_maps[2][26], tank_maps[2][27], tank_maps[2][28], tank_maps[2][29], tank_maps[2][30], tank_maps[2][31], tank_maps[2][32], tank_maps[2][33], tank_maps[2][34], tank_maps[2][35], tank_maps[2][36], tank_maps[2][37]};

  xpm_row_t rstank_NNE[] = {tank_maps[3][0], RSTANK_LINE1, RSTANK_LINE2, RSTANK_LINE3, RSTANK_LINE4, tank_maps[3][5], tank_maps[3][6], tank_maps[3][7], tank_maps[3][8], tank_maps[3][9], tank_maps[3][10], tank_maps[3][11], tank_maps[3][12], tank_maps[3][13], tank_maps[3][14], tank_maps[3][15], tank_maps[3][16], tank_maps[3][17], tank_maps[3][18], tank_maps[3][19], tank_maps[3][20], tank_maps[3][21], tank_maps[3][22], tank_maps[3][23], tank_maps[3][24], tank_maps[3][25], tank_maps[3][26], tank_maps[3][27], tank_maps[3][28], tank_maps[3][29], tank_maps[3][30], tank_maps[3][31], tank_maps[3][32], tank_maps[3][33], tank_maps[3][34], tank_maps[3][35], tank_maps[3][36], tank_maps[3][37]};

  xpm_row_t rstank_N[] = {tank_maps[4][0], RSTANK_LINE1, RSTANK_LINE2, RSTANK_LINE3, RSTANK_LINE4, tank_maps[4][5], tank_maps[4][6], tank_maps[4][7], tank_maps[4][8], tank_maps[4][9], tank_maps[4][10], tank_maps[4][11], tank_maps[4][12], tank_maps[4][13], tank_maps[4][14], tank_maps[4][15], tank_maps[4][16], tank_maps[4][17], tank_maps[4][18], tank_maps[4][19], tank_maps[4][20], tank_maps[4][21], tank_maps[4][22], tank_maps[4][23], tank_maps[4][24], tank_maps[4][25], tank_maps[4][26], tank_maps[4][27], tank_maps[4][28], tank_maps[4][29], tank_maps[4][30], tank_maps[4][31], tank_maps[4][32], tank_maps[4][33], tank_maps[4][34], tank_maps[4][35], tank_maps[4][36], tank_maps[4][37]};

  xpm_row_t rstank_NNO[] = {tank_maps[5][0], RSTANK_LINE1, RSTANK_LINE2, RSTANK_LINE3, RSTANK_LINE4, tank_maps[5][5], tank_maps[5][6], tank_maps[5][7], tank_maps[5][8], tank_maps[5][9], tank_maps[5][10], tank_maps[5][11], tank_maps[5][12], tank_maps[5][13], tank_maps[5][14], tank_maps[5][15], tank_maps[5][16], tank_maps[5][17], tank_maps[5][18], tank_maps[5][19], tank_maps[5][20], tank_maps[5][21], tank_maps[5][22], tank_maps[5][23], tank_maps[5][24], tank_maps[5][25], tank_maps[5][26], tank_maps[5][27], tank_maps[5][28], tank_maps[5][29], tank_maps[5][30], tank_maps[5][31], tank_maps[5][32], tank_maps[5][33], tank_maps[5][34], tank_maps[5][35], tank_maps[5][36], tank_maps[5][37]};

  xpm_row_t rstank_NO[] = {tank_maps[6][0], RSTANK_LINE1, RSTANK_LINE2, RSTANK_LINE3, RSTANK_LINE4, tank_maps[6][5], tank_maps[6][6], tank_maps[6][7], tank_maps[6][8], tank_maps[6][9], tank_maps[6][10], tank_maps[6][11], tank_maps[6][12], tank_maps[6][13], tank_maps[6][14], tank_maps[6][15], tank_maps[6][16], tank_maps[6][17], tank_maps[6][18], tank_maps[6][19], tank_maps[6][20], tank_maps[6][21], tank_maps[6][22], tank_maps[6][23], tank_maps[6][24], tank_maps[6][25], tank_maps[6][26], tank_maps[6][27], tank_maps[6][28], tank_maps[6][29], tank_maps[6][30], tank_maps[6][31], tank_maps[6][32], tank_maps[6][33], tank_maps[6][34], tank_maps[6][35], tank_maps[6][36], tank_maps[6][37]};

  xpm_row_t rstank_ONO[] = {tank_maps[7][0], RSTANK_LINE1, RSTANK_LINE2, RSTANK_LINE3, RSTANK_LINE4, tank_maps[7][5], tank_maps[7][6], tank_maps[7][7], tank_maps[7][8], tank_maps[7][9], tank_maps[7][10], tank_maps[7][11], tank_maps[7][12], tank_maps[7][13], tank_maps[7][14], tank_maps[7][15], tank_maps[7][16], tank_maps[7][17], tank_maps[7][18], tank_maps[7][19], tank_maps[7][20], tank_maps[7][21], tank_maps[7][22], tank_maps[7][23], tank_maps[7][24], tank_maps[7][25], tank_maps[7][26], tank_maps[7][27], tank_maps[7][28], tank_maps[7][29], tank_maps[7][30], tank_maps[7][31], tank_maps[7][32], tank_maps[7][33], tank_maps[7][34], tank_maps[7][35], tank_maps[7][36], tank_maps[7][37]};

  xpm_row_t rstank_O[] = {tank_maps[8][0], RSTANK_LINE1, RSTANK_LINE2, RSTANK_LINE3, RSTANK_LINE4, tank_maps[8][5], tank_maps[8][6], tank_maps[8][7], tank_maps[8][8], tank_maps[8][9], tank_maps[8][10], tank_maps[8][11], tank_maps[8][12], tank_maps[8][13], tank_maps[8][14], tank_maps[8][15], tank_maps[8][16], tank_maps[8][17], tank_maps[8][18], tank_maps[8][19], tank_maps[8][20], tank_maps[8][21], tank_maps[8][22], tank_maps[8][23], tank_maps[8][24], tank_maps[8][25], tank_maps[8][26], tank_maps[8][27], tank_maps[8][28], tank_maps[8][29], tank_maps[8][30], tank_maps[8][31], tank_maps[8][32], tank_maps[8][33], tank_maps[8][34], tank_maps[8][35], tank_maps[8][36], tank_maps[8][37]};

  xpm_row_t rstank_OSO[] = {tank_maps[9][0], RSTANK_LINE1, RSTANK_LINE2, RSTANK_LINE3, RSTANK_LINE4, tank_maps[9][5], tank_maps[9][6], tank_maps[9][7], tank_maps[9][8], tank_maps[9][9], tank_maps[9][10], tank_maps[9][11], tank_maps[9][12], tank_maps[9][13], tank_maps[9][14], tank_maps[9][15], tank_maps[9][16], tank_maps[9][17], tank_maps[9][18], tank_maps[9][19], tank_maps[9][20], tank_maps[9][21], tank_maps[9][22], tank_maps[9][23], tank_maps[9][24], tank_maps[9][25], tank_maps[9][26], tank_maps[9][27], tank_maps[9][28], tank_maps[9][29], tank_maps[9][30], tank_maps[9][31], tank_maps[9][32], tank_maps[9][33], tank_maps[9][34], tank_maps[9][35], tank_maps[9][36], tank_maps[9][37]};

  xpm_row_t rstank_SO[] = {tank_maps[10][0], RSTANK_LINE1, RSTANK_LINE2, RSTANK_LINE3, RSTANK_LINE4, tank_maps[10][5], tank_maps[10][6], tank_maps[10][7], tank_maps[10][8], tank_maps[10][9], tank_maps[10][10], tank_maps[10][11], tank_maps[10][12], tank_maps[10][13], tank_maps[10][14], tank_maps[10][15], tank_maps[10][16], tank_maps[10][17], tank_maps[10][18], tank_maps[10][19], tank_maps[10][20], tank_maps[10][21], tank_maps[10][22], tank_maps[10][23], tank_maps[10][24], tank_maps[10][25], tank_maps[10][26], tank_maps[10][27], tank_maps[10][28], tank_maps[10][29], tank_maps[10][30], tank_maps[10][31], tank_maps[10][32], tank_maps[10][33], tank_maps[10][34], tank_maps[10][35], tank_maps[10][36], tank_maps[10][37]};

  xpm_row_t rstank_SSO[] = {tank_maps[11][0], RSTANK_LINE1, RSTANK_LINE2, RSTANK_LINE3, RSTANK_LINE4, tank_maps[11][5], tank_maps[11][6], tank_maps[11][7], tank_maps[11][8], tank_maps[11][9], tank_maps[11][10], tank_maps[11][11], tank_maps[11][12], tank_maps[11][13], tank_maps[11][14], tank_maps[11][15], tank_maps[11][16], tank_maps[11][17], tank_maps[11][18], tank_maps[11][19], tank_maps[11][20], tank_maps[11][21], tank_maps[11][22], tank_maps[11][23], tank_maps[11][24], tank_maps[11][25], tank_maps[11][26], tank_maps[11][27], tank_maps[11][28], tank_maps[11][29], tank_maps[11][30], tank_maps[11][31], tank_maps[11][32], tank_maps[11][33], tank_maps[11][34], tank_maps[11][35], tank_maps[11][36], tank_maps[11][37]};

  xpm_row_t rstank_S[] = {tank_maps[12][0], RSTANK_LINE1, RSTANK_LINE2, RSTANK_LINE3, RSTANK_LINE4, tank_maps[12][5], tank_maps[12][6], tank_maps[12][7], tank_maps[12][8], tank_maps[12][9], tank_maps[12][10], tank_maps[12][11], tank_maps[12][12], tank_maps[12][13], tank_maps[12][14], tank_maps[12][15], tank_maps[12][16], tank_maps[12][17], tank_maps[12][18], tank_maps[12][19], tank_maps[12][20], tank_maps[12][21], tank_maps[12][22], tank_maps[12][23], tank_maps[12][24], tank_maps[12][25], tank_maps[12][26], tank_maps[12][27], tank_maps[12][28], tank_maps[12][29], tank_maps[12][30], tank_maps[12][31], tank_maps[12][32], tank_maps[12][33], tank_maps[12][34], tank_maps[12][35], tank_maps[12][36], tank_maps[12][37]};

  xpm_row_t rstank_SSE[] = {tank_maps[13][0], RSTANK_LINE1, RSTANK_LINE2, RSTANK_LINE3, RSTANK_LINE4, tank_maps[13][5], tank_maps[13][6], tank_maps[13][7], tank_maps[13][8], tank_maps[13][9], tank_maps[13][10], tank_maps[13][11], tank_maps[13][12], tank_maps[13][13], tank_maps[13][14], tank_maps[13][15], tank_maps[13][16], tank_maps[13][17], tank_maps[13][18], tank_maps[13][19], tank_maps[13][20], tank_maps[13][21], tank_maps[13][22], tank_maps[13][23], tank_maps[13][24], tank_maps[13][25], tank_maps[13][26], tank_maps[13][27], tank_maps[13][28], tank_maps[13][29], tank_maps[13][30], tank_maps[13][31], tank_maps[13][32], tank_maps[13][33], tank_maps[13][34], tank_maps[13][35], tank_maps[13][36], tank_maps[13][37]};

  xpm_row_t rstank_SE[] = {tank_maps[14][0], RSTANK_LINE1, RSTANK_LINE2, RSTANK_LINE3, RSTANK_LINE4, tank_maps[14][5], tank_maps[14][6], tank_maps[14][7], tank_maps[14][8], tank_maps[14][9], tank_maps[14][10], tank_maps[14][11], tank_maps[14][12], tank_maps[14][13], tank_maps[14][14], tank_maps[14][15], tank_maps[14][16], tank_maps[14][17], tank_maps[14][18], tank_maps[14][19], tank_maps[14][20], tank_maps[14][21], tank_maps[14][22], tank_maps[14][23], tank_maps[14][24], tank_maps[14][25], tank_maps[14][26], tank_maps[14][27], tank_maps[14][28], tank_maps[14][29], tank_maps[14][30], tank_maps[14][31], tank_maps[14][32], tank_maps[14][33], tank_maps[14][34], tank_maps[14][35], tank_maps[14][36], tank_maps[14][37]};

  xpm_row_t rstank_ESE[] = {tank_maps[15][0], RSTANK_LINE1, RSTANK_LINE2, RSTANK_LINE3, RSTANK_LINE4, tank_maps[15][5], tank_maps[15][6], tank_maps[15][7], tank_maps[15][8], tank_maps[15][9], tank_maps[15][10], tank_maps[15][11], tank_maps[15][12], tank_maps[15][13], tank_maps[15][14], tank_maps[15][15], tank_maps[15][16], tank_maps[15][17], tank_maps[15][18], tank_maps[15][19], tank_maps[15][20], tank_maps[15][21], tank_maps[15][22], tank_maps[15][23], tank_maps[15][24], tank_maps[15][25], tank_maps[15][26], tank_maps[15][27], tank_maps[15][28], tank_maps[15][29], tank_maps[15][30], tank_maps[15][31], tank_maps[15][32], tank_maps[15][33], tank_maps[15][34], tank_maps[15][35], tank_maps[15][36], tank_maps[15][37]};

  xpm_row_t *rstank_maps[NSPRITES_TANK] = {rstank_E, rstank_ENE, rstank_NE, rstank_NNE, rstank_N, rstank_NNO, rstank_NO, rstank_ONO, rstank_O, rstank_OSO, rstank_SO, rstank_SSO, rstank_S, rstank_SSE, rstank_SE, rstank_ESE, tank_exp1, tank_exp2, tank_base};

  Tank_t * rstank = create_new_EnemyTank(*create_animSprite_spec(NCOLS * TILE_SIZE / 2, NROWS * TILE_SIZE / 2, 0, 0, ANIMSPEED, NSPRITES_TANK, rstank_maps[0], rstank_maps[1], rstank_maps[2], rstank_maps[3], rstank_maps[4], rstank_maps[5], rstank_maps[6], rstank_maps[7], rstank_maps[8], rstank_maps[9], rstank_maps[10], rstank_maps[11], rstank_maps[12], rstank_maps[13], rstank_maps[14], rstank_maps[15], rstank_maps[16], rstank_maps[17], rstank_maps[18]), create_animSprite_spec(0, 0, 0, 0, ANIMSPEED, NSPRITES_BLACK_BALL, black_ball_maps[0], black_ball_maps[1], black_ball_maps[2]), RSTANK_COOLDOWN_FRAMES, RSTANK_MAX_BALLS, RSTANK_BALL_REB, RSTANK_BALL_STR, RSTANK_BALL_VEL, RANDOM_STATIC_TANK, RSTANK_HITS, RSTANK_SCORE, RSTANK_IFRAMES);

  rstank->asp.sp->x = x_init;
  rstank->asp.sp->y = y_init;

  rstank->angle = angle_init;

  set_slope_tank(rstank);
  update_tank_sprite(rstank);

  add_tank(tanks, rstank);

  draw_animsprite(&rstank->asp);
}
/**
 * @brief creates an enemy tank that is static and shoots directly at the player and it's respective XPM's
 * 
 */
void (create_new_DSTank)(Tank_t * tanks[MAX_TANKS], double angle_init, uint16_t x_init, uint16_t y_init){
  xpm_row_t dstank_E[] = {tank_maps[0][0], DSTANK_LINE1, DSTANK_LINE2, DSTANK_LINE3, DSTANK_LINE4, tank_maps[0][5], tank_maps[0][6], tank_maps[0][7], tank_maps[0][8], tank_maps[0][9], tank_maps[0][10], tank_maps[0][11], tank_maps[0][12], tank_maps[0][13], tank_maps[0][14], tank_maps[0][15], tank_maps[0][16], tank_maps[0][17], tank_maps[0][18], tank_maps[0][19], tank_maps[0][20], tank_maps[0][21], tank_maps[0][22], tank_maps[0][23], tank_maps[0][24], tank_maps[0][25], tank_maps[0][26], tank_maps[0][27], tank_maps[0][28], tank_maps[0][29], tank_maps[0][30], tank_maps[0][31], tank_maps[0][32], tank_maps[0][33], tank_maps[0][34], tank_maps[0][35], tank_maps[0][36], tank_maps[0][37]};

  xpm_row_t dstank_ENE[] = {tank_maps[1][0], DSTANK_LINE1, DSTANK_LINE2, DSTANK_LINE3, DSTANK_LINE4, tank_maps[1][5], tank_maps[1][6], tank_maps[1][7], tank_maps[1][8], tank_maps[1][9], tank_maps[1][10], tank_maps[1][11], tank_maps[1][12], tank_maps[1][13], tank_maps[1][14], tank_maps[1][15], tank_maps[1][16], tank_maps[1][17], tank_maps[1][18], tank_maps[1][19], tank_maps[1][20], tank_maps[1][21], tank_maps[1][22], tank_maps[1][23], tank_maps[1][24], tank_maps[1][25], tank_maps[1][26], tank_maps[1][27], tank_maps[1][28], tank_maps[1][29], tank_maps[1][30], tank_maps[1][31], tank_maps[1][32], tank_maps[1][33], tank_maps[1][34], tank_maps[1][35], tank_maps[1][36], tank_maps[1][37]};

  xpm_row_t dstank_NE[] = {tank_maps[2][0], DSTANK_LINE1, DSTANK_LINE2, DSTANK_LINE3, DSTANK_LINE4, tank_maps[2][5], tank_maps[2][6], tank_maps[2][7], tank_maps[2][8], tank_maps[2][9], tank_maps[2][10], tank_maps[2][11], tank_maps[2][12], tank_maps[2][13], tank_maps[2][14], tank_maps[2][15], tank_maps[2][16], tank_maps[2][17], tank_maps[2][18], tank_maps[2][19], tank_maps[2][20], tank_maps[2][21], tank_maps[2][22], tank_maps[2][23], tank_maps[2][24], tank_maps[2][25], tank_maps[2][26], tank_maps[2][27], tank_maps[2][28], tank_maps[2][29], tank_maps[2][30], tank_maps[2][31], tank_maps[2][32], tank_maps[2][33], tank_maps[2][34], tank_maps[2][35], tank_maps[2][36], tank_maps[2][37]};

  xpm_row_t dstank_NNE[] = {tank_maps[3][0], DSTANK_LINE1, DSTANK_LINE2, DSTANK_LINE3, DSTANK_LINE4, tank_maps[3][5], tank_maps[3][6], tank_maps[3][7], tank_maps[3][8], tank_maps[3][9], tank_maps[3][10], tank_maps[3][11], tank_maps[3][12], tank_maps[3][13], tank_maps[3][14], tank_maps[3][15], tank_maps[3][16], tank_maps[3][17], tank_maps[3][18], tank_maps[3][19], tank_maps[3][20], tank_maps[3][21], tank_maps[3][22], tank_maps[3][23], tank_maps[3][24], tank_maps[3][25], tank_maps[3][26], tank_maps[3][27], tank_maps[3][28], tank_maps[3][29], tank_maps[3][30], tank_maps[3][31], tank_maps[3][32], tank_maps[3][33], tank_maps[3][34], tank_maps[3][35], tank_maps[3][36], tank_maps[3][37]};

  xpm_row_t dstank_N[] = {tank_maps[4][0], DSTANK_LINE1, DSTANK_LINE2, DSTANK_LINE3, DSTANK_LINE4, tank_maps[4][5], tank_maps[4][6], tank_maps[4][7], tank_maps[4][8], tank_maps[4][9], tank_maps[4][10], tank_maps[4][11], tank_maps[4][12], tank_maps[4][13], tank_maps[4][14], tank_maps[4][15], tank_maps[4][16], tank_maps[4][17], tank_maps[4][18], tank_maps[4][19], tank_maps[4][20], tank_maps[4][21], tank_maps[4][22], tank_maps[4][23], tank_maps[4][24], tank_maps[4][25], tank_maps[4][26], tank_maps[4][27], tank_maps[4][28], tank_maps[4][29], tank_maps[4][30], tank_maps[4][31], tank_maps[4][32], tank_maps[4][33], tank_maps[4][34], tank_maps[4][35], tank_maps[4][36], tank_maps[4][37]};

  xpm_row_t dstank_NNO[] = {tank_maps[5][0], DSTANK_LINE1, DSTANK_LINE2, DSTANK_LINE3, DSTANK_LINE4, tank_maps[5][5], tank_maps[5][6], tank_maps[5][7], tank_maps[5][8], tank_maps[5][9], tank_maps[5][10], tank_maps[5][11], tank_maps[5][12], tank_maps[5][13], tank_maps[5][14], tank_maps[5][15], tank_maps[5][16], tank_maps[5][17], tank_maps[5][18], tank_maps[5][19], tank_maps[5][20], tank_maps[5][21], tank_maps[5][22], tank_maps[5][23], tank_maps[5][24], tank_maps[5][25], tank_maps[5][26], tank_maps[5][27], tank_maps[5][28], tank_maps[5][29], tank_maps[5][30], tank_maps[5][31], tank_maps[5][32], tank_maps[5][33], tank_maps[5][34], tank_maps[5][35], tank_maps[5][36], tank_maps[5][37]};

  xpm_row_t dstank_NO[] = {tank_maps[6][0], DSTANK_LINE1, DSTANK_LINE2, DSTANK_LINE3, DSTANK_LINE4, tank_maps[6][5], tank_maps[6][6], tank_maps[6][7], tank_maps[6][8], tank_maps[6][9], tank_maps[6][10], tank_maps[6][11], tank_maps[6][12], tank_maps[6][13], tank_maps[6][14], tank_maps[6][15], tank_maps[6][16], tank_maps[6][17], tank_maps[6][18], tank_maps[6][19], tank_maps[6][20], tank_maps[6][21], tank_maps[6][22], tank_maps[6][23], tank_maps[6][24], tank_maps[6][25], tank_maps[6][26], tank_maps[6][27], tank_maps[6][28], tank_maps[6][29], tank_maps[6][30], tank_maps[6][31], tank_maps[6][32], tank_maps[6][33], tank_maps[6][34], tank_maps[6][35], tank_maps[6][36], tank_maps[6][37]};

  xpm_row_t dstank_ONO[] = {tank_maps[7][0], DSTANK_LINE1, DSTANK_LINE2, DSTANK_LINE3, DSTANK_LINE4, tank_maps[7][5], tank_maps[7][6], tank_maps[7][7], tank_maps[7][8], tank_maps[7][9], tank_maps[7][10], tank_maps[7][11], tank_maps[7][12], tank_maps[7][13], tank_maps[7][14], tank_maps[7][15], tank_maps[7][16], tank_maps[7][17], tank_maps[7][18], tank_maps[7][19], tank_maps[7][20], tank_maps[7][21], tank_maps[7][22], tank_maps[7][23], tank_maps[7][24], tank_maps[7][25], tank_maps[7][26], tank_maps[7][27], tank_maps[7][28], tank_maps[7][29], tank_maps[7][30], tank_maps[7][31], tank_maps[7][32], tank_maps[7][33], tank_maps[7][34], tank_maps[7][35], tank_maps[7][36], tank_maps[7][37]};

  xpm_row_t dstank_O[] = {tank_maps[8][0], DSTANK_LINE1, DSTANK_LINE2, DSTANK_LINE3, DSTANK_LINE4, tank_maps[8][5], tank_maps[8][6], tank_maps[8][7], tank_maps[8][8], tank_maps[8][9], tank_maps[8][10], tank_maps[8][11], tank_maps[8][12], tank_maps[8][13], tank_maps[8][14], tank_maps[8][15], tank_maps[8][16], tank_maps[8][17], tank_maps[8][18], tank_maps[8][19], tank_maps[8][20], tank_maps[8][21], tank_maps[8][22], tank_maps[8][23], tank_maps[8][24], tank_maps[8][25], tank_maps[8][26], tank_maps[8][27], tank_maps[8][28], tank_maps[8][29], tank_maps[8][30], tank_maps[8][31], tank_maps[8][32], tank_maps[8][33], tank_maps[8][34], tank_maps[8][35], tank_maps[8][36], tank_maps[8][37]};

  xpm_row_t dstank_OSO[] = {tank_maps[9][0], DSTANK_LINE1, DSTANK_LINE2, DSTANK_LINE3, DSTANK_LINE4, tank_maps[9][5], tank_maps[9][6], tank_maps[9][7], tank_maps[9][8], tank_maps[9][9], tank_maps[9][10], tank_maps[9][11], tank_maps[9][12], tank_maps[9][13], tank_maps[9][14], tank_maps[9][15], tank_maps[9][16], tank_maps[9][17], tank_maps[9][18], tank_maps[9][19], tank_maps[9][20], tank_maps[9][21], tank_maps[9][22], tank_maps[9][23], tank_maps[9][24], tank_maps[9][25], tank_maps[9][26], tank_maps[9][27], tank_maps[9][28], tank_maps[9][29], tank_maps[9][30], tank_maps[9][31], tank_maps[9][32], tank_maps[9][33], tank_maps[9][34], tank_maps[9][35], tank_maps[9][36], tank_maps[9][37]};

  xpm_row_t dstank_SO[] = {tank_maps[10][0], DSTANK_LINE1, DSTANK_LINE2, DSTANK_LINE3, DSTANK_LINE4, tank_maps[10][5], tank_maps[10][6], tank_maps[10][7], tank_maps[10][8], tank_maps[10][9], tank_maps[10][10], tank_maps[10][11], tank_maps[10][12], tank_maps[10][13], tank_maps[10][14], tank_maps[10][15], tank_maps[10][16], tank_maps[10][17], tank_maps[10][18], tank_maps[10][19], tank_maps[10][20], tank_maps[10][21], tank_maps[10][22], tank_maps[10][23], tank_maps[10][24], tank_maps[10][25], tank_maps[10][26], tank_maps[10][27], tank_maps[10][28], tank_maps[10][29], tank_maps[10][30], tank_maps[10][31], tank_maps[10][32], tank_maps[10][33], tank_maps[10][34], tank_maps[10][35], tank_maps[10][36], tank_maps[10][37]};

  xpm_row_t dstank_SSO[] = {tank_maps[11][0], DSTANK_LINE1, DSTANK_LINE2, DSTANK_LINE3, DSTANK_LINE4, tank_maps[11][5], tank_maps[11][6], tank_maps[11][7], tank_maps[11][8], tank_maps[11][9], tank_maps[11][10], tank_maps[11][11], tank_maps[11][12], tank_maps[11][13], tank_maps[11][14], tank_maps[11][15], tank_maps[11][16], tank_maps[11][17], tank_maps[11][18], tank_maps[11][19], tank_maps[11][20], tank_maps[11][21], tank_maps[11][22], tank_maps[11][23], tank_maps[11][24], tank_maps[11][25], tank_maps[11][26], tank_maps[11][27], tank_maps[11][28], tank_maps[11][29], tank_maps[11][30], tank_maps[11][31], tank_maps[11][32], tank_maps[11][33], tank_maps[11][34], tank_maps[11][35], tank_maps[11][36], tank_maps[11][37]};

  xpm_row_t dstank_S[] = {tank_maps[12][0], DSTANK_LINE1, DSTANK_LINE2, DSTANK_LINE3, DSTANK_LINE4, tank_maps[12][5], tank_maps[12][6], tank_maps[12][7], tank_maps[12][8], tank_maps[12][9], tank_maps[12][10], tank_maps[12][11], tank_maps[12][12], tank_maps[12][13], tank_maps[12][14], tank_maps[12][15], tank_maps[12][16], tank_maps[12][17], tank_maps[12][18], tank_maps[12][19], tank_maps[12][20], tank_maps[12][21], tank_maps[12][22], tank_maps[12][23], tank_maps[12][24], tank_maps[12][25], tank_maps[12][26], tank_maps[12][27], tank_maps[12][28], tank_maps[12][29], tank_maps[12][30], tank_maps[12][31], tank_maps[12][32], tank_maps[12][33], tank_maps[12][34], tank_maps[12][35], tank_maps[12][36], tank_maps[12][37]};

  xpm_row_t dstank_SSE[] = {tank_maps[13][0], DSTANK_LINE1, DSTANK_LINE2, DSTANK_LINE3, DSTANK_LINE4, tank_maps[13][5], tank_maps[13][6], tank_maps[13][7], tank_maps[13][8], tank_maps[13][9], tank_maps[13][10], tank_maps[13][11], tank_maps[13][12], tank_maps[13][13], tank_maps[13][14], tank_maps[13][15], tank_maps[13][16], tank_maps[13][17], tank_maps[13][18], tank_maps[13][19], tank_maps[13][20], tank_maps[13][21], tank_maps[13][22], tank_maps[13][23], tank_maps[13][24], tank_maps[13][25], tank_maps[13][26], tank_maps[13][27], tank_maps[13][28], tank_maps[13][29], tank_maps[13][30], tank_maps[13][31], tank_maps[13][32], tank_maps[13][33], tank_maps[13][34], tank_maps[13][35], tank_maps[13][36], tank_maps[13][37]};

  xpm_row_t dstank_SE[] = {tank_maps[14][0], DSTANK_LINE1, DSTANK_LINE2, DSTANK_LINE3, DSTANK_LINE4, tank_maps[14][5], tank_maps[14][6], tank_maps[14][7], tank_maps[14][8], tank_maps[14][9], tank_maps[14][10], tank_maps[14][11], tank_maps[14][12], tank_maps[14][13], tank_maps[14][14], tank_maps[14][15], tank_maps[14][16], tank_maps[14][17], tank_maps[14][18], tank_maps[14][19], tank_maps[14][20], tank_maps[14][21], tank_maps[14][22], tank_maps[14][23], tank_maps[14][24], tank_maps[14][25], tank_maps[14][26], tank_maps[14][27], tank_maps[14][28], tank_maps[14][29], tank_maps[14][30], tank_maps[14][31], tank_maps[14][32], tank_maps[14][33], tank_maps[14][34], tank_maps[14][35], tank_maps[14][36], tank_maps[14][37]};

  xpm_row_t dstank_ESE[] = {tank_maps[15][0], DSTANK_LINE1, DSTANK_LINE2, DSTANK_LINE3, DSTANK_LINE4, tank_maps[15][5], tank_maps[15][6], tank_maps[15][7], tank_maps[15][8], tank_maps[15][9], tank_maps[15][10], tank_maps[15][11], tank_maps[15][12], tank_maps[15][13], tank_maps[15][14], tank_maps[15][15], tank_maps[15][16], tank_maps[15][17], tank_maps[15][18], tank_maps[15][19], tank_maps[15][20], tank_maps[15][21], tank_maps[15][22], tank_maps[15][23], tank_maps[15][24], tank_maps[15][25], tank_maps[15][26], tank_maps[15][27], tank_maps[15][28], tank_maps[15][29], tank_maps[15][30], tank_maps[15][31], tank_maps[15][32], tank_maps[15][33], tank_maps[15][34], tank_maps[15][35], tank_maps[15][36], tank_maps[15][37]};

  xpm_row_t *dstank_maps[NSPRITES_TANK] = {dstank_E, dstank_ENE, dstank_NE, dstank_NNE, dstank_N, dstank_NNO, dstank_NO, dstank_ONO, dstank_O, dstank_OSO, dstank_SO, dstank_SSO, dstank_S, dstank_SSE, dstank_SE, dstank_ESE, tank_exp1, tank_exp2, tank_base};

  Tank_t * dstank = create_new_EnemyTank(*create_animSprite_spec(NCOLS * TILE_SIZE / 2, NROWS * TILE_SIZE / 2, 0, 0, ANIMSPEED, NSPRITES_TANK, dstank_maps[0], dstank_maps[1], dstank_maps[2], dstank_maps[3], dstank_maps[4], dstank_maps[5], dstank_maps[6], dstank_maps[7], dstank_maps[8], dstank_maps[9], dstank_maps[10], dstank_maps[11], dstank_maps[12], dstank_maps[13], dstank_maps[14], dstank_maps[15], dstank_maps[16], dstank_maps[17], dstank_maps[18]), create_animSprite_spec(0, 0, 0, 0, ANIMSPEED, NSPRITES_BLACK_BALL, black_ball_maps[0], black_ball_maps[1], black_ball_maps[2]), DSTANK_COOLDOWN_FRAMES, DSTANK_MAX_BALLS, DSTANK_BALL_REB, DSTANK_BALL_STR, DSTANK_BALL_VEL, DIRECT_STATIC_TANK, DSTANK_HITS, DSTANK_SCORE, DSTANK_IFRAMES);

  dstank->asp.sp->x = x_init;
  dstank->asp.sp->y = y_init;

  dstank->angle = angle_init;

  set_slope_tank(dstank);
  update_tank_sprite(dstank);

  add_tank(tanks, dstank);

  draw_animsprite(&dstank->asp);
}
/**
 * @brief creates an enemy tank that is static and shoots to the walls and it's respective XPM's
 * 
 */
void (create_new_ISTank)(Tank_t * tanks[MAX_TANKS], double angle_init, uint16_t x_init, uint16_t y_init){
  xpm_row_t istank_E[] = {tank_maps[0][0], ISTANK_LINE1, ISTANK_LINE2, ISTANK_LINE3, ISTANK_LINE4, tank_maps[0][5], tank_maps[0][6], tank_maps[0][7], tank_maps[0][8], tank_maps[0][9], tank_maps[0][10], tank_maps[0][11], tank_maps[0][12], tank_maps[0][13], tank_maps[0][14], tank_maps[0][15], tank_maps[0][16], tank_maps[0][17], tank_maps[0][18], tank_maps[0][19], tank_maps[0][20], tank_maps[0][21], tank_maps[0][22], tank_maps[0][23], tank_maps[0][24], tank_maps[0][25], tank_maps[0][26], tank_maps[0][27], tank_maps[0][28], tank_maps[0][29], tank_maps[0][30], tank_maps[0][31], tank_maps[0][32], tank_maps[0][33], tank_maps[0][34], tank_maps[0][35], tank_maps[0][36], tank_maps[0][37]};

  xpm_row_t istank_ENE[] = {tank_maps[1][0], ISTANK_LINE1, ISTANK_LINE2, ISTANK_LINE3, ISTANK_LINE4, tank_maps[1][5], tank_maps[1][6], tank_maps[1][7], tank_maps[1][8], tank_maps[1][9], tank_maps[1][10], tank_maps[1][11], tank_maps[1][12], tank_maps[1][13], tank_maps[1][14], tank_maps[1][15], tank_maps[1][16], tank_maps[1][17], tank_maps[1][18], tank_maps[1][19], tank_maps[1][20], tank_maps[1][21], tank_maps[1][22], tank_maps[1][23], tank_maps[1][24], tank_maps[1][25], tank_maps[1][26], tank_maps[1][27], tank_maps[1][28], tank_maps[1][29], tank_maps[1][30], tank_maps[1][31], tank_maps[1][32], tank_maps[1][33], tank_maps[1][34], tank_maps[1][35], tank_maps[1][36], tank_maps[1][37]};

  xpm_row_t istank_NE[] = {tank_maps[2][0], ISTANK_LINE1, ISTANK_LINE2, ISTANK_LINE3, ISTANK_LINE4, tank_maps[2][5], tank_maps[2][6], tank_maps[2][7], tank_maps[2][8], tank_maps[2][9], tank_maps[2][10], tank_maps[2][11], tank_maps[2][12], tank_maps[2][13], tank_maps[2][14], tank_maps[2][15], tank_maps[2][16], tank_maps[2][17], tank_maps[2][18], tank_maps[2][19], tank_maps[2][20], tank_maps[2][21], tank_maps[2][22], tank_maps[2][23], tank_maps[2][24], tank_maps[2][25], tank_maps[2][26], tank_maps[2][27], tank_maps[2][28], tank_maps[2][29], tank_maps[2][30], tank_maps[2][31], tank_maps[2][32], tank_maps[2][33], tank_maps[2][34], tank_maps[2][35], tank_maps[2][36], tank_maps[2][37]};

  xpm_row_t istank_NNE[] = {tank_maps[3][0], ISTANK_LINE1, ISTANK_LINE2, ISTANK_LINE3, ISTANK_LINE4, tank_maps[3][5], tank_maps[3][6], tank_maps[3][7], tank_maps[3][8], tank_maps[3][9], tank_maps[3][10], tank_maps[3][11], tank_maps[3][12], tank_maps[3][13], tank_maps[3][14], tank_maps[3][15], tank_maps[3][16], tank_maps[3][17], tank_maps[3][18], tank_maps[3][19], tank_maps[3][20], tank_maps[3][21], tank_maps[3][22], tank_maps[3][23], tank_maps[3][24], tank_maps[3][25], tank_maps[3][26], tank_maps[3][27], tank_maps[3][28], tank_maps[3][29], tank_maps[3][30], tank_maps[3][31], tank_maps[3][32], tank_maps[3][33], tank_maps[3][34], tank_maps[3][35], tank_maps[3][36], tank_maps[3][37]};

  xpm_row_t istank_N[] = {tank_maps[4][0], ISTANK_LINE1, ISTANK_LINE2, ISTANK_LINE3, ISTANK_LINE4, tank_maps[4][5], tank_maps[4][6], tank_maps[4][7], tank_maps[4][8], tank_maps[4][9], tank_maps[4][10], tank_maps[4][11], tank_maps[4][12], tank_maps[4][13], tank_maps[4][14], tank_maps[4][15], tank_maps[4][16], tank_maps[4][17], tank_maps[4][18], tank_maps[4][19], tank_maps[4][20], tank_maps[4][21], tank_maps[4][22], tank_maps[4][23], tank_maps[4][24], tank_maps[4][25], tank_maps[4][26], tank_maps[4][27], tank_maps[4][28], tank_maps[4][29], tank_maps[4][30], tank_maps[4][31], tank_maps[4][32], tank_maps[4][33], tank_maps[4][34], tank_maps[4][35], tank_maps[4][36], tank_maps[4][37]};

  xpm_row_t istank_NNO[] = {tank_maps[5][0], ISTANK_LINE1, ISTANK_LINE2, ISTANK_LINE3, ISTANK_LINE4, tank_maps[5][5], tank_maps[5][6], tank_maps[5][7], tank_maps[5][8], tank_maps[5][9], tank_maps[5][10], tank_maps[5][11], tank_maps[5][12], tank_maps[5][13], tank_maps[5][14], tank_maps[5][15], tank_maps[5][16], tank_maps[5][17], tank_maps[5][18], tank_maps[5][19], tank_maps[5][20], tank_maps[5][21], tank_maps[5][22], tank_maps[5][23], tank_maps[5][24], tank_maps[5][25], tank_maps[5][26], tank_maps[5][27], tank_maps[5][28], tank_maps[5][29], tank_maps[5][30], tank_maps[5][31], tank_maps[5][32], tank_maps[5][33], tank_maps[5][34], tank_maps[5][35], tank_maps[5][36], tank_maps[5][37]};

  xpm_row_t istank_NO[] = {tank_maps[6][0], ISTANK_LINE1, ISTANK_LINE2, ISTANK_LINE3, ISTANK_LINE4, tank_maps[6][5], tank_maps[6][6], tank_maps[6][7], tank_maps[6][8], tank_maps[6][9], tank_maps[6][10], tank_maps[6][11], tank_maps[6][12], tank_maps[6][13], tank_maps[6][14], tank_maps[6][15], tank_maps[6][16], tank_maps[6][17], tank_maps[6][18], tank_maps[6][19], tank_maps[6][20], tank_maps[6][21], tank_maps[6][22], tank_maps[6][23], tank_maps[6][24], tank_maps[6][25], tank_maps[6][26], tank_maps[6][27], tank_maps[6][28], tank_maps[6][29], tank_maps[6][30], tank_maps[6][31], tank_maps[6][32], tank_maps[6][33], tank_maps[6][34], tank_maps[6][35], tank_maps[6][36], tank_maps[6][37]};

  xpm_row_t istank_ONO[] = {tank_maps[7][0], ISTANK_LINE1, ISTANK_LINE2, ISTANK_LINE3, ISTANK_LINE4, tank_maps[7][5], tank_maps[7][6], tank_maps[7][7], tank_maps[7][8], tank_maps[7][9], tank_maps[7][10], tank_maps[7][11], tank_maps[7][12], tank_maps[7][13], tank_maps[7][14], tank_maps[7][15], tank_maps[7][16], tank_maps[7][17], tank_maps[7][18], tank_maps[7][19], tank_maps[7][20], tank_maps[7][21], tank_maps[7][22], tank_maps[7][23], tank_maps[7][24], tank_maps[7][25], tank_maps[7][26], tank_maps[7][27], tank_maps[7][28], tank_maps[7][29], tank_maps[7][30], tank_maps[7][31], tank_maps[7][32], tank_maps[7][33], tank_maps[7][34], tank_maps[7][35], tank_maps[7][36], tank_maps[7][37]};

  xpm_row_t istank_O[] = {tank_maps[8][0], ISTANK_LINE1, ISTANK_LINE2, ISTANK_LINE3, ISTANK_LINE4, tank_maps[8][5], tank_maps[8][6], tank_maps[8][7], tank_maps[8][8], tank_maps[8][9], tank_maps[8][10], tank_maps[8][11], tank_maps[8][12], tank_maps[8][13], tank_maps[8][14], tank_maps[8][15], tank_maps[8][16], tank_maps[8][17], tank_maps[8][18], tank_maps[8][19], tank_maps[8][20], tank_maps[8][21], tank_maps[8][22], tank_maps[8][23], tank_maps[8][24], tank_maps[8][25], tank_maps[8][26], tank_maps[8][27], tank_maps[8][28], tank_maps[8][29], tank_maps[8][30], tank_maps[8][31], tank_maps[8][32], tank_maps[8][33], tank_maps[8][34], tank_maps[8][35], tank_maps[8][36], tank_maps[8][37]};

  xpm_row_t istank_OSO[] = {tank_maps[9][0], ISTANK_LINE1, ISTANK_LINE2, ISTANK_LINE3, ISTANK_LINE4, tank_maps[9][5], tank_maps[9][6], tank_maps[9][7], tank_maps[9][8], tank_maps[9][9], tank_maps[9][10], tank_maps[9][11], tank_maps[9][12], tank_maps[9][13], tank_maps[9][14], tank_maps[9][15], tank_maps[9][16], tank_maps[9][17], tank_maps[9][18], tank_maps[9][19], tank_maps[9][20], tank_maps[9][21], tank_maps[9][22], tank_maps[9][23], tank_maps[9][24], tank_maps[9][25], tank_maps[9][26], tank_maps[9][27], tank_maps[9][28], tank_maps[9][29], tank_maps[9][30], tank_maps[9][31], tank_maps[9][32], tank_maps[9][33], tank_maps[9][34], tank_maps[9][35], tank_maps[9][36], tank_maps[9][37]};

  xpm_row_t istank_SO[] = {tank_maps[10][0], ISTANK_LINE1, ISTANK_LINE2, ISTANK_LINE3, ISTANK_LINE4, tank_maps[10][5], tank_maps[10][6], tank_maps[10][7], tank_maps[10][8], tank_maps[10][9], tank_maps[10][10], tank_maps[10][11], tank_maps[10][12], tank_maps[10][13], tank_maps[10][14], tank_maps[10][15], tank_maps[10][16], tank_maps[10][17], tank_maps[10][18], tank_maps[10][19], tank_maps[10][20], tank_maps[10][21], tank_maps[10][22], tank_maps[10][23], tank_maps[10][24], tank_maps[10][25], tank_maps[10][26], tank_maps[10][27], tank_maps[10][28], tank_maps[10][29], tank_maps[10][30], tank_maps[10][31], tank_maps[10][32], tank_maps[10][33], tank_maps[10][34], tank_maps[10][35], tank_maps[10][36], tank_maps[10][37]};

  xpm_row_t istank_SSO[] = {tank_maps[11][0], ISTANK_LINE1, ISTANK_LINE2, ISTANK_LINE3, ISTANK_LINE4, tank_maps[11][5], tank_maps[11][6], tank_maps[11][7], tank_maps[11][8], tank_maps[11][9], tank_maps[11][10], tank_maps[11][11], tank_maps[11][12], tank_maps[11][13], tank_maps[11][14], tank_maps[11][15], tank_maps[11][16], tank_maps[11][17], tank_maps[11][18], tank_maps[11][19], tank_maps[11][20], tank_maps[11][21], tank_maps[11][22], tank_maps[11][23], tank_maps[11][24], tank_maps[11][25], tank_maps[11][26], tank_maps[11][27], tank_maps[11][28], tank_maps[11][29], tank_maps[11][30], tank_maps[11][31], tank_maps[11][32], tank_maps[11][33], tank_maps[11][34], tank_maps[11][35], tank_maps[11][36], tank_maps[11][37]};

  xpm_row_t istank_S[] = {tank_maps[12][0], ISTANK_LINE1, ISTANK_LINE2, ISTANK_LINE3, ISTANK_LINE4, tank_maps[12][5], tank_maps[12][6], tank_maps[12][7], tank_maps[12][8], tank_maps[12][9], tank_maps[12][10], tank_maps[12][11], tank_maps[12][12], tank_maps[12][13], tank_maps[12][14], tank_maps[12][15], tank_maps[12][16], tank_maps[12][17], tank_maps[12][18], tank_maps[12][19], tank_maps[12][20], tank_maps[12][21], tank_maps[12][22], tank_maps[12][23], tank_maps[12][24], tank_maps[12][25], tank_maps[12][26], tank_maps[12][27], tank_maps[12][28], tank_maps[12][29], tank_maps[12][30], tank_maps[12][31], tank_maps[12][32], tank_maps[12][33], tank_maps[12][34], tank_maps[12][35], tank_maps[12][36], tank_maps[12][37]};

  xpm_row_t istank_SSE[] = {tank_maps[13][0], ISTANK_LINE1, ISTANK_LINE2, ISTANK_LINE3, ISTANK_LINE4, tank_maps[13][5], tank_maps[13][6], tank_maps[13][7], tank_maps[13][8], tank_maps[13][9], tank_maps[13][10], tank_maps[13][11], tank_maps[13][12], tank_maps[13][13], tank_maps[13][14], tank_maps[13][15], tank_maps[13][16], tank_maps[13][17], tank_maps[13][18], tank_maps[13][19], tank_maps[13][20], tank_maps[13][21], tank_maps[13][22], tank_maps[13][23], tank_maps[13][24], tank_maps[13][25], tank_maps[13][26], tank_maps[13][27], tank_maps[13][28], tank_maps[13][29], tank_maps[13][30], tank_maps[13][31], tank_maps[13][32], tank_maps[13][33], tank_maps[13][34], tank_maps[13][35], tank_maps[13][36], tank_maps[13][37]};

  xpm_row_t istank_SE[] = {tank_maps[14][0], ISTANK_LINE1, ISTANK_LINE2, ISTANK_LINE3, ISTANK_LINE4, tank_maps[14][5], tank_maps[14][6], tank_maps[14][7], tank_maps[14][8], tank_maps[14][9], tank_maps[14][10], tank_maps[14][11], tank_maps[14][12], tank_maps[14][13], tank_maps[14][14], tank_maps[14][15], tank_maps[14][16], tank_maps[14][17], tank_maps[14][18], tank_maps[14][19], tank_maps[14][20], tank_maps[14][21], tank_maps[14][22], tank_maps[14][23], tank_maps[14][24], tank_maps[14][25], tank_maps[14][26], tank_maps[14][27], tank_maps[14][28], tank_maps[14][29], tank_maps[14][30], tank_maps[14][31], tank_maps[14][32], tank_maps[14][33], tank_maps[14][34], tank_maps[14][35], tank_maps[14][36], tank_maps[14][37]};

  xpm_row_t istank_ESE[] = {tank_maps[15][0], ISTANK_LINE1, ISTANK_LINE2, ISTANK_LINE3, ISTANK_LINE4, tank_maps[15][5], tank_maps[15][6], tank_maps[15][7], tank_maps[15][8], tank_maps[15][9], tank_maps[15][10], tank_maps[15][11], tank_maps[15][12], tank_maps[15][13], tank_maps[15][14], tank_maps[15][15], tank_maps[15][16], tank_maps[15][17], tank_maps[15][18], tank_maps[15][19], tank_maps[15][20], tank_maps[15][21], tank_maps[15][22], tank_maps[15][23], tank_maps[15][24], tank_maps[15][25], tank_maps[15][26], tank_maps[15][27], tank_maps[15][28], tank_maps[15][29], tank_maps[15][30], tank_maps[15][31], tank_maps[15][32], tank_maps[15][33], tank_maps[15][34], tank_maps[15][35], tank_maps[15][36], tank_maps[15][37]};

  xpm_row_t *istank_maps[NSPRITES_TANK] = {istank_E, istank_ENE, istank_NE, istank_NNE, istank_N, istank_NNO, istank_NO, istank_ONO, istank_O, istank_OSO, istank_SO, istank_SSO, istank_S, istank_SSE, istank_SE, istank_ESE, tank_exp1, tank_exp2, tank_base};

  Tank_t * istank = create_new_EnemyTank(*create_animSprite_spec(NCOLS * TILE_SIZE / 2, NROWS * TILE_SIZE / 2, 0, 0, ANIMSPEED, NSPRITES_TANK, istank_maps[0], istank_maps[1], istank_maps[2], istank_maps[3], istank_maps[4], istank_maps[5], istank_maps[6], istank_maps[7], istank_maps[8], istank_maps[9], istank_maps[10], istank_maps[11], istank_maps[12], istank_maps[13], istank_maps[14], istank_maps[15], istank_maps[16], istank_maps[17], istank_maps[18]), create_animSprite_spec(0, 0, 0, 0, ANIMSPEED, NSPRITES_BLACK_BALL, black_ball_maps[0], black_ball_maps[1], black_ball_maps[2]), ISTANK_COOLDOWN_FRAMES, ISTANK_MAX_BALLS, ISTANK_BALL_REB, ISTANK_BALL_STR, ISTANK_BALL_VEL, INDIRECT_STATIC_TANK, ISTANK_HITS, ISTANK_SCORE, ISTANK_IFRAMES);

  istank->asp.sp->x = x_init;
  istank->asp.sp->y = y_init;

  istank->angle = angle_init;

  set_slope_tank(istank);
  update_tank_sprite(istank);

  add_tank(tanks, istank);

  draw_animsprite(&istank->asp);
}


//Struct Related
/**
 * @brief makes the ball state turn to BOOM
 * 
 */
int (destroy_ball)(Tank_t * tank, Ball_t * ball){

  int index = -1;
  for(int i = 0; tank->max_balls > i; i++){
    if(tank->balls[i].id == ball->id){
      index = i;
      break;
    }
  }

  if(index == -1) return 1;

  tank->balls[index].state = BOOM;


  if(tank->ball_count > 0) tank->ball_count--;

  return 0;
}
/**
 * @brief ads a tank to an array of pointers to tanks
 * 
 */
int (add_tank)(Tank_t * tanks[MAX_TANKS], Tank_t * tank){
  for(int i = 0; MAX_TANKS > i; i++){
    if(tanks[i] == NULL){
      tanks[i] = tank;
      return 0;
    }
  }
  return 1;
}
/**
 * @brief turns tank state to BOOM
 * 
 */
int (destroy_tank)(Tank_t * tanks[MAX_TANKS], int index){
  
  for(int i = 0; MAX_TANKS > i; i++){
    if(i == index){
      if(tanks[i]->hits <= 1) tanks[i]->state = BOOM;
      else{
        tanks[i]->hits--;
        tanks[i]->last_fig = tanks[i]->asp.cur_fig;
        tanks[i]->asp.cur_fig = 18;
        tanks[i]->asp.cur_aspeed = 0;
      }
      return 0;
    }
  }

  return 1;
}
/**
 * @brief defines the figure of the tank based on the angle with the aim
 * 
 */
void (update_tank_sprite)(Tank_t * PlayerTank){
  if(PlayerTank->angle < PI/16.0) PlayerTank->asp.cur_fig = 0;
  if(PlayerTank->angle >= PI/16.0 && PlayerTank->angle < 3*PI/16.0) PlayerTank->asp.cur_fig = 1;
  if(PlayerTank->angle >= 3*PI/16.0 && PlayerTank->angle < 5*PI/16.0) PlayerTank->asp.cur_fig = 2;
  if(PlayerTank->angle >= 5*PI/16.0 && PlayerTank->angle < 7*PI/16.0) PlayerTank->asp.cur_fig = 3;
  if(PlayerTank->angle >= 7*PI/16.0 && PlayerTank->angle < 9*PI/16.0) PlayerTank->asp.cur_fig = 4;
  if(PlayerTank->angle >= 9*PI/16.0 && PlayerTank->angle < 11*PI/16.0) PlayerTank->asp.cur_fig = 5;
  if(PlayerTank->angle >= 11*PI/16.0 && PlayerTank->angle < 13*PI/16.0) PlayerTank->asp.cur_fig = 6;
  if(PlayerTank->angle >= 13*PI/16.0 && PlayerTank->angle < 15*PI/16.0) PlayerTank->asp.cur_fig = 7;
  if(PlayerTank->angle >= 15*PI/16.0 && PlayerTank->angle < 17*PI/16.0) PlayerTank->asp.cur_fig = 8;
  if(PlayerTank->angle >= 17*PI/16.0 && PlayerTank->angle < 19*PI/16.0) PlayerTank->asp.cur_fig = 9;
  if(PlayerTank->angle >= 19*PI/16.0 && PlayerTank->angle < 21*PI/16.0) PlayerTank->asp.cur_fig = 10;
  if(PlayerTank->angle >= 21*PI/16.0 && PlayerTank->angle < 23*PI/16.0) PlayerTank->asp.cur_fig = 11;
  if(PlayerTank->angle >= 23*PI/16.0 && PlayerTank->angle < 25*PI/16.0) PlayerTank->asp.cur_fig = 12;
  if(PlayerTank->angle >= 25*PI/16.0 && PlayerTank->angle < 27*PI/16.0) PlayerTank->asp.cur_fig = 13;
  if(PlayerTank->angle >= 27*PI/16.0 && PlayerTank->angle < 29*PI/16.0) PlayerTank->asp.cur_fig = 14;
  if(PlayerTank->angle >= 29*PI/16.0 && PlayerTank->angle < 31*PI/16.0) PlayerTank->asp.cur_fig = 15;
  if(PlayerTank->angle >= 31*PI/16.0) PlayerTank->asp.cur_fig = 0;
}
/**
 * @brief checks if the aim is on the centre of the player's tank
 * 
 */
bool (check_tank_aim_colision)(Tank_t * tank){
  uint16_t tank_xmin = tank->asp.sp->x;
  uint16_t tank_ymin = tank->asp.sp->y;
  uint16_t tank_xmax = tank->asp.sp->x + tank->asp.sp->width;
  uint16_t tank_ymax = tank->asp.sp->y + tank->asp.sp->height;

  uint16_t aim_xmin = tank->aim->asp.sp->x;
  uint16_t aim_ymin = tank->aim->asp.sp->y;
  uint16_t aim_xmax = tank->aim->asp.sp->x + tank->aim->asp.sp->width;
  uint16_t aim_ymax = tank->aim->asp.sp->y + tank->aim->asp.sp->height;

  if(aim_xmin >= tank_xmin && aim_ymin >= tank_ymin && aim_xmax <= tank_xmax && aim_ymax <= tank_ymax) return 1;
  else return 0;
}
/**
 * @brief checks if there is a colision between a specified ball and other element and return the type of colision. Returns NO_COLISION if there isn't one
 * 
 */
Colision_t (check_ball_colision)(Tank_t * tank, Ball_t * ball, Tank_t * tanks[MAX_TANKS], Tile_t layout[][NCOLS], bool real){
  int frontier = 0;
  Coordinate_t colision_coord = check_colision_coord(ball, layout, &frontier);

  uint16_t ball_y_max_og = ball->asp.sp->y + ball->asp.sp->height -16;
  uint16_t ball_x_max_og = ball->asp.sp->x + ball->asp.sp->width;
  uint16_t ball_y_min_og = ball->asp.sp->y -16;
  uint16_t ball_x_min_og = ball->asp.sp->x;

  uint16_t ball_y_max = ball->asp.sp->y + ball->asp.sp->height + db_to_int(ball->vy) -16;
  uint16_t ball_x_max = ball->asp.sp->x + ball->asp.sp->width + db_to_int(ball->vx);
  uint16_t ball_y_min = ball->asp.sp->y + db_to_int(ball->vy) -16;
  uint16_t ball_x_min = ball->asp.sp->x + db_to_int(ball->vx);

  for(int i = 0; MAX_TANKS > i; i++){
    if(tanks[i] != NULL){
      if(tanks[i]->state == ALIVE){
        if(( (ball_y_max >= tanks[i]->asp.sp->y - 16 && ball_y_max <= tanks[i]->asp.sp->y + tanks[i]->asp.sp->height - 16) || (ball_y_min <= tanks[i]->asp.sp->y + tanks[i]->asp.sp->height - 16 && ball_y_min >= tanks[i]->asp.sp->y - 16) ) && ( (ball_x_max >= tanks[i]->asp.sp->x && ball_x_max <= tanks[i]->asp.sp->x + tanks[i]->asp.sp->width) || (ball_x_min <= tanks[i]->asp.sp->x + tanks[i]->asp.sp->width && ball_x_min >= tanks[i]->asp.sp->x) )){
          if(real){
            destroy_ball(tank, ball);
            if(tanks[i]->asp.cur_fig < 16) destroy_tank(tanks, i);
          }
          if(tanks[i]->type == TPLAYER) return PLAYER_C;
          else return TANK_C;
        }
      }

    for(int j = 0; tanks[i]->max_balls > j; j++){
      if(tanks[i]->balls[j].state == ALIVE && tanks[i]->balls[j].id != ball->id){
          if(( (ball_y_max >= tanks[i]->balls[j].asp.sp->y - 16 && ball_y_max <= tanks[i]->balls[j].asp.sp->y + tanks[i]->balls[j].asp.sp->height - 16) || (ball_y_min <= tanks[i]->balls[j].asp.sp->y + tanks[i]->balls[j].asp.sp->height - 16 && ball_y_min >= tanks[i]->balls[j].asp.sp->y - 16) ) && ( (ball_x_max >= tanks[i]->balls[j].asp.sp->x && ball_x_max <= tanks[i]->balls[j].asp.sp->x + tanks[i]->balls[j].asp.sp->width) || (ball_x_min <= tanks[i]->balls[j].asp.sp->x + tanks[i]->balls[j].asp.sp->width && ball_x_min >= tanks[i]->balls[j].asp.sp->x) )){
            if(real){
              destroy_ball(tanks[i], & tanks[i]->balls[j]);
              destroy_ball(tank, ball);
            }
            return BALL_C;
          }
        }
      }

    }
  }
  

  double x_frontier = 0;
  double y_frontier = 0;
  double ratio = 0;

  switch (colision_coord){
    case NCX:
      if(ball_x_min - ball_x_min_og != 0){
        //if(ball_x_min > XOFFSET) x_frontier = ball_x_min_og + ball->vx + (ball_x_min % TILE_SIZE);
        if(ball_x_min > XOFFSET) x_frontier = frontier;
        else x_frontier = 8;
        ratio = (x_frontier - ball_x_min_og) / (ball_x_min - ball_x_min_og);
        if(ball->rebounds <= 0){
          ball->asp.sp->x = db_to_int(x_frontier);
          ball->asp.sp->y = ball_y_min_og + db_to_int(ball->vy * ratio) + 16;
          destroy_ball(tank, ball);
          return SOLID_C;
        }
        else{
          //remaining = x_frontier - (ball_x_min_og + ball->vx);
          //ball->asp.sp->x = db_to_int((double) (x_frontier + remaining));
          ball->asp.sp->x = db_to_int((double) (x_frontier + (1 - ratio) * -ball->vx));
          ball->asp.sp->y = db_to_int((double) (ball_y_min_og + ball->vy)) + 16;
          ball->vx = -ball->vx;
          ball->rebounds--;
          return REBOUND_CX;
        }
      }
      break;

    case CX:
      if(ball_x_max - ball_x_max_og != 0){
        //if(ball_x_max < H_RES - XOFFSET) x_frontier = ball_x_max_og + ball->vx - (ball_x_max % TILE_SIZE);
        if(ball_x_max < H_RES - XOFFSET) x_frontier = frontier;
        else x_frontier = H_RES - XOFFSET;
        ratio = (x_frontier - ball_x_max_og) / (ball_x_max - ball_x_max_og);
        if(ball->rebounds <= 0){
          ball->asp.sp->x = db_to_int((double) (x_frontier - ball->asp.sp->width));
          ball->asp.sp->y = ball_y_min_og + db_to_int(ball->vy * ratio) + 16;
          destroy_ball(tank, ball);
          return SOLID_C;
        }
        else{
          //remaining = x_frontier - (ball_x_max_og + ball->vx);
          //ball->asp.sp->x = db_to_int((double) (x_frontier - remaining - ball->asp.sp->width));
          ball->asp.sp->x = db_to_int((double) (x_frontier + (1 - ratio) * -ball->vx - ball->asp.sp->width));
          ball->asp.sp->y = db_to_int((double) (ball_y_min_og + ball->vy)) + 16;
          ball->vx = -ball->vx;
          ball->rebounds--;
          return REBOUND_CX;
        }
      }
      break;

    case NCY:
      if(ball_y_min - ball_y_min_og != 0){
        //if(ball_y_min > YOFFSET) y_frontier = ball_y_min_og + ball->vy + (ball_y_min % TILE_SIZE);
        if(ball_y_min > YOFFSET) y_frontier = frontier;
        else y_frontier = YOFFSET;
        ratio = (y_frontier - 16 - ball_y_min_og) / (ball_y_min - ball_y_min_og);
        if(ball->rebounds <= 0){
          ball->asp.sp->y = db_to_int(y_frontier);
          ball->asp.sp->x = ball_x_min_og + db_to_int(ball->vx * ratio);
          destroy_ball(tank, ball);
          return SOLID_C;
        }
        else{
          //remaining = y_frontier - (ball_y_min_og + ball->vy);
          //ball->asp.sp->y = db_to_int((double) (y_frontier + remaining));
          ball->asp.sp->y = db_to_int((double) (y_frontier + (1 - ratio) * (-ball->vy)));
          ball->asp.sp->x = db_to_int((double) (ball_x_min_og + ball->vx));
          ball->vy = -ball->vy;
          ball->rebounds--;
          return REBOUND_CY;
        }
      }
      break;

    case CY:
      if(ball_y_max - ball_y_max_og != 0){
        //if(ball_y_max < V_RES - OFFSET) y_frontier = ball_y_max_og + ball->vy - (ball_y_max % TILE_SIZE) + 32;
        if(ball_y_max < V_RES - OFFSET) y_frontier = frontier;
        else y_frontier = V_RES - OFFSET;
        ratio = (y_frontier - 16 - ball_y_max_og) / (ball_y_max - ball_y_max_og);
        if(ball->rebounds <= 0){
          ball->asp.sp->y = db_to_int((double) (y_frontier - ball->asp.sp->height));
          ball->asp.sp->x = ball_x_min_og + db_to_int(ball->vx * ratio);
          destroy_ball(tank, ball);
          return SOLID_C;
        }
        else{
          //remaining = y_frontier - (ball_y_max_og + ball->vy);
          //ball->asp.sp->y = db_to_int((double) (y_frontier - remaining - ball->asp.sp->height));
          ball->asp.sp->y = db_to_int((double) (y_frontier + (1 - ratio) * (-ball->vy) - ball->asp.sp->height));
          ball->asp.sp->x = db_to_int((double) (ball_x_min_og + ball->vx));
          ball->vy = -ball->vy;
          ball->rebounds--;
          return REBOUND_CY;
        }
      }
      break;

    case NO_C:
      break;
    
    default:
      break;
  }

  return NO_COLISION;
}
/**
 * @brief checks in which coordinate the colision occurs. If it doesn't returns NO_C
 * 
 */
Coordinate_t (check_colision_coord)(Ball_t * ball, Tile_t layout[][NCOLS], int * frontier){
  *frontier = 0;
  //uint16_t ball_y_max_og = ball->asp.sp->y + ball->asp.sp->height;
  uint16_t ball_x_max_og = ball->asp.sp->x + ball->asp.sp->width;
  //uint16_t ball_y_min_og = ball->asp.sp->y;
  uint16_t ball_x_min_og = ball->asp.sp->x;

  uint16_t ball_y_max = ball->asp.sp->y + ball->asp.sp->height + db_to_int(ball->vy);
  uint16_t ball_x_max = ball->asp.sp->x + ball->asp.sp->width + db_to_int(ball->vx);
  uint16_t ball_y_min = ball->asp.sp->y + db_to_int(ball->vy);
  uint16_t ball_x_min = ball->asp.sp->x + db_to_int(ball->vx);

  if(ball_x_min_og <= XOFFSET || ball_x_min <= XOFFSET) return NCX;
  if(ball_y_min <= YOFFSET) return NCY;

  uint16_t ball_y_max_tile = (ball_y_max - YOFFSET -16) / TILE_SIZE;
  uint16_t ball_x_max_tile = (ball_x_max - XOFFSET) / TILE_SIZE;
  uint16_t ball_y_min_tile = (ball_y_min - YOFFSET -16) / TILE_SIZE;
  uint16_t ball_x_min_tile = (ball_x_min - XOFFSET) / TILE_SIZE;

  //uint16_t ball_y_max_og_tile = (ball_y_max_og - YOFFSET -16) / TILE_SIZE;
  uint16_t ball_x_max_og_tile = (ball_x_max_og - XOFFSET) / TILE_SIZE;
  //uint16_t ball_y_min_og_tile = (ball_y_min_og - YOFFSET -16) / TILE_SIZE;
  uint16_t ball_x_min_og_tile = (ball_x_min_og - XOFFSET) / TILE_SIZE;

  if(ball_x_min_tile < 0 || ball_x_min_tile > 48 || ball_x_min_og_tile < 0 || ball_x_min_og_tile > 48) return NCX;

  bool colision_points[4] = {0, 0, 0, 0};

  if(layout[ball_y_min_tile][ball_x_min_tile] == WALL) colision_points[0] = 1;
  if(layout[ball_y_min_tile][ball_x_max_tile] == WALL) colision_points[1] = 1;
  if(layout[ball_y_max_tile][ball_x_min_tile] == WALL) colision_points[2] = 1;
  if(layout[ball_y_max_tile][ball_x_max_tile] == WALL) colision_points[3] = 1;

  if(ball_y_min <= YOFFSET || (colision_points[0] && colision_points[1])){
    *frontier = YOFFSET + TILE_SIZE * (ball_y_min_tile + 1);
    return NCY;
  } 
  if(ball_x_min <= XOFFSET || (colision_points[0] && colision_points[2])){
    *frontier = XOFFSET + TILE_SIZE * (ball_x_min_tile + 1);
    return NCX;
  } 
  if(ball_x_max >= H_RES - XOFFSET || (colision_points[1] && colision_points[3])){
    *frontier = XOFFSET + TILE_SIZE * (ball_x_max_tile);
    return CX;
  }
  if(ball_y_max >= V_RES - OFFSET || (colision_points[2] && colision_points[3])){
    *frontier = YOFFSET + TILE_SIZE * (ball_y_max_tile);
    return CY;
  }

  if(colision_points[0]){
    if(layout[ball_y_min_tile][ball_x_min_og_tile] == WALL){
      if (ball->vy < 0){
        *frontier = YOFFSET + TILE_SIZE * (ball_y_min_tile + 1);
        return NCY;
      }
      else{
        *frontier = YOFFSET + TILE_SIZE * (ball_y_max_tile);
        return CY;
      }
    }
    else{
      if (ball->vx < 0){
        *frontier = XOFFSET + TILE_SIZE * (ball_x_min_tile + 1);
        return NCX;
      }
      else{
        *frontier = XOFFSET + TILE_SIZE * (ball_x_max_tile);
        return CX;
      }
    }
  }

  if(colision_points[1]){
    if(layout[ball_y_min_tile][ball_x_max_og_tile] == WALL){
      if (ball->vy < 0){
        *frontier = YOFFSET + TILE_SIZE * (ball_y_min_tile + 1);
        return NCY;
      }
      else{
        *frontier = YOFFSET + TILE_SIZE * (ball_y_max_tile);
        return CY;
      }
    }
    else{
      if (ball->vx < 0){
        *frontier = XOFFSET + TILE_SIZE * (ball_x_min_tile + 1);
        return NCX;
      }
      else{
        *frontier = XOFFSET + TILE_SIZE * (ball_x_max_tile);
        return CX;
      }
    }
  }

  if(colision_points[2]){
    if(layout[ball_y_max_tile][ball_x_min_og_tile] == WALL){
      if (ball->vy < 0){
        *frontier = YOFFSET + TILE_SIZE * (ball_y_min_tile + 1);
        return NCY;
      }
      else{
        *frontier = YOFFSET + TILE_SIZE * (ball_y_max_tile);
        return CY;
      }
    }
    else{
      if (ball->vx < 0){
        *frontier = XOFFSET + TILE_SIZE * (ball_x_min_tile + 1);
        return NCX;
      }
      else{
        *frontier = XOFFSET + TILE_SIZE * (ball_x_max_tile);
        return CX;
      }
    }
  }

  if(colision_points[3]){
    if(layout[ball_y_max_tile][ball_x_max_og_tile] == WALL){
      if (ball->vy < 0){
        *frontier = YOFFSET + TILE_SIZE * (ball_y_min_tile + 1);
        return NCY;
      }
      else{
        *frontier = YOFFSET + TILE_SIZE * (ball_y_max_tile);
        return CY;
      }
    }
    else{
      if (ball->vx < 0){
        *frontier = XOFFSET + TILE_SIZE * (ball_x_min_tile + 1);
        return NCX;
      }
      else{
        *frontier = XOFFSET + TILE_SIZE * (ball_x_max_tile);
        return CX;
      }
    }
  }

  return NO_C;
}
/**
 * @brief true if the PlayerTank collides with something, false otherwise
 * 
 */
bool (check_colision)(Tank_t * PlayerTank, Tile_t layout[][NCOLS], Coordinate_t coord, int *correct, Tank_t * tanks[MAX_TANKS]){
  int speed = 0;
  int correction = 0;

  switch (coord){
    case CX:
      speed = PlayerTank->asp.sp->xspeed;
      break;
    case NCX:
      speed = PlayerTank->asp.sp->xspeed;
      break;
    case CY:
      speed = PlayerTank->asp.sp->yspeed;
      break;
    case NCY:
      speed = PlayerTank->asp.sp->yspeed;
      break;
    default:
      break;
  }

  bool colision = false;

  while(correction < speed){
    int mx = PlayerTank->asp.sp->x - XOFFSET;
    int my = PlayerTank->asp.sp->y - YOFFSET - 16;
    colision = false;

    switch (coord){ //Borders
    case CX:
      mx += PlayerTank->asp.sp->xspeed - correction;
      if(PlayerTank->asp.sp->x + PlayerTank->asp.sp->xspeed < XOFFSET || PlayerTank->asp.sp->y + PlayerTank->asp.sp->height > (V_RES - XOFFSET) || PlayerTank->asp.sp->y < YOFFSET || PlayerTank->asp.sp->x + PlayerTank->asp.sp->width + PlayerTank->asp.sp->xspeed > (H_RES - XOFFSET)) colision = true;

      for(int i = 0; MAX_TANKS > i; i++){
        if(tanks[i] != NULL && tanks[i]->state == ALIVE){
          if(check_tank_tank_colision(PlayerTank, tanks[i], 0, -PlayerTank->asp.sp->yspeed)){
            while(check_tank_tank_colision(PlayerTank, tanks[i], -correction, -PlayerTank->asp.sp->yspeed) && abs(correction) < abs(speed)){
              correction++;
            }
            if(abs(correction) >= abs(speed)) return 1;
            break;
          }
        }
      }
      break;
      break;
    
    case NCX:
      mx -= PlayerTank->asp.sp->xspeed + correction;
      if(PlayerTank->asp.sp->x - PlayerTank->asp.sp->xspeed < XOFFSET || PlayerTank->asp.sp->y + PlayerTank->asp.sp->height > (V_RES - XOFFSET) ||PlayerTank->asp.sp->y < YOFFSET || PlayerTank->asp.sp->x + PlayerTank->asp.sp->width - PlayerTank->asp.sp->xspeed > (H_RES - XOFFSET))colision = true;

      for(int i = 0; MAX_TANKS > i; i++){
        if(tanks[i] != NULL && tanks[i]->state == ALIVE){
          if(check_tank_tank_colision(PlayerTank, tanks[i], - 2 * PlayerTank->asp.sp->xspeed, -PlayerTank->asp.sp->yspeed)){
            while(check_tank_tank_colision(PlayerTank, tanks[i], - 2 * PlayerTank->asp.sp->xspeed + correction, -PlayerTank->asp.sp->yspeed) && abs(correction) < abs(speed)){
              correction++;
            }
            if(abs(correction) >= abs(speed)) return 1;
            break;
          }
        }
      }
      break;

    case CY:
      my += PlayerTank->asp.sp->yspeed - correction;
      if(PlayerTank->asp.sp->x < XOFFSET || PlayerTank->asp.sp->y + PlayerTank->asp.sp->height + PlayerTank->asp.sp->yspeed > (V_RES - XOFFSET) || PlayerTank->asp.sp->y + PlayerTank->asp.sp->yspeed < YOFFSET || PlayerTank->asp.sp->x + PlayerTank->asp.sp->width > (H_RES - XOFFSET))colision = true;

      for(int i = 0; MAX_TANKS > i; i++){
        if(tanks[i] != NULL && tanks[i]->state == ALIVE){
          if(check_tank_tank_colision(PlayerTank, tanks[i], -PlayerTank->asp.sp->xspeed, 0)){
            while(check_tank_tank_colision(PlayerTank, tanks[i], -PlayerTank->asp.sp->xspeed, -correction) && abs(correction) < abs(speed)){
              correction++;
            }
            if(abs(correction) >= abs(speed)) return 1;
            break;
          }
        }
      }
      break;
      break;

    case NCY:
      my -= PlayerTank->asp.sp->yspeed + correction;
      if(PlayerTank->asp.sp->x < XOFFSET || PlayerTank->asp.sp->y + PlayerTank->asp.sp->height - PlayerTank->asp.sp->yspeed > (V_RES - XOFFSET) || PlayerTank->asp.sp->y - PlayerTank->asp.sp->yspeed < YOFFSET || PlayerTank->asp.sp->x + PlayerTank->asp.sp->width > (H_RES - XOFFSET)) colision = true;

      for(int i = 0; MAX_TANKS > i; i++){
        if(tanks[i] != NULL && tanks[i]->state == ALIVE){
          if(check_tank_tank_colision(PlayerTank, tanks[i], -PlayerTank->asp.sp->xspeed, - 2 * PlayerTank->asp.sp->yspeed)){
            while(check_tank_tank_colision(PlayerTank, tanks[i], -PlayerTank->asp.sp->xspeed, - 2 * PlayerTank->asp.sp->yspeed + correction) && abs(correction) < abs(speed)){
              correction++;
            }
            if(abs(correction) >= abs(speed)) return 1;
            break;
          }
        }
      }
      break;

    default:
      if(PlayerTank->asp.sp->x < XOFFSET || PlayerTank->asp.sp->y + PlayerTank->asp.sp->height > (V_RES - XOFFSET) || PlayerTank->asp.sp->y < YOFFSET || PlayerTank->asp.sp->x + PlayerTank->asp.sp->width > (H_RES - XOFFSET)) colision = true;
      break;
    }

    /*
    for(int i = 0; MAX_TANKS > i; i++){
      if(tanks[i] != NULL && tanks[i]->state == ALIVE){
        xcorrection = 0;
        ycorrection = 0;
        if(check_tank_tank_colision(PlayerTank, tanks[i], xcorrection, ycorrection)){
          switch (coord){
            case CX:
              xcorrection--;
              break;
            case NCX:
              xcorrection++;
              break;
            case CY:
              ycorrection--;
              break;
            case NCY:
              xcorrection++;
              break;
            default:
              break;
          }
          colision = true;
          break;
        } 
      }
    }*/

    int r_up[2], r_d[2], l_d[2];
    int l_up[2] = {mx / TILE_SIZE, my / TILE_SIZE};
    r_up[0] = (mx + PlayerTank->asp.sp->width) / TILE_SIZE;
    r_up[1] = my / TILE_SIZE;
    r_d[0] = (mx + PlayerTank->asp.sp->width) / TILE_SIZE;
    r_d[1] = (my + PlayerTank->asp.sp->height) / TILE_SIZE;
    l_d[0] = mx / TILE_SIZE;
    l_d[1] = (my + PlayerTank->asp.sp->height) / TILE_SIZE;

    if(layout[l_up[1]][l_up[0]] == WALL ) colision = true;
    if(layout[l_d[1]][l_d[0]] == WALL ) colision = true;
    if(layout[r_up[1]][r_up[0]] == WALL ) colision = true;
    if(layout[r_d[1]][r_d[0]] == WALL ) colision = true;
    
    if(!colision){
      *correct = correction;
      return 0;
    }

    correction++;

  }

  *correct = 0;
  return 1;
}
/**
 * @brief checks if a specified tank collides with something and return the type of colision
 * 
 */
Colision_t (check_tank_colision)(Tank_t * tank, Tile_t layout[][NCOLS], Tank_t * tanks[MAX_TANKS], int xcorrection, int ycorrection){

  uint16_t x = tank->asp.sp->x + tank->asp.sp->xspeed + xcorrection;
  uint16_t y = tank->asp.sp->y + tank->asp.sp->yspeed + ycorrection;

  if(x < XOFFSET || y + tank->asp.sp->height > (V_RES - OFFSET) || y < YOFFSET || x + tank->asp.sp->width > (H_RES - XOFFSET)) return SOLID_C;

  int mx = tank->asp.sp->x - XOFFSET + xcorrection;
  int my = tank->asp.sp->y - YOFFSET - 16 + ycorrection;

  for(int i = 0; MAX_TANKS > i; i++){
    if(tanks[i] != NULL && tanks[i]->state == ALIVE){
      if(check_tank_tank_colision(tank, tanks[i], xcorrection, ycorrection)) return TANK_C;
    }
  }

  int r_up[2], r_d[2], l_d[2];
  int l_up[2] = {mx / TILE_SIZE, my / TILE_SIZE};
  r_up[0] = (mx + tank->asp.sp->width) / TILE_SIZE;
  r_up[1] = my / TILE_SIZE;
  r_d[0] = (mx + tank->asp.sp->width) / TILE_SIZE;
  r_d[1] = (my + tank->asp.sp->height) / TILE_SIZE;
  l_d[0] = mx / TILE_SIZE;
  l_d[1] = (my + tank->asp.sp->height) / TILE_SIZE;

  if(layout[l_up[1]][l_up[0]] == WALL ) return SOLID_C;
  if(layout[l_d[1]][l_d[0]] == WALL ) return SOLID_C;
  if(layout[r_up[1]][r_up[0]] == WALL ) return SOLID_C;
  if(layout[r_d[1]][r_d[0]] == WALL ) return SOLID_C;
  
  return NO_COLISION;
}
/**
 * @brief handles the colision between two moving tanks. True if there is a colision
 * 
 */
bool (check_tank_tank_colision)(Tank_t * tank1, Tank_t * tank2, int xcorrection, int ycorrection){
  if(tank1->asp.sp->x == tank2->asp.sp->x && tank1->asp.sp->y == tank2->asp.sp->y) return false;

  uint16_t t1_x_min = tank1->asp.sp->x + tank1->asp.sp->xspeed + xcorrection;
  uint16_t t2_x_min = tank2->asp.sp->x;

  uint16_t t1_x_max = tank1->asp.sp->x + tank1->asp.sp->xspeed + tank1->asp.sp->width + xcorrection;
  uint16_t t2_x_max = tank2->asp.sp->x + tank2->asp.sp->width;

  uint16_t t1_y_min = tank1->asp.sp->y + tank1->asp.sp->yspeed + ycorrection;
  uint16_t t2_y_min = tank2->asp.sp->y;

  uint16_t t1_y_max = tank1->asp.sp->y + tank1->asp.sp->yspeed + tank1->asp.sp->height + ycorrection;
  uint16_t t2_y_max = tank2->asp.sp->y + tank2->asp.sp->height;


  if( ((t1_y_min >= t2_y_min && t1_y_min <= t2_y_max) || (t1_y_max >= t2_y_min && t1_y_max <= t2_y_max)) && ((t1_x_min >= t2_x_min && t1_x_min <= t2_x_max) || (t1_x_max >= t2_x_min && t1_x_max <= t2_x_max)) ) return true;

  return false;
}
/**
 * @brief checks if the mouse is within the borders of the screen
 * 
 */
bool (valid_mouse_mov)(AnimSprite * asp, uint16_t deltax, uint16_t deltay){
  int aim_x = asp->sp->x;
  int aim_y = asp->sp->y;
  uint8_t new_deltax, new_deltay, rev_deltax, rev_deltay;

  util_get_LSB(deltax, &new_deltax);
  util_get_LSB(deltay, &new_deltay);

  rev_deltax = new_deltax; 
  rev_deltay = new_deltay;
  abs_byte(&rev_deltax);
  abs_byte(&rev_deltay);

  bool invert_x = (deltax >> 15);
  bool invert_y = (deltay >> 15);
  
  bool valid = true;

  
  if(invert_x) aim_x -= rev_deltax;
  else aim_x += new_deltax;

  if(invert_y) aim_y += rev_deltay;
  else aim_y -= new_deltay;


  if(aim_x <= 0){
    asp->sp->x = 0;
    asp->sp->y = aim_y;
    valid = false;
  }

  if(aim_y < 0){
    asp->sp->y = 0;
    if(valid)asp->sp->x = aim_x;
    valid = false;
  }

  
  if((aim_x + asp->sp->width) > H_RES){
    asp->sp->x = H_RES - asp->sp->width;
    if(valid)asp->sp->y = aim_y;
    valid = false;
  }

  if((aim_y + asp->sp->height) > V_RES - 4){
    asp->sp->y = V_RES - asp->sp->height;
    if(valid)asp->sp->x = aim_x;
    valid = false;
  }

  return valid;
}
/**
 * @brief alters a ball's x and y to their initial position based on their tank's position
 * 
 */
void (ball_init_position)(Tank_t * tank, Ball_t * ball){
  double x_centre_tank = tank->asp.sp->x + (tank->asp.sp->width / 2.0);
  double y_centre_tank = tank->asp.sp->y + (tank->asp.sp->height / 2.0);
  double b = 0;
  
  uint8_t coordinate_to_find = 0; // 0->none , 1->x , 2->y
  switch (tank->aim_quadrant){
    case XP:
      ball->asp.sp->x = tank->asp.sp->x + tank->asp.sp->width + 1;
      //ball->asp.sp->y = y_centre_tank + tank->asp.sp->width / (2 * tan(ball->angle));
      coordinate_to_find = 2;
      break;

    case YP_XP:
      ball->asp.sp->y = tank->asp.sp->y - ball->asp.sp->height - 1;
      //ball->asp.sp->x = x_centre_tank + tank->asp.sp->height / (2 * tan(ball->angle));
      coordinate_to_find = 1;
      break;

    case VP:
      ball->asp.sp->x = tank->asp.sp->x + (tank->asp.sp->width / 2) - (ball->asp.sp->width / 2);
      ball->asp.sp->y = tank->asp.sp->y - ball->asp.sp->height - 1;
      coordinate_to_find = 0;
      break;

    case YP_XN:
      ball->asp.sp->y = tank->asp.sp->y - ball->asp.sp->height - 1;
      //ball->asp.sp->x = x_centre_tank + tank->asp.sp->height / (2 * tan(ball->angle));
      coordinate_to_find = 1;
      break;

    case XN:
      ball->asp.sp->x = tank->asp.sp->x - ball->asp.sp->width - 1;
      //ball->asp.sp->y = y_centre_tank + tank->asp.sp->width / (2 * tan(ball->angle));
      coordinate_to_find = 2;
      break;

    case YN_XN:
      ball->asp.sp->y = tank->asp.sp->y + tank->asp.sp->height + 1;
      //ball->asp.sp->x = x_centre_tank + tank->asp.sp->height / (2 * -tan(ball->angle));
      coordinate_to_find = 1;
      break;

    case VN:
      ball->asp.sp->x = tank->asp.sp->x + (tank->asp.sp->width / 2) - (ball->asp.sp->width / 2);
      ball->asp.sp->y = tank->asp.sp->y + tank->asp.sp->height + 1;
      coordinate_to_find = 0;
      break;

    case YN_XP:
      ball->asp.sp->y = tank->asp.sp->y + tank->asp.sp->height + 1;
      //ball->asp.sp->x = x_centre_tank + tank->asp.sp->height / (2 * -tan(ball->angle));
      coordinate_to_find = 1;
      break;

    default:
      break;

  }
  
  double x_centre_ball = ball->asp.sp->x + (ball->asp.sp->width / 2.0);
  double y_centre_ball = ball->asp.sp->y + (ball->asp.sp->height / 2.0);

  if(coordinate_to_find == 1){
    //b = y_centre_tank - x_centre_tank * tank->slope;
    b = y_centre_tank - x_centre_tank * -tank->slope;
    //x_centre_ball = (y_centre_ball - b) / tank->slope;
    x_centre_ball = (y_centre_ball - b) / -tank->slope;
    ball->asp.sp->x = db_to_int( x_centre_ball - (ball->asp.sp->width / 2.0) );
  }
  else if(coordinate_to_find == 2){
    //b = y_centre_tank - x_centre_tank * tank->slope;
    b = y_centre_tank - x_centre_tank * -tank->slope;
    //y_centre_ball = tank->slope * x_centre_ball + b;
    y_centre_ball = -tank->slope * x_centre_ball + b;
    ball->asp.sp->y = db_to_int( y_centre_ball - (ball->asp.sp->height / 2.0) );
  }

}
/**
 * @brief simulates a shot by creating a new ball and putting it in it's initial position
 * 
 */
void (shoot)(Tank_t * tank){
  Ball_t * ball = create_newball(tank, true);

  //Ball_t * ball = create_newball(*create_animSprite_spec(0, 0, 0, 0, ANIMSPEED, 2, ball_maps[0], ball_maps[1]), tank, true);


  ball_init_position(tank, ball);

  tank->cooldown_count = 0;

  if(tank->type == TPLAYER){
    tank->aim->status = SHOOTING;
    tank->aim->asp.cur_fig = 2;
    tank->aim->asp.cur_aspeed = 0;
  }

}


//Loop
/**
 * @brief the centre of the game. Initiates the level and menu arrays and calls game_init and main_menu
 * 
 */
void (game_loop)(){
  Level_t * Game_Levels[MAX_LEVELS] = {NULL};
  Menu_t * Game_Menus[MAX_MENUS] = {NULL};

  Aim_t P_Aim = *create_newaim(*create_animSprite_spec(NCOLS * TILE_SIZE / 2, NROWS * TILE_SIZE / 2, AIMSPEED, AIMSPEED, AIM_ANIMSPEED, NSPRITES_AIM, aim_maps[0], aim_maps[1], aim_maps[2], aim_maps[3], aim_maps[4]));

  Player_t Player = *create_newplayer(create_new_PlayerTank(*create_animSprite_spec(NCOLS * TILE_SIZE / 2, NROWS * TILE_SIZE / 2, TANKSPEED, TANKSPEED, ANIMSPEED, NSPRITES_TANK, tank_maps[0], tank_maps[1], tank_maps[2], tank_maps[3], tank_maps[4], tank_maps[5], tank_maps[6], tank_maps[7], tank_maps[8], tank_maps[9], tank_maps[10], tank_maps[11], tank_maps[12], tank_maps[13], tank_maps[14], tank_maps[15], tank_maps[16], tank_maps[17], tank_maps[18]), &P_Aim, P_COOLDOWN_FRAMES, P_MAX_BALLS, P_BALL_REB, P_BALL_STR, BALL_VEL, P_IFRAMES, P_HITS), LIVES, "DarKKoala");

  game_init(Game_Levels, Game_Menus);
  
  //play_level(Game_Menus, Game_Levels, 4, &Player, *(Player.PlayerTank), *(Player.PlayerTank->aim));

  main_menu(Game_Menus, Game_Levels, &Player);

}

/**
 * @brief Initiates the video mode of the graphics card (mode 0x105) and creates the levels and menus of the game
 * 
 */
void (game_init)(Level_t * Game_Levels[MAX_LEVELS], Menu_t * Game_Menus[MAX_MENUS]){
  vg_init(VMODE);

  vbe_mode_info_t mode_info;
  vg_get_mode_info(VMODE, & mode_info);
  //uint16_t xres = mode_info.XResolution;
  //uint16_t yres = mode_info.YResolution;
  screen_saver = (char*) malloc(H_RES * V_RES * (BPP / 8));

  create_levels(Game_Levels);
  create_menus(Game_Menus);
}
/**
 * @brief receives the input from the menus and changes some bools as a result of that
 * 
 */
void (menu_select)(int * menu_sel, Menu_t * Game_Menus[MAX_MENUS], Player_t * Player, bool * lets_play, bool * quit, bool * transparent, int * level_number){
  int menu_option = -1;
  switch (*menu_sel){
    case 0:
      menu_option = menu_loop(Game_Menus[1], Player, transparent, true, true);
      if(menu_option == 4) *quit = true;
      //draw_rtc(RTC_X, RTC_Y, false, rtc);
      break;
    case 1:
      menu_option = menu_loop(Game_Menus[2], Player, transparent, true, false);
      char * str_number = "";
      sprintf(str_number, "%d", *level_number);
      draw_string(LN_X, LN_Y, false, str_number);
      update_screen();
      if(((menu_option == 1 && Player->PlayerTank->aim->asp.cur_fig == 4) || movement[3]) && *level_number < MAX_LEVELS - 1){
        counter = 0;
        (*level_number)++;
      }
      if(((menu_option == 2 && Player->PlayerTank->aim->asp.cur_fig == 4) || movement[1]) && *level_number > 0){
        counter = 0;
        (*level_number)--;
      }
      if(menu_option == 3) *lets_play = true;
      break;
    default:
      menu_option = menu_loop(Game_Menus[1], Player, transparent, true, true);
      //draw_rtc(RTC_X, RTC_Y, false, rtc);
      break;
  }
  
  if(menu_option != -1 && menu_option < 4 && *menu_sel == 0) *menu_sel = menu_option;
}
/**
 * @brief subscribes what's needed for the menu navigation. Has the interrupt loop. Ends with the closing of the game or with the initiation of a level
 * 
 */
void (main_menu)(Menu_t * Game_Menus[MAX_MENUS], Level_t * Game_Levels[MAX_LEVELS], Player_t * Player){
  reset_mouse_info();
  
  int ipc_status;
  message msg;
  
  uint8_t kbd_bit = 0;
  uint8_t timer_bit = 1;
  uint8_t mouse_bit = 2;
  uint8_t rtc_bit = 3;

  mouse_enable();

  timer_subscribe_int(& timer_bit);
  kbd_subscribe_int(& kbd_bit);
  mouse_subscribe_int(& mouse_bit);
  rtc_subscribe_int(& rtc_bit);

  rtc_enable_update_int();

  uint64_t irq_timer = BIT(timer_bit);
  uint64_t irq_kbd = BIT(kbd_bit);
  uint64_t irq_mouse = BIT(mouse_bit);
  uint64_t irq_rtc = BIT(rtc_bit);

  uint16_t timer_freq = FRAME_RATE_MAX;
  timer_set_frequency(0, timer_freq);
  int r;

  counter = 0;
  scancode = 0;
  mousecounter = 0;
  

  
  int level_number = 0;
  int menu_sel = 0;
  bool quit = false;
  bool lets_play = false;
  bool transparent = false;
  
  while(scancode != ESC_BREAK && !lets_play && !quit){
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != OK) {
      printf("driver_receive failed with: %d", r);
      continue;
    }
    if (is_ipc_notify(ipc_status)) {
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:
        if (msg.m_notify.interrupts & irq_rtc) {
            rtc_ih();
            rtc_get_time(&rtc);
          }
          if (msg.m_notify.interrupts & irq_kbd) {
            kbc_ih();
            menu_ih(Game_Menus, KBD_INT, Game_Levels, Player, &menu_sel, &quit, &lets_play, &transparent, &level_number);
          }
          if (msg.m_notify.interrupts & irq_mouse){
            mouse_ih();
            assemble_bytes(&mousecounter, packet);
            if (mousecounter == 3){
              build_packet_ret(&mousecounter, packet, &pck);
              mousecounter = 0;
              menu_ih(Game_Menus, MOUSE_INT, Game_Levels, Player, &menu_sel, &quit, &lets_play, &transparent, &level_number);
            }
          }
          if (msg.m_notify.interrupts & irq_timer){
            timer_int_handler();
            menu_ih(Game_Menus, TIMER_INT, Game_Levels, Player, &menu_sel, &quit, &lets_play, &transparent, &level_number);
          }

          break;
        default:
          break;
      }
    }
  }
  if(scancode == ESC_BREAK) quit = true;

  if(quit){
    vg_exit();
    disable_mouse_data();
    mouse_unsubscribe_int();
    timer_unsubscribe_int();
    kbd_unsubscribe_int();
    rtc_unsubscribe_int();
    return;
  }
  if(lets_play){
    if(Game_Levels[level_number] != NULL) play_level(Game_Menus, Game_Levels, level_number, Player, *(Player->PlayerTank), *(Player->PlayerTank->aim));
    else play_level(Game_Menus, Game_Levels, 4, Player, *(Player->PlayerTank), *(Player->PlayerTank->aim));
  }
}
/**
 * @brief handles the menu interrupts. First sorts them by type and then uses the information provided by the devices
 * 
 */
void (menu_ih)(Menu_t * Game_Menus[MAX_MENUS], Interrupt_t int_type, Level_t * Game_Levels[MAX_LEVELS], Player_t * Player, int * menu_sel, bool * quit, bool * lets_play, bool * transparent, int * level_number){
   switch(int_type){
    case MOUSE_INT:
      check_mouse_b();
      update_mouse(Player->PlayerTank);
      break;

    case KBD_INT:
      check_kbd();
      break;

    case TIMER_INT:
      menu_select(menu_sel, Game_Menus, Player, lets_play, quit, transparent, level_number);
      break;

    default:
      break;
  }
}
/**
 * @brief constitutes the main function of the game. Subscribes all the devices. Has the interrupt loop and calls game_ih for each type of interrupt
 * 
 */
void (play_level)(Menu_t * Game_Menus[MAX_MENUS], Level_t * Game_Levels[MAX_LEVELS], int level_number, Player_t * Player, Tank_t OG_PlayerTank, Aim_t OG_Aim){
  if(Game_Levels[level_number] == NULL) return;
  
  //strcpy(mc, Game_Menus[0]->boxes[1].text);

  sleep(1);

  Box_t menu_box = *(create_newbox(MENU_BOX_X, MENU_BOX_Y, MENU_BOX_COLOR, MENU_BOX_FCOLOR, true, "Menu"));
  Tank_t *level_tanks[MAX_TANKS] = {NULL};
  Tile_t level_layout[NROWS][NCOLS]; // 3 by 2 level map
  layout_init(level_layout, Game_Levels[level_number]->layout);
  Clock_t clock = Game_Levels[level_number]->clock;
  LevelType_t level_type =  Game_Levels[level_number]->type;

  reset_mouse_info();
  
  int ipc_status;
  message msg;
  
  uint8_t kbd_bit = 0;
  uint8_t timer_bit = 1;
  uint8_t mouse_bit = 2;
  uint8_t rtc_bit = 3;

  mouse_enable();

  timer_subscribe_int(& timer_bit);
  kbd_subscribe_int(& kbd_bit);
  mouse_subscribe_int(& mouse_bit);
  rtc_subscribe_int(& rtc_bit);

  rtc_enable_update_int();

  uint64_t irq_timer = BIT(timer_bit);
  uint64_t irq_kbd = BIT(kbd_bit);
  uint64_t irq_mouse = BIT(mouse_bit);
  uint64_t irq_rtc = BIT(rtc_bit);

  uint16_t timer_freq = FRAME_RATE_MAX;
  timer_set_frequency(0, timer_freq);
  int r;

  counter = 0;
  scancode = 0;
  mousecounter = 0;

  add_tank(level_tanks, Player->PlayerTank);

  draw_level(level_layout, level_tanks, Player, clock);
  update_screen();

  bool retry = false;
  bool level_beat = false;
  bool ingame_menu = false;
  bool game_over = false;
  bool quit = false;

  while(scancode != ESC_BREAK && !retry && !game_over && !level_beat){
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != OK) {
      printf("driver_receive failed with: %d", r);
      continue;
    }
    if (is_ipc_notify(ipc_status)) {
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:
          if (msg.m_notify.interrupts & irq_rtc) {
            rtc_ih();
            rtc_get_time(&rtc);
          }
          if (msg.m_notify.interrupts & irq_kbd) {
            kbc_ih();
            game_ih(Game_Menus, KBD_INT, level_layout, level_tanks, Player, &retry, &game_over, menu_box, &ingame_menu, &level_beat, clock, level_type);
          }
          if (msg.m_notify.interrupts & irq_mouse){
            mouse_ih();
            assemble_bytes(&mousecounter, packet);
            if (mousecounter == 3){
              build_packet_ret(&mousecounter, packet, &pck);
              mousecounter = 0;
              game_ih(Game_Menus, MOUSE_INT, level_layout, level_tanks, Player, &retry, &game_over, menu_box, &ingame_menu, &level_beat, clock, level_type);
            }
          }
          if (msg.m_notify.interrupts & irq_timer){
            timer_int_handler();
            if(counter > 0 && counter % 60 == 0) update_clock(&clock);
            game_ih(Game_Menus, TIMER_INT, level_layout, level_tanks, Player, &retry, &game_over, menu_box, &ingame_menu, &level_beat, clock, level_type);
          }

          break;
        default:
          break;
      }
    }
  }

  if(scancode == ESC_BREAK) quit = true;

  if(quit){
    vg_exit();
    disable_mouse_data();
    mouse_unsubscribe_int();
    timer_unsubscribe_int();
    kbd_unsubscribe_int();
    rtc_unsubscribe_int();
    return;
  }

  *(OG_PlayerTank.aim) = OG_Aim;
  Player->PlayerTank = &OG_PlayerTank;

  if(level_beat){

    sleep(1);
    main_menu(Game_Menus, Game_Levels, Player);
  }

  if(retry){
    for(int i = 0; Player->PlayerTank->max_balls > i; i++){
      Player->PlayerTank->balls[i].state = DESTROYED;
    }
    play_level(Game_Menus, Game_Levels, level_number, Player, OG_PlayerTank, OG_Aim);
  } 

  if(game_over) main_menu(Game_Menus, Game_Levels, Player);

  
  main_menu(Game_Menus, Game_Levels, Player);

  
}
/**
 * @brief returns true if the level is passed, false otherwise
 * 
 */
bool (check_level_passed) (LevelType_t level_type, Player_t * Player, Tank_t * tanks[MAX_TANKS], Clock_t clock, Tile_t layout[][NCOLS]){
  switch(level_type){
    case DEATHMATCH:
      for(int i = 0; MAX_TANKS > i; i++){
        if(tanks[i] != NULL && tanks[i]->type != TPLAYER && tanks[i]->state != DESTROYED) return false;
      }
      return true;
      break;

    case SURVIVAL:
      if(clock.minutes == 0 && clock.seconds == 0) return true;
      else return false;
      break;
    
    case GOAL:
      break; // TO DO
    
    default:
    break;
  }
  return false;
}
/**
 * @brief handles in-level interrupts. First sorts them by tyoe and then uses the information provided by the devices
 * 
 */
void (game_ih)(Menu_t * Game_Menus[MAX_MENUS], Interrupt_t int_type, Tile_t layout[][NCOLS], Tank_t * tanks[MAX_TANKS], Player_t * Player, bool * retry, bool * game_over, Box_t menu_box, bool * ingame_menu, bool * level_beat, Clock_t clock, LevelType_t level_type){
  int menu_option = -1;
  switch(int_type){
    case TIMER_INT:
      if(!(*ingame_menu))frame_loop(Game_Menus, layout, tanks, Player, retry, game_over, menu_box, ingame_menu, level_beat, clock, level_type);
      else{
        bool transp = true;
        menu_option = menu_loop(Game_Menus[0], Player, &transp, false, true);
      }

      if(menu_option != -1) *ingame_menu = false;
      if(menu_option == 2) *game_over = true;

      break;
    case KBD_INT:
      if(!(*ingame_menu)) check_kbd();
      break;
    case MOUSE_INT:
      check_mouse_b();
      update_mouse(Player->PlayerTank);
      if(!(*ingame_menu)) update_aim(layout, Player->PlayerTank);
      break;
    default:
      break;
  }
}

//Game Handlers
/**
 * @brief updates the movement array depending on the information of the kbd_ih for the letters WASD
 * 
 */
void (check_kbd)(){
  switch(scancode){
    case W_MAKE:
      if(!movement[0]) movement[0] = 1;
      break;

    case W_BREAK:
      if(movement[0]) movement[0] = 0;
      break;

    case A_MAKE:
      if(!movement[1]) movement[1] = 1;
      break;

    case A_BREAK:
      if(movement[1]) movement[1] = 0;
      break;

    case S_MAKE:
      if(!movement[2]) movement[2] = 1;
      break;

    case S_BREAK:
      if(movement[2]) movement[2] = 0;
      break;

    case D_MAKE:
      if(!movement[3]) movement[3] = 1;
      break;

    case D_BREAK:
      if(movement[3]) movement[3] = 0;
      break;

    default:
      break;
  }
}
/**
 * @brief updates the mouseinfo array, when the buttons are pressed and when they are released
 * 
 */
void (check_mouse_b)(){

  mouseinfo[3] = 0;
  mouseinfo[4] = 0;
  mouseinfo[5] = 0;

  if(mouseinfo[0] && !pck.lb) mouseinfo[3] = 1;
  if(mouseinfo[1] && !pck.mb) mouseinfo[4] = 1;
  if(mouseinfo[2] && !pck.rb) mouseinfo[5] = 1;

  if(pck.lb) mouseinfo[0] = 1;
  else mouseinfo[0] = 0;

  if(pck.mb) mouseinfo[1] = 1;
  else mouseinfo[1] = 0;

  if(pck.rb) mouseinfo[2] = 1;
  else mouseinfo[2] = 0;
  
}
/**
 * @brief updates the mouse position with the delta_x and delta_y from the packets
 * 
 */
void (update_mouse) (Tank_t * tank){
  if(valid_mouse_mov(& tank->aim->asp, pck.delta_x, pck.delta_y)){
    if((pck.delta_x >> 15) == 1) tank->aim->asp.sp->x -= pck.delta_x;
    else tank->aim->asp.sp->x += pck.delta_x;

    if((pck.delta_y >> 15) == 1) tank->aim->asp.sp->y += pck.delta_y;
    else tank->aim->asp.sp->y -= pck.delta_y;
  }
}
/**
 * @brief updates aim position and status and also the angle, aim_quadrant and slope of the respective tank as a result
 * 
 */
void (update_aim) (Tile_t layout[][NCOLS], Tank_t * tank){
  if(tank->aim->asp.sp->y < RESERVED_Y){
    tank->aim->ingame = false;
    tank->aim->asp.cur_fig = 3;
  }
  else tank->aim->ingame = true;

  if(tank->aim->ingame){
    double centre_y_tank = 0;
    double centre_x_tank = 0;
    get_tank_centre(tank, &centre_x_tank, &centre_y_tank);

    double centre_y_aim = tank->aim->asp.sp->y + tank->aim->asp.sp->height / 2;
    double centre_x_aim = tank->aim->asp.sp->x + tank->aim->asp.sp->width / 2;

    //double centre_dif_y = centre_y_tank - centre_y_aim;
    //double centre_dif_x = centre_x_aim - centre_x_tank;

    if(!check_tank_aim_colision(tank)){

      set_angle_slope(centre_x_tank, centre_y_tank, centre_x_aim, centre_y_aim, &tank->angle, &tank->slope, &tank->aim_quadrant, &tank->aim_right, &tank->aim_up);

      if(tank->state == ALIVE) update_tank_sprite(tank);

      if(mouseinfo[3] && tank->cooldown_count == tank->cooldown_frames && tank->ball_count < tank->max_balls && tank->asp.cur_fig != 18 && check_shooting_position(layout, tank)){
        shoot(tank);
      }
    }
  }
  
}
/**
 * @brief cicle repeated 60 times per second, updating every position and drawing the entire screen
 * 
 */
void (frame_loop)(Menu_t * Game_Menus[MAX_MENUS], Tile_t layout[][NCOLS], Tank_t * tanks[MAX_TANKS], Player_t * Player, bool * retry, bool * game_over, Box_t menu_box, bool * ingame_menu, bool * level_beat, Clock_t clock, LevelType_t level_type){
  if(Player->PlayerTank->state == DESTROYED){
    Player->lives--;
    if(Player->lives >= 0) *retry = true;
    else *game_over = true;
  }  

  if(Player->PlayerTank->state == ALIVE){
    if(check_level_passed(level_type, Player, tanks, clock, layout)) *level_beat = true;

    update_tanks_shooting(layout, tanks, Player);
    update_tanks_pos(layout, tanks, Player);
    update_balls_positions(tanks, layout);
    update_player_pos(layout, Player->PlayerTank, tanks);
    update_aim_status(layout, Player->PlayerTank);

    if(mouse_over_box(Player, menu_box) && mouseinfo[3]){
      save_screen(screen_saver);
      *ingame_menu = true;
    }
  }
  else{
    Player->PlayerTank->aim->status = NORMAL_AIM;
    Player->PlayerTank->aim->asp.cur_fig = 0;
    
    if(Player->PlayerTank->state == BOOM){

      if(Player->PlayerTank->asp.cur_fig < 16) Player->PlayerTank->asp.cur_fig = 16;
      if(Player->PlayerTank->asp.cur_aspeed > 0) Player->PlayerTank->asp.cur_aspeed--;
      if(Player->PlayerTank->asp.cur_fig == 16 && Player->PlayerTank->asp.cur_aspeed <= 0){
        Player->PlayerTank->asp.cur_aspeed = Player->PlayerTank->asp.aspeed;
        Player->PlayerTank->asp.cur_fig = 17;
      }
      if(Player->PlayerTank->asp.cur_fig == 17 && Player->PlayerTank->asp.cur_aspeed <= 0){
        Player->PlayerTank->asp.cur_aspeed = Player->PlayerTank->asp.aspeed;
        Player->PlayerTank->state = DESTROYED;
      }
    }
  }
  
  draw_screen(layout, tanks, Player, menu_box, clock);
}
/**
 * @brief updates the aim shooting status so it can't shoot if the tank has exceeded the maximum number of balls or is in cooldown or even next to a wall
 * 
 */
void (update_aim_status) (Tile_t layout[][NCOLS], Tank_t * PlayerTank){
  if(PlayerTank->aim->asp.sp->y < RESERVED_Y){
    PlayerTank->aim->ingame = false;
    if(mouseinfo[0]) PlayerTank->aim->asp.cur_fig = 4;
    else PlayerTank->aim->asp.cur_fig = 3;
  }
  else PlayerTank->aim->ingame = true;

  if(PlayerTank->aim->ingame){
    if(PlayerTank->aim->status == SHOOTING){
      PlayerTank->aim->asp.cur_aspeed++;
      if(PlayerTank->aim->asp.cur_aspeed >= PlayerTank->aim->asp.aspeed){
        PlayerTank->aim->status = NORMAL_AIM;
        PlayerTank->aim->asp.cur_fig = 0;
      }
    }
    else{
      PlayerTank->aim->status = NORMAL_AIM;
      PlayerTank->aim->asp.cur_fig = 0;

      if((PlayerTank->cooldown_count < PlayerTank->cooldown_frames || PlayerTank->ball_count >= PlayerTank->max_balls || PlayerTank->asp.cur_fig == 18) && PlayerTank->aim->status == NORMAL_AIM){
        PlayerTank->aim->status = NO_FIRE;
        PlayerTank->aim->asp.cur_fig = 1;
      }

      if(!check_shooting_position(layout, PlayerTank) && PlayerTank->aim->status == NORMAL_AIM){
        PlayerTank->aim->status = NO_FIRE;
        PlayerTank->aim->asp.cur_fig = 1;
      }
    }
  }

}
/**
 * @brief simulates a fake shooting to see if shooting is indeed possible in the tank's current situation, returns true if it is
 * 
 */
bool (check_shooting_position) (Tile_t layout[][NCOLS], Tank_t * tank){
  Ball_t * temp_ball = create_newball(tank, false);

  //Ball_t * temp_ball = create_newball(*create_animSprite_spec(0, 0, 0, 0, ANIMSPEED, 2, ball_maps[0], ball_maps[1]), PlayerTank, false);

  ball_init_position(tank, temp_ball);

  int frontier_temp = 0;
  return check_colision_coord(temp_ball, layout, &frontier_temp) == NO_C;
}

//Frame Loop Related
/**
 * @brief updates the PlayerTank position according to the movement array and if there is no colision to stop it
 * 
 */
void (update_player_pos)(Tile_t layout[][NCOLS], Tank_t * PlayerTank, Tank_t * tanks[MAX_TANKS]){
  int correction = 0;
  if(PlayerTank->cooldown_count < PlayerTank->cooldown_frames) PlayerTank->cooldown_count++;

  if(PlayerTank->asp.cur_fig != 18){
    if(movement[0]){
      if(!check_colision(PlayerTank, layout, NCY, & correction, tanks)){
        PlayerTank->asp.sp->y -= PlayerTank->asp.sp->yspeed + correction;
        correction = 0;
      }
    }

    if(movement[1]){
      if(!check_colision(PlayerTank, layout, NCX, & correction, tanks)){
        PlayerTank->asp.sp->x -= PlayerTank->asp.sp->xspeed + correction;
        correction = 0;
      }
    }

    if(movement[2]){
      if(!check_colision(PlayerTank, layout, CY, & correction, tanks)){
        PlayerTank->asp.sp->y += PlayerTank->asp.sp->yspeed - correction;
        correction = 0;
      }
    }

    if(movement[3]){
      if(!check_colision(PlayerTank, layout, CX, & correction, tanks)){
        PlayerTank->asp.sp->x += PlayerTank->asp.sp->xspeed - correction;
        correction = 0;
      } 
    }
  }
  
}
/**
 * @brief updates the position of every tank in the level's array
 * 
 */
void (update_tanks_pos)(Tile_t layout[][NCOLS], Tank_t * tanks[MAX_TANKS], Player_t * Player){
  for(int i = 0; MAX_TANKS > i; i++){
    if(tanks[i] != NULL && tanks[i]->type != TPLAYER && tanks[i]->state != DESTROYED){
      if(tanks[i]->state == ALIVE && tanks[i]->asp.cur_fig != 18){
        switch (tanks[i]->type){
          case XTANK:
            if(check_tank_colision(tanks[i], layout, tanks, 0, 0) != NO_COLISION){
              tanks[i]->asp.sp->xspeed = -tanks[i]->asp.sp->xspeed;
              int correction = 0;
              while(check_tank_colision(tanks[i], layout, tanks, tanks[i]->asp.sp->xspeed + correction, 0) != NO_COLISION && abs(correction) < abs(tanks[i]->asp.sp->xspeed)){
                if(tanks[i]->asp.sp->xspeed > 0) correction--;
                else correction++;
              }
              if(abs(correction) < abs(tanks[i]->asp.sp->xspeed)) tanks[i]->asp.sp->x += tanks[i]->asp.sp->xspeed + correction;
            }
            else{
              tanks[i]->asp.sp->x += tanks[i]->asp.sp->xspeed;
            }
            break;
        
          case YTANK:
            if(check_tank_colision(tanks[i], layout, tanks, 0, 0) != NO_COLISION){
              tanks[i]->asp.sp->yspeed = -tanks[i]->asp.sp->yspeed;
              int correction = 0;
              while(check_tank_colision(tanks[i], layout, tanks, 0, tanks[i]->asp.sp->yspeed + correction) != NO_COLISION && abs(correction) < abs(tanks[i]->asp.sp->yspeed)){
                if(tanks[i]->asp.sp->yspeed > 0) correction--;
                else correction++;
              }
              if(abs(correction) < abs(tanks[i]->asp.sp->yspeed)) tanks[i]->asp.sp->y += tanks[i]->asp.sp->yspeed + correction;
            }
            else{
              tanks[i]->asp.sp->y += tanks[i]->asp.sp->yspeed;
            }
            break;

          default:
            break;
        }
      }
    }
  }
}
/**
 * @brief handles the tank's shooting, having in mind their different types, which translate to different shooting strategies
 * 
 */
void (update_tanks_shooting)(Tile_t layout[][NCOLS], Tank_t * tanks[MAX_TANKS], Player_t * Player){
  for(int i = 0; MAX_TANKS > i; i++){
    if(tanks[i] != NULL && tanks[i]->type != TPLAYER && tanks[i]->state != DESTROYED){
      if(tanks[i]->state == ALIVE){
          if(tanks[i]->cooldown_count < tanks[i]->cooldown_frames) tanks[i]->cooldown_count++;
          if(tanks[i]->asp.cur_fig != 18){
            switch (tanks[i]->type){
              case XTANK:
                if(tanks[i]->cooldown_count == tanks[i]->cooldown_frames && tanks[i]->ball_count < tanks[i]->max_balls && check_shooting_position(layout, tanks[i])){
                  if(tanks[i]->asp.cur_fig == 4){
                    tanks[i]->angle = PI/2.0;
                    tanks[i]->aim_quadrant = VP;
                  }
                  else{
                    tanks[i]->angle = 3*PI/2.0;
                    tanks[i]->aim_quadrant = VN;
                  }
                  shoot(tanks[i]);
                }
                break;

              case YTANK:
                if(tanks[i]->cooldown_count == tanks[i]->cooldown_frames && tanks[i]->ball_count < tanks[i]->max_balls && check_shooting_position(layout, tanks[i])){
                  if(tanks[i]->asp.cur_fig == 0){
                    tanks[i]->angle = 0;
                    tanks[i]->aim_quadrant = XP;
                  }
                  else{
                    tanks[i]->angle = PI;
                    tanks[i]->aim_quadrant = XN;
                  }
                  shoot(tanks[i]);
                }
                break;
              
              case RANDOM_STATIC_TANK:

                if(tanks[i]->cooldown_count == tanks[i]->cooldown_frames && tanks[i]->ball_count < tanks[i]->max_balls && check_shooting_position(layout, tanks[i])){
                  if(random_event_percentage(25)){
                    shoot(tanks[i]);
                  }
                  else{
                    if(random_event_percentage(50)){
                      if(random_event_percentage(50)) tanks[i]->angle += PI/4.0;
                      else tanks[i]->angle -= PI/4.0;
                    }
                    else{
                      if(random_event_percentage(50)) tanks[i]->angle += PI/8.0;
                      else tanks[i]->angle -= PI/8.0;
                    }

                    set_slope_tank(tanks[i]);
                    update_tank_sprite(tanks[i]);
                    tanks[i]->cooldown_count = 20;
                  }
                }
                break;

              case DIRECT_STATIC_TANK:
                point_to_tank(tanks[i], Player->PlayerTank);
                if(tanks[i]->cooldown_count == tanks[i]->cooldown_frames && tanks[i]->ball_count < tanks[i]->max_balls && check_shooting_position(layout, tanks[i])){
                  if(check_direct_hit(tanks[i], tanks, layout)){
                    shoot(tanks[i]);
                  }
                }
                break;

              case INDIRECT_STATIC_TANK:
                if(tanks[i]->cooldown_count == tanks[i]->cooldown_frames && tanks[i]->ball_count < tanks[i]->max_balls && check_shooting_position(layout, tanks[i])){
                  if(try_indirect_shot(tanks[i], tanks, layout)){
                    shoot(tanks[i]);
                  }
                  else{
                    point_to_tank(tanks[i], Player->PlayerTank);
                  }
                }
                break;
        
              default:
                break;
            }
          }
        }
      }
    }

}
/**
 * @brief updates the position of every ball in the arrays of every tank in the level's array of tanks
 * 
 */
void (update_balls_positions) (Tank_t * tanks[MAX_TANKS], Tile_t layout[][NCOLS]){
  for(int i = 0; MAX_TANKS > i; i++){
    if(tanks[i] != NULL){
      int ball_counter_temp = 0;
      for(int j = 0; tanks[i]->max_balls > j; j++){
        if(tanks[i]->balls[j].state == ALIVE){
          if(tanks[i]->balls[j].state != DESTROYED) ball_counter_temp++;

          if((check_ball_colision)(tanks[i], & tanks[i]->balls[j], tanks, layout, true) == NO_COLISION){
            tanks[i]->balls[j].asp.sp->x += db_to_int(tanks[i]->balls[j].vx);
            tanks[i]->balls[j].asp.sp->y += db_to_int(tanks[i]->balls[j].vy);
          }
        }
      }
      tanks[i]->ball_count = ball_counter_temp;
    }
  }
}

/**
 * @brief draws every the entire screen to a buffer and uses update_screen to pass it to the video mem
 * 
 */
void (draw_screen)(Tile_t layout[][NCOLS], Tank_t * tanks[MAX_TANKS], Player_t * Player, Box_t menu_box, Clock_t clock){
  draw_level(layout, tanks, Player, clock);
  draw_animsprite(& Player->PlayerTank->asp);
  draw_tanks(tanks);
  draw_balls(tanks);
  draw_box(menu_box, mouse_over_box(Player, menu_box));
  if(!check_tank_aim_colision(Player->PlayerTank)) draw_animsprite(& Player->PlayerTank->aim->asp);
  update_screen();
}

//Draw Screen Related
/**
 * @brief copies one level layout to another
 * 
 */
void (layout_init)(Tile_t layout_dest[][NCOLS], Tile_t level_to_copy[][NCOLS]){
  
  for(int r = 0; NROWS > r; r++){
    for(int c = 0; NCOLS > c; c++){
      layout_dest[r][c] = level_to_copy[r][c];
    }
  }
}

/**
 * @brief copies one backgroung to another
 * 
 */
void (background_init)(uint16_t bg_dest[][BACKGROUND_NTILES], uint16_t bg_to_copy[][BACKGROUND_NTILES]){
  for(int r = 0; BACKGROUND_NTILES > r; r++){
    for(int c = 0; BACKGROUND_NTILES > c; c++){
      bg_dest[r][c] = bg_to_copy[r][c];
    }
  }
}

/**
 * @brief reads a level's layout and draws them, in the first time adds enemy tanks to the level's array of tanks as well as initializing every element in their initial positions 
 * 
 */
void (draw_level)(Tile_t layout[][NCOLS], Tank_t * tanks[MAX_TANKS], Player_t * Player, Clock_t clock){
  
  vg_draw_rectangle_buffer(0, 0, H_RES, RESERVED_Y, MENU_COLOR);
  draw_animsprite(create_animSprite_spec(10, 10, 0, 0, ANIMSPEED, 1, tank_E));
  char plives[5];
  sprintf(plives, "X%02d", Player->lives);
  draw_string(45, 10, false, plives);
  draw_clock(CLOCK_X, CLOCK_Y, 0, &clock);

  vg_draw_rectangle_buffer(0, RESERVED_Y, H_RES, XOFFSET, WALL);
  vg_draw_rectangle_buffer(0, RESERVED_Y, XOFFSET, V_RES - RESERVED_Y, WALL);
  vg_draw_rectangle_buffer(0, V_RES - XOFFSET, H_RES, XOFFSET, WALL);
  vg_draw_rectangle_buffer(H_RES - XOFFSET, RESERVED_Y, XOFFSET, V_RES - RESERVED_Y, WALL);

  for(int row = 0; NROWS > row; row++){
    for(int col = 0; NCOLS > col; col++){
      if(layout[row][col] >= 200){
        vg_draw_rectangle_buffer(XOFFSET + col * TILE_SIZE, YOFFSET + row * TILE_SIZE, TILE_SIZE, TILE_SIZE, FLOOR);

        if(layout[row][col] == PLAYER_TILE){
          Player->PlayerTank->asp.sp->x = (Player->PlayerTank->asp.sp->width / 4) + XOFFSET + col * TILE_SIZE;
          Player->PlayerTank->asp.sp->y = (Player->PlayerTank->asp.sp->height / 4) + YOFFSET +  row * TILE_SIZE;

          draw_animsprite(&Player->PlayerTank->asp);
          draw_animsprite(&Player->PlayerTank->aim->asp);
        }

        if(layout[row][col] == XTANK_U_TILE){
          create_new_XTank(tanks, true, TANK_L/4 + XOFFSET + col * TILE_SIZE, TANK_L/4 + YOFFSET +  row * TILE_SIZE);
        }

        if(layout[row][col] == XTANK_D_TILE){
          create_new_XTank(tanks, false, TANK_L/4 + XOFFSET + col * TILE_SIZE, TANK_L/4 + YOFFSET +  row * TILE_SIZE);
        }

        if(layout[row][col] == YTANK_R_TILE){
          create_new_YTank(tanks, true, TANK_L/4 + XOFFSET + col * TILE_SIZE, TANK_L/4 + YOFFSET +  row * TILE_SIZE);
        }

        if(layout[row][col] == YTANK_L_TILE){
          create_new_YTank(tanks, false, TANK_L/4 + XOFFSET + col * TILE_SIZE, TANK_L/4 + YOFFSET +  row * TILE_SIZE);
        }

        if(layout[row][col] == RSTANK_TILE){
          create_new_RSTank(tanks, get_rand_angle(), TANK_L/4 + XOFFSET + col * TILE_SIZE, TANK_L/4 + YOFFSET +  row * TILE_SIZE);
        }

        if(layout[row][col] == DSTANK_TILE){
          create_new_DSTank(tanks, get_rand_angle(), TANK_L/4 + XOFFSET + col * TILE_SIZE, TANK_L/4 + YOFFSET +  row * TILE_SIZE);
        }

        if(layout[row][col] == ISTANK_TILE){
          create_new_ISTank(tanks, get_rand_angle(), TANK_L/4 + XOFFSET + col * TILE_SIZE, TANK_L/4 + YOFFSET +  row * TILE_SIZE);
        }

        layout[row][col] = FLOOR;
      }
      else{
        vg_draw_rectangle_buffer(XOFFSET + col * TILE_SIZE, YOFFSET + row * TILE_SIZE, TILE_SIZE, TILE_SIZE, layout[row][col]);
      }
    }
  }
}
/**
 * @brief draws to the buffer every ball in the level
 * 
 */
void (draw_balls)(Tank_t * tanks[MAX_TANKS]){
  for(int i = 0; MAX_TANKS > i; i++){
    if(tanks[i] != NULL){
      for(int j = 0; tanks[i]->max_balls > j; j++){
        if(tanks[i]->balls[j].state != DESTROYED){
          if(tanks[i]->balls[j].state == BOOM){
            
            if(tanks[i]->balls[j].asp.cur_fig == 0) tanks[i]->balls[j].asp.cur_fig = 1;
            if(tanks[i]->balls[j].asp.cur_aspeed > 0)tanks[i]->balls[j].asp.cur_aspeed--;
            if(tanks[i]->balls[j].asp.cur_fig == 1 && tanks[i]->balls[j].asp.cur_aspeed <= 0){
              tanks[i]->balls[j].asp.cur_aspeed = tanks[i]->balls[j].asp.aspeed;
              tanks[i]->balls[j].asp.cur_fig = 2;
            }
            if(tanks[i]->balls[j].asp.cur_fig == 2 && tanks[i]->balls[j].asp.cur_aspeed <= 0){
              tanks[i]->balls[j].asp.cur_aspeed = tanks[i]->balls[j].asp.aspeed;
              tanks[i]->balls[j].state = DESTROYED;
            }

          } 
          draw_animsprite(& tanks[i]->balls[j].asp);
        }
      }
    }
  }
}
/**
 * @brief draws to the buffer every tank in the level
 * 
 */
void (draw_tanks)(Tank_t * tanks[MAX_TANKS]){
  for(int i = 0; MAX_TANKS > i; i++){
    if(tanks[i] != NULL && tanks[i]->state != DESTROYED){
      if(tanks[i]->asp.cur_fig == 18){
        if(tanks[i]->asp.cur_aspeed < tanks[i]->i_frames) tanks[i]->asp.cur_aspeed++;
        if(tanks[i]->asp.cur_aspeed >= tanks[i]->i_frames){
          tanks[i]->asp.cur_fig = tanks[i]->last_fig;
          tanks[i]->asp.cur_aspeed = tanks[i]->asp.aspeed;
        }
      }
      if(tanks[i]->type != TPLAYER){
        draw_animsprite(& tanks[i]->asp);
        if(tanks[i]->state == BOOM){
          if(tanks[i]->asp.cur_fig < 16) tanks[i]->asp.cur_fig = 16;
          if(tanks[i]->asp.cur_aspeed > 0) tanks[i]->asp.cur_aspeed--;
          if(tanks[i]->asp.cur_fig == 16 && tanks[i]->asp.cur_aspeed <= 0){
            tanks[i]->asp.cur_aspeed = tanks[i]->asp.aspeed;
            tanks[i]->asp.cur_fig = 17;
          }
          if(tanks[i]->asp.cur_fig == 17 && tanks[i]->asp.cur_aspeed <= 0){
            tanks[i]->state = DESTROYED;
          }
        }
      }
    }
  }

}

//Debug and Aux
/**
 * @brief prints in the screen which of the WASD keys are being pressed
 * 
 */
void (print_mov)(){
  char mov_message[10] = "MOV:     ";
  if(movement[0]) mov_message[5] = 'W';
  if(movement[1]) mov_message[6] = 'A';
  if(movement[2]) mov_message[7] = 'S';
  if(movement[3]) mov_message[8] = 'D';
  printf("\n %s \n", mov_message);
}
/**
 * @brief changes parameters x and y to the provided tank centre's x and y
 * 
 */
void (get_tank_centre) (Tank_t * tank, double * x, double * y){
  *x = tank->asp.sp->x + tank->asp.sp->width / 2;
  *y = tank->asp.sp->y + tank->asp.sp->height / 2;
}
/**
 * @brief sets a tanks angle and slope using the two points
 * 
 */
void (set_angle_slope) (double x1, double y1, double x2, double y2, double * angle, double * slope, Aim_Quadrant_t * aim_quadrant, bool * aim_right, bool * aim_up){
  double difx = x2 - x1;
  double dify = y1 - y2;

  if(difx != 0){
    *slope = (double)(dify / difx);
    *angle = atan(*slope);
  }
  
  if(x2 >= x1) *aim_right = true;
  else *aim_right = false;

  if(y2 <= y1) *aim_up = true;
  else *aim_up = false;

  if(difx == 0){
    *slope = 0;
    if(*aim_up){
      *aim_quadrant = VP;
      *angle = (PI)/2.0;
    } 
    else{
      *aim_quadrant = VN;
      *angle = 3*(PI)/2.0;
    }
  }
  else{
    if(*slope >= -1 && *slope <= 1 && *aim_right) *aim_quadrant = XP;
    if(*slope >= -1 && *slope <= 1 && (!(*aim_right))) *aim_quadrant = XN;
    if(*slope > 1 && *aim_up) *aim_quadrant = YP_XP;
    if(*slope < -1 && *aim_up) *aim_quadrant = YP_XN;
    if(*slope > 1 && (!(*aim_up))) *aim_quadrant = YN_XN;
    if(*slope < -1 && (!(*aim_up))) *aim_quadrant = YN_XP;

    if(*aim_quadrant == XN || *aim_quadrant == YP_XN || *aim_quadrant == YN_XN){
      *angle = (*angle + (PI));
    }
  
    if(*angle < 0) *angle += 2*PI;

  }
}
/**
 * @brief updates tank slope using the tank itself and a point
 * 
 */
void (set_angle_slope_tank) (Tank_t * tank, double x2, double y2){
  double centre_y_tank = 0;
  double centre_x_tank = 0;
  get_tank_centre(tank, &centre_x_tank, &centre_y_tank);

  set_angle_slope(centre_x_tank, centre_y_tank, x2, y2, &tank->angle, &tank->slope, &tank->aim_quadrant, &tank->aim_right, &tank->aim_up);
}
/**
 * @brief sets a tank's slope using the angle
 * 
 */
void (set_slope_tank) (Tank_t * tank){
  if(tank->angle < 0) tank->angle += 2*PI;
  if(tank->angle >= 2*PI) tank->angle -= 2*PI;

  if(tank->angle >= 3*PI/2.0 || tank->angle <= PI/2.0) tank->aim_right = true;
  else tank->aim_right = false;

  if(tank->angle <= PI) tank->aim_up = true;
  else tank->aim_up = false;

  if(tank->angle == PI/2.0 || tank->angle == 3*PI/2.0){
    tank->slope = 0;
    if(tank->aim_up){
      tank->aim_quadrant = VP;
      tank->angle = (PI)/2.0;
    } 
    else{
      tank->aim_quadrant = VN;
      tank->angle = 3*(PI)/2.0;
    }
  }
  else{
    tank->slope = tan(tank->angle);

    if(tank->slope >= -1 && tank->slope <= 1 && tank->aim_right) tank->aim_quadrant = XP;
    if(tank->slope >= -1 && tank->slope <= 1 && (!(tank->aim_right))) tank->aim_quadrant = XN;
    if(tank->slope > 1 && tank->aim_up) tank->aim_quadrant = YP_XP;
    if(tank->slope < -1 && tank->aim_up) tank->aim_quadrant = YP_XN;
    if(tank->slope > 1 && (!(tank->aim_up))) tank->aim_quadrant = YN_XN;
    if(tank->slope < -1 && (!(tank->aim_up))) tank->aim_quadrant = YN_XP;
  }
}
/**
 * @brief checks if a ball would hit the player if he would not move in that moment
 * 
 */
bool (check_player_hit) (Tank_t * tank, Ball_t * temp_ball, Tank_t * tanks[MAX_TANKS], Tile_t layout[][NCOLS]){
  Colision_t colision = check_ball_colision(tank, temp_ball, tanks, layout, false);
  int ball_its = 0;
  while(colision == NO_COLISION && ball_its < 1000){
    temp_ball->asp.sp->x += db_to_int(temp_ball->vx);
    temp_ball->asp.sp->y += db_to_int(temp_ball->vy);
    ball_its++;
    colision = check_ball_colision(tank, temp_ball, tanks, layout, false);
  }

  return colision == PLAYER_C;
}
/**
 * @brief checks if a ball would hit the player directly if he would not move in that moment
 * 
 */
bool (check_direct_hit) (Tank_t * tank, Tank_t * tanks[MAX_TANKS], Tile_t layout[][NCOLS]){
  Ball_t * temp_ball = create_newball(tank, false);
  ball_init_position(tank, temp_ball);

  return check_player_hit(tank, temp_ball, tanks, layout);
}
/**
 * @brief checks if a ball would hit the player after a ricochet if he would not move in that moment
 * 
 */
bool (check_indirect_hit) (Tank_t * tank, Tank_t * tanks[MAX_TANKS], Tile_t layout[][NCOLS]){
  Ball_t * temp_ball = create_newball(tank, false);
  ball_init_position(tank, temp_ball);

  int ball_its = 0;
  int temp_rebounds = temp_ball->rebounds;
  if(temp_rebounds == 0) return false;

  Colision_t colision = check_ball_colision(tank, temp_ball, tanks, layout, false);
  
  while(temp_rebounds >= 0){
    while(colision == NO_COLISION && ball_its < 1000){
    temp_ball->asp.sp->x += db_to_int(temp_ball->vx);
    temp_ball->asp.sp->y += db_to_int(temp_ball->vy);
    ball_its++;
    colision = check_ball_colision(tank, temp_ball, tanks, layout, false);
    }
    ball_its = 0;
    if(colision == TANK_C || colision == BALL_C || colision == SOLID_C) return false;
    temp_rebounds--;

    if(colision == PLAYER_C) return true;
  }
  
  return false;
}
/**
 * @brief tries multiple angles until one performs an indirect shot (return true) or none work (return false)
 * 
 */
bool (try_indirect_shot) (Tank_t * tank, Tank_t * tanks[MAX_TANKS], Tile_t layout[][NCOLS]){
  for(double ang = 0; 2*PI > ang;){
    tank->angle = ang;
    set_slope_tank(tank);
    if(!check_direct_hit(tank, tanks, layout) && check_indirect_hit(tank, tanks, layout)) return true;
    ang += PI / 12.0;
  }
  return false;
}
/**
 * @brief makes the tank point to the target tank, updating it's slope and angle accordingly
 * 
 */
void (point_to_tank) (Tank_t * tank, Tank_t * target){
  double xtarget = 0;
  double ytarget = 0;
  get_tank_centre(target, &xtarget, &ytarget);

  set_angle_slope_tank(tank, xtarget, ytarget);

  update_tank_sprite(tank);
}

/**
 * @brief draw letter using an index
 * 
 */
void (draw_letter_test) (uint16_t x, uint16_t y, bool alt, int ind){
  //vg_draw_xpmind_buffer(letter_size[ind], x, y);
  Sprite * sp = NULL;
  if(!alt) sp = create_sprite(letter_size_32[ind], x, y, 0, 0);
  else sp = create_sprite(letter_size_32_alt[ind], x, y, 0, 0);
  if(sp != NULL) draw_sprite(sp);
}

/**
 * @brief draw the correspondent character XPM on the given position on screen
 * 
 */
void (draw_letter) (uint16_t x, uint16_t y, bool alt, char c){
  //vg_draw_xpmind_buffer(letter_size[ind], x, y);
  if(c == ' '){
    Sprite * sp = create_sprite(letter_size_32_space, x, y, 0, 0);
    draw_sprite(sp);
    return;
  }
  if(c == ':'){
    Sprite * sp = NULL;
    if(!alt) sp = create_sprite(letter_size_32_tp, x, y, 0, 0);
    else sp = create_sprite(letter_size_32_tp_alt, x, y, 0, 0);
    if(sp != NULL) draw_sprite(sp);
    return;
  }
  if(c == '-'){
    Sprite * sp = NULL;
    if(!alt) sp = create_sprite(letter_size_32_minus, x, y, 0, 0);
    else sp = create_sprite(letter_size_32_minus_alt, x, y, 0, 0);
    if(sp != NULL) draw_sprite(sp);
    return;
  }
  if(c == '+'){
    Sprite * sp = NULL;
    if(!alt) sp = create_sprite(letter_size_32_plus, x, y, 0, 0);
    else sp = create_sprite(letter_size_32_plus_alt, x, y, 0, 0);
    if(sp != NULL) draw_sprite(sp);
    return;
  }
  
  if(c < 48 || (c > 57 && c < 65) || (c > 90 && c < 97) || c > 122) return;
  int ind = 0;
  if(c < 65) {
    ind = (int) c - 48;
    Sprite * sp = NULL;
    if(!alt) sp = create_sprite(numbers_size_32[ind], x, y, 0, 0);
    else sp = create_sprite(numbers_size_32_alt[ind], x, y, 0, 0);
    if(sp != NULL) draw_sprite(sp);
    return;
  }
  else{
    if(c < 90) ind = (int) c - 65;
    else ind = (int) c - 97;
  }
  

  Sprite * sp = NULL;
  if(!alt) sp = create_sprite(letter_size_32[ind], x, y, 0, 0);
  else sp = create_sprite(letter_size_32_alt[ind], x, y, 0, 0);
  if(sp != NULL) draw_sprite(sp);
}
/**
 * @brief draws a string, using draw_letter for each character of said string
 * 
 */
void (draw_string) (uint16_t x, uint16_t y, bool alt, char * string){
  for(size_t i = 0; strlen(string) > i; i++){
    draw_letter(x, y, alt, string[i]);
    x += TEXT_SIZE + 1;
  }
}
/**
 * @brief dras a timer containing seconds and minutes in the provided position
 * 
 */
void (draw_clock) (uint16_t x, uint16_t y, bool alt, Clock_t * clock){
  char s_seconds[3];
  char s_minutes[3];
  char string_timer[10] = "";
  sprintf(s_seconds, "%02d", clock->seconds);
  sprintf(s_minutes, "%02d", clock->minutes);
  strcat(string_timer, s_minutes);
  strcat(string_timer, ":");
  strcat(string_timer, s_seconds);
  draw_string(x, y, alt, string_timer);
}


/**
 * @brief updates the received clock, increasing or decreasing depending on that clock's boolean element "increasing"
 * 
 */
void (update_clock) (Clock_t * clock){
  if(clock->increasing){
    clock->timer_tot++;
    if(clock->timer_tot > 3600) clock->timer_tot = 0;
    clock->seconds++;
    if(clock->seconds >= 60){
      clock->seconds-= 60;
      clock->minutes++;
    }
    if(clock->minutes >= 60){
      clock->minutes = 0;
    }
  }
  else{
    if(clock->timer_tot >0) clock->timer_tot--;
    if(clock->seconds >0) clock->seconds--;
    if(clock->seconds <= 0){
      if(clock->minutes > 0) clock->seconds = 59;
      if(clock->minutes > 0) clock->minutes--;
    } 
  }
}
/**
 * @brief creates a string in an human readable format of the rtc's information
 * 
 */
void (make_rtc) (rtc_time rtc, char * string_rtc){
  char s_seconds[3];
  char s_minutes[3];
  char s_hours[3];
  char s_d[3];
  char s_m[3];
  char s_y[5];
  sprintf(s_seconds, "%02d", rtc.seconds);
  sprintf(s_minutes, "%02d", rtc.minutes);
  sprintf(s_hours, "%02d", rtc.hours);
  sprintf(s_d, "%02d", rtc.month_day);
  sprintf(s_m, "%02d", rtc.month);
  sprintf(s_y, "%02d", rtc.year);
  strcat(string_rtc, s_d);
  strcat(string_rtc, "-");
  strcat(string_rtc, s_m);
  strcat(string_rtc, "-");
  strcat(string_rtc, s_y);
  strcat(string_rtc, "  ");
  strcat(string_rtc, s_hours);
  strcat(string_rtc, ":");
  strcat(string_rtc, s_minutes);
  strcat(string_rtc, ":");
  strcat(string_rtc, s_seconds);
}
/**
 * @brief draws the information of the rtc using the make_rtc string
 * 
 */
void (draw_rtc) (uint16_t x, uint16_t y, bool alt, rtc_time rtc){
  char * string_rtc = malloc(sizeof(char) * 25);
  make_rtc(rtc, string_rtc);
  draw_string(x, y, alt, string_rtc);
}
/**
 * @brief puts all members of mouseinfo to zero 
 * 
 */
void (reset_mouse_info)(){
  mouseinfo[0] = 0;
  mouseinfo[1] = 0;
  mouseinfo[2] = 0;
  mouseinfo[3] = 0;
  mouseinfo[4] = 0;
  mouseinfo[5] = 0;
}
/**
 * @brief draws the given box, changes colors if mouse is over said box
 * 
 */
void (draw_box) (Box_t box, bool mouse_over){
  if(!mouse_over){
    vg_draw_rectangle_buffer(box.x, box.y, box.width, box.height, box.color);
    draw_string(box.x + BOX_RESERV_X, box.y + BOX_RESERV_Y, false, box.text);
  }
  else{
    vg_draw_rectangle_buffer(box.x, box.y, box.width, box.height, box.frame_color);
    vg_draw_rectangle_buffer(box.x + BOX_FRAME, box.y + BOX_FRAME, box.width - 2 * BOX_FRAME, box.height - 2 * BOX_FRAME, box.color);
    draw_string(box.x + BOX_RESERV_X, box.y + BOX_RESERV_Y, true, box.text);
  }
}
/**
 * @brief return true if the cursor is on top of the box, false otherwise
 * 
 */
bool (mouse_over_box) (Player_t * Player, Box_t box){
  if(!box.clickable) return false;
  int mouse_x = Player->PlayerTank->aim->asp.sp->x;
  int mouse_y = Player->PlayerTank->aim->asp.sp->y;
  if(mouse_x >= box.x && mouse_x <= box.x + box.width && mouse_y >= box.y && mouse_y <= box.y + box.height) return true;
  return false;
}
/**
 * @brief draws a background
 * 
 */
void (draw_bg) (uint16_t bg[][BACKGROUND_NTILES]){
  for(int row = 0; BACKGROUND_NTILES > row; row++){
    for(int col = 0; BACKGROUND_NTILES > col; col++){
      vg_draw_rectangle_buffer(col * BG_XFACTOR, row * BG_YFACTOR, BG_XFACTOR, BG_YFACTOR, bg[row][col]);
    }
  }
}
/**
 * @brief draws the menu background and then all of the menu's boxes
 * 
 */
void (draw_menu) (Menu_t * menu, Player_t * Player, bool * transparent, bool rtc_d){
  if(*transparent){
    reset_screen(screen_saver);
  }
  else{
    draw_bg(menu->background);
    save_screen(screen_saver);
    *transparent = true;
  } 
  

  for(int i = 0; menu->num_boxes > i; i++){
    if (mouse_over_box(Player, menu->boxes[i])) draw_box(menu->boxes[i], true);
    else draw_box(menu->boxes[i], false);
  }

  if(rtc_d) draw_rtc(RTC_X, RTC_Y, false, rtc);

  draw_animsprite(& Player->PlayerTank->aim->asp);

  update_screen();
}
/**
 * @brief calls draw_menu and checks if any option has been choosed
 * 
 */
int (menu_loop) (Menu_t * menu, Player_t * Player, bool * transparent, bool rtc_d, bool click){
  if(mouseinfo[0]) Player->PlayerTank->aim->asp.cur_fig = 4;
  else Player->PlayerTank->aim->asp.cur_fig = 3;

  draw_menu(menu, Player, transparent, rtc_d);

  for(int i = 0; menu->num_boxes > i; i++){
    if (click && mouse_over_box(Player, menu->boxes[i]) && mouseinfo[3]) return i;
    if (!click && mouse_over_box(Player, menu->boxes[i]) && mouseinfo[0]) return i;
  }
  return -1;
}
/**
 * @brief returns the x that a box must have in order to be align with the centre of the screen
 * 
 */
int (align_mid) (size_t lstr){
  int target = H_RES / 2;
  return (int) (target - ( ((lstr * 32) /2) + BOX_FRAME + BOX_X_PIX ));
}














//TEST

void(test)(){
  
  //GL!!!!!!!!
  game_loop();

  //if(level_tanks[0]->balls[0] != NULL) printf("\n BALL FOUND \n");
  //else printf("\n ;( \n");

  /**
  Aim = *create_newaim(*create_animSprite_spec(100, 400 , AIMSPEED, AIMSPEED, ANIMSPEED, NSPRITES_AIM, aim_maps[0], aim_maps[1]));

  Tank_t T1 = *create_newtank(*create_animSprite_spec(420, 420, TANKSPEED, TANKSPEED, ANIMSPEED, NSPRITES_TANK,
        tank_maps[0], tank_maps[1], tank_maps[2]), &Aim, 5, 2, 2, 1);

  Ball_t * ball = create_newball(*create_animSprite_spec(0, 0, 0, 0, ANIMSPEED, NSPRITES_BALL, ball_maps[0]), &T1, BALL_VEL);

  update_aim(&T1);
  ball_init_position( &T1, ball);*/

  //printf("\n BX: %d \n BY: %d \n", ball->asp.sp->x, ball->asp.sp->y);*/

  /*
  int a = db_to_int(level_tanks[0]->angle);
  int s = db_to_int(level_tanks[0]->slope);
  int bc = level_tanks[0]->ball_count;

  printf("\n Ang: %d \n Slope: %d \n BallCount: %d \n", a, s, bc);
  
  for(int i = 0; MAX_TANKS > i; i++){
    if(lballs[i] != NULL){
      uint16_t bx = lballs[i]->asp.sp->x;
      uint16_t by = lballs[i]->asp.sp->y;
      int breb = lballs[i]->rebounds;
      uint16_t bvx = db_to_int(lballs[i]->vx);
      uint16_t bvy = db_to_int(lballs[i]->vy);
      uint16_t bang = db_to_int(lballs[i]->angle);
      printf("\n BX: %d \n BY: %d \n BVX: %d \n BVY: %d \n Rebounds: %d \n Ang: %d \n", bx, by, bvx, bvy, breb, bang);
    }
  }*/
  

  //bool cx = true;
  //for(int j = 0; 6 > j; j++){
    //if(cx) printf("\n X: %d \n", ballxy[j]);
    //else printf("Y: %d \n", ballxy[j]);
    //if(cx) cx = false;
    //else cx = true;
  //}

  //int tx = level_tanks[0]->asp.sp->x;
  //int ty = level_tanks[0]->asp.sp->y;
  //printf("\n  TX: %d  \n  TY: %d \n", tx, ty);

  /*
  Aim_Quadrant_t q = level_tanks[0]->aim_quadrant;
  switch(q){
    case XP:
      printf("\n XP \n");
      break;
    case YP_XP:
      printf("\n YP_XP \n");
      break;
    case VP:
      printf("\n VP \n");
      break;
    case YP_XN:
      printf("\n YP_XN \n");
      break;
    case XN:
      printf("\n XN \n");
      break;
    case YN_XN:
      printf("\n YN_XN \n");
      break;
    case VN:
      printf("\n VN \n");
      break;
    case YN_XP:
      printf("\n YN_XP \n");
      break;
    default:
      break;
  }*/

  /*
  int ccc = 0;
  while(ccc < 10){
    int a = get_rand(8, 48);
    printf("\n %d", a);
    ccc++;
  }*/

  //printf("\n %s \n", mc);
  printf("\n DONE \n");

  /*
  char a = 'b';
  printf("\n %d \n", a);
  */
}



